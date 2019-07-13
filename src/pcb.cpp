#include "pcb.h"

#include "DOS.H"
#include "SCHEDULE.H"

ID PCB::next_id = 0;

#ifdef THREAD_SIGNALS
bool PCB::is_signal_blocked_globally[PCB::NUMBER_OF_SIGNALS];
#endif

PCB::PCB(StackSize stack_size, Time time_slice, Thread *my_thread) 
    : my_thread_(my_thread)
    , id_(next_id++)
    , stack_size_(stack_size)
    , state_(NEW)
    , time_slice_(time_slice)
    , is_unbounded_(time_slice == 0)
    , woken_by_signal_(false)
{
    lock;
    this->stack_ = new Word[this->stack_size_ / sizeof(Word)];
    this->stack_[this->stack_size_ - 1] = 0x200; // I bit in psw set
    this->stack_[this->stack_size_ - 2] = FP_SEG(PCB::wrapper);
	this->stack_[this->stack_size_ - 3] = FP_OFF(PCB::wrapper);

    this->stack_segment_ = FP_SEG(this->stack_ + this->stack_size_ - 12);
	this->stack_offset_ = FP_OFF(this->stack_ + this->stack_size_ - 12);
	this->base_pointer_ = FP_OFF(this->stack_ + this->stack_size_ - 12);
    System::all_pcbs.push_back(this);

#ifdef THREAD_SIGNALS
    this->parent_ = (PCB*)System::running_pcb;

    for (int i = 0; i < NUMBER_OF_SIGNALS; i++) {
        if (this->parent_ != nullptr) {
            this->is_signal_blocked_[i] = this->parent_->is_signal_blocked_[i];
            
            for (Queue<SignalHandler>::size_type num_handlers = 0; num_handlers < this->parent_->signal_handlers_[i].size(); num_handlers++) {
                SignalHandler handler = this->parent_->signal_handlers_[i].front();
                this->parent_->signal_handlers_[i].pop();
                this->parent_->signal_handlers_[i].push(handler);
                this->signal_handlers_[i].push(handler);
            }

        }
        else {
            this->is_signal_blocked_[i] = false;
        }

    }

#endif

    unlock;
}

PCB::~PCB() {
    //lock;
    delete [] this->stack_;
    //unlock;
}

void PCB::start() {
    lock;
    if (this->state_ == NEW) {
        this->state_ = READY;
        Scheduler::put(this);
    }

    unlock;
}

void PCB::waitToComplete() {
    lock;
    if (this->state_ != COMPLETED) {
        System::running_pcb->state_ = BLOCKED;
        this->waiting_on_this_.push_back((PCB*)System::running_pcb);
        unlock;
        dispatch();
        return;
    }

    unlock;
}

ID PCB::id() const {
    return this->id_;
}

void PCB::wrapper() {
    System::running_pcb->my_thread_->run();
    lock;
    for (ForwardList<PCB*>::Iterator it = ((PCB*)System::running_pcb)->waiting_on_this_.begin();
        it != ((PCB*)System::running_pcb)->waiting_on_this_.end(); ++it) {

        (*it)->state_ = READY;
        Scheduler::put(*it);
    }

    #ifdef THREAD_SIGNALS
    
    if (System::running_pcb->parent_ != nullptr) {
        System::running_pcb->parent_->signal(SIG1);
    }

    ((PCB*)System::running_pcb)->signal(SIG2);

    #endif

    System::running_pcb->state_ = COMPLETED;
    unlock;
    dispatch();
}

#ifdef THREAD_SIGNALS

void PCB::signal(SignalId signal) {
    if (signal < NUMBER_OF_SIGNALS) {
        this->signals_received_.push(signal);
    }

}

void PCB::registerHandler(SignalId signal, SignalHandler handler) {
    if (signal < NUMBER_OF_SIGNALS) {
        this->signal_handlers_[signal].push(handler);
    }

}

void PCB::unregisterAllHandlers(SignalId id) {
    if (id < NUMBER_OF_SIGNALS) {
        this->signal_handlers_[id].clear();
    }

}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
    if (id < NUMBER_OF_SIGNALS) {
        this->signal_handlers_[id].swap(hand1, hand2);
    }

}
    
void PCB::blockSignal(SignalId signal) {
    if (signal < NUMBER_OF_SIGNALS) {
        this->is_signal_blocked_[signal] = true;
    }

}

void PCB::blockSignalGlobally(SignalId signal) {
    if (signal < NUMBER_OF_SIGNALS) {
        is_signal_blocked_globally[signal] = true;
    }

}

void PCB::unblockSignal(SignalId signal) {
    if (signal < NUMBER_OF_SIGNALS) {
        this->is_signal_blocked_[signal] = false;
    }

}

void PCB::unblockSignalGlobally(SignalId signal) {
    if (signal < NUMBER_OF_SIGNALS) {
        is_signal_blocked_globally[signal] = false;
    }

}

void PCB::handle_signals() {
    for (Queue<SignalId>::size_type i = this->signals_received_.size(); i > 0; i--) {
        SignalId signal_to_process = this->signals_received_.front();
        this->signals_received_.pop();
        if (this->is_signal_blocked_[signal_to_process] || PCB::is_signal_blocked_globally[signal_to_process]) {
            this->signals_received_.push(signal_to_process);
            continue;
        }

        // signal_to_process should be handled
        for (Queue<SignalHandler>::size_type num_handlers = 0; num_handlers < this->signal_handlers_[signal_to_process].size(); num_handlers++) {
            SignalHandler handler = this->signal_handlers_[signal_to_process].front();
            this->signal_handlers_[signal_to_process].pop();
            this->signal_handlers_[signal_to_process].push(handler);
            handler();
        }

        //cout << "\nHandle signal:" << signal_to_process << "\n";
        if (signal_to_process == SIG0) {
            this->signals_received_.clear();
            for (ForwardList<PCB*>::Iterator it = this->waiting_on_this_.begin();
                it != this->waiting_on_this_.end(); ++it) {

                (*it)->state_ = READY;
                Scheduler::put(*it);
            }

            this->state_ = COMPLETED;
            unlock;
            dispatch();
        }
        
    }

}

#endif
