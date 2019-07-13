#include "KrnlSem.h"

#include "System.h"
#include "utility.h"
#include "SCHEDULE.H"

KernelSem::KernelSem(int initial_value, Semaphore *my_semaphore)
    : value_(initial_value)
    , my_semaphore_(my_semaphore)
{
    System::all_kernel_sems.push_back(this);
}

KernelSem::~KernelSem() {
    System::all_kernel_sems.remove(this);
}

int KernelSem::wait(Time max_time_to_wait) {
    lock;
    if (this->value_-- > 0) {
        unlock;
        return 1;
    }

    System::running_pcb->state_ = PCB::BLOCKED;
    if (max_time_to_wait == 0) {
        this->blocked_threads_.push((PCB*)System::running_pcb);
    }
    else {
        this->time_blocked_threads_.push(TimeBlockedThread((PCB*)System::running_pcb, System::ticks_since_boot + max_time_to_wait));
    }

    unlock;
    dispatch();
    if (!System::running_pcb->woken_by_signal_) {
        System::running_pcb->woken_by_signal_ = false;
        return 0;
    }
    
    return 1;
}

int KernelSem::signal(int n) {
    if (n < 0) {
        return n;
    }
    else if (n == 0) {
        n = 1;
    }

    lock;
    this->value_ += n;
    int number_of_threads_to_wake_up = n;
    int to_wake = min(number_of_threads_to_wake_up, (int)this->blocked_threads_.size());
    for (int i = 0; i < to_wake; i++) {
        PCB *pcb = this->blocked_threads_.front();
        this->blocked_threads_.pop();
        pcb->state_ = PCB::READY;
        Scheduler::put(pcb);
        pcb->woken_by_signal_ = true;
    }

    number_of_threads_to_wake_up -= to_wake;
    while (number_of_threads_to_wake_up > 0 && this->time_blocked_threads_.size() > 0) {
        number_of_threads_to_wake_up--;
        PCB *pcb = this->time_blocked_threads_.top().blocked_pcb;
        this->time_blocked_threads_.pop();
        pcb->state_ = PCB::READY;
        Scheduler::put(pcb);
        pcb->woken_by_signal_ = true;
    }
    
    unlock;
    return 0;
}

int KernelSem::value() const {
    return this->value_;
}

void KernelSem::time_wake_threads() {
    lock;
    while (this->time_blocked_threads_.size() > 0 && this->time_blocked_threads_.top().absolute_time_to_wake_up <= System::ticks_since_boot) {
        PCB *pcb = this->time_blocked_threads_.top().blocked_pcb;
        this->time_blocked_threads_.pop();
        pcb->state_ = PCB::READY;
        Scheduler::put(pcb);
        pcb->woken_by_signal_ = false;
    }

    unlock;
}
