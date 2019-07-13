#include "KrnlEvnt.h"

#include "pcb.h"
#include "System.h"

#include "SCHEDULE.H"

KernelEv::KernelEv(IVTNo ivt_number)
    : ivt_number_(ivt_number)
{
    lock;
    System::ivt_entries[ivt_number]->my_event_ = this;
    this->owner_ = (PCB*)System::running_pcb;
    unlock;
}

KernelEv::~KernelEv() {
    lock;
    System::ivt_entries[this->ivt_number_]->my_event_ = nullptr;
    unlock;
}

void KernelEv::wait() {
    lock;
    if (this->owner_ == System::running_pcb) {
        this->owner_->state_ = PCB::BLOCKED;
        unlock;
        dispatch();
    }
    else {
        unlock;
    }

}

void KernelEv::signal() {
    lock;
    this->owner_->state_ = PCB::READY;
    Scheduler::put(this->owner_);
    unlock;
}
