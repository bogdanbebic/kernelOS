#include "pcb.h"

#include "DOS.H"
#include "SCHEDULE.H"

ID PCB::next_id = 0;

PCB::PCB(StackSize stack_size, Time time_slice, Thread *my_thread) 
    : my_thread_(my_thread)
    , id_(next_id++)
    , stack_size_(stack_size)
    , state_(NEW)
    , time_slice_(time_slice)
{
    this->stack_ = new Word[this->stack_size_];
    this->stack_[this->stack_size_ - 1] = 0x200; // I bit in psw set
    this->stack_[this->stack_size_ - 2] = FP_SEG(PCB::wrapper);
	this->stack_[this->stack_size_ - 3] = FP_OFF(PCB::wrapper);

    this->stack_segment_ = FP_SEG(this->stack_ + this->stack_size_ - 12);
	this->stack_offset_ = FP_OFF(this->stack_ + this->stack_size_ - 12);
	this->base_pointer_ = FP_OFF(this->stack_ + this->stack_size_ - 12);
    System::all_pcbs.push_back(this);
}

PCB::~PCB() {
    delete [] this->stack_;
}

void PCB::start() {
    if (this->state_ == NEW) {
        this->state_ = READY;
        Scheduler::put(this);
    }

}

void PCB::waitToComplete() {
    if (this->state_ == READY) {
        lock;
        System::running_pcb->state_ = BLOCKED;
        this->waiting_on_this_.push_back((PCB*)System::running_pcb);
        unlock;
        dispatch();
    }

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

    // place for signals

    unlock;
    System::running_pcb->state_ = COMPLETED;
    dispatch();
}
