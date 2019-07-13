#include "IVTEntry.h"

#include "DOS.H"

#include "KrnlEvnt.h"

IVTEntry::IVTEntry(IVTNo ivt_number, pInterrupt new_interrupt)
    : ivt_number_(ivt_number)
    , my_event_(nullptr)
{
#ifndef BCC_BLOCK_IGNORE
    lock;
    this->old_interrupt_ = getvect(ivt_number);
    setvect(ivt_number, new_interrupt);
    System::ivt_entries[ivt_number] = this;
    unlock;
#endif
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
    lock;
    setvect(this->ivt_number_, this->old_interrupt_);
    System::ivt_entries[this->ivt_number_] = nullptr;
    unlock;
#endif
}

void IVTEntry::call_old_interrupt() {
    this->old_interrupt_();
}

void IVTEntry::signal_event() {
    if (this->my_event_ != nullptr) {
        this->my_event_->signal();
    }
    
}
