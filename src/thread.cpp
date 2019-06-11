#include "thread.h"

#include "System.h"
#include "pcb.h"

void Thread::start() {
    this->myPCB->start();
}

void Thread::waitToComplete() {
    this->myPCB->waitToComplete();
}

Thread::~Thread() {
    this->waitToComplete();
    delete this->myPCB;
}

ID Thread::getId() {
    return this->myPCB->id();
}

ID Thread::getRunningId() {
    return ((PCB*)System::running_pcb)->id();
}

Thread *Thread::getThreadById(ID id) {
    Thread *ret = nullptr;
    lock;
    for (ForwardList<PCB*>::Iterator it = System::all_pcbs.begin(); it != System::all_pcbs.end(); ++it) {
        if (id == (*it)->id()) {
            ret = (*it)->my_thread_;
            break;
        }

    }

    unlock;
    return ret;
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
    this->myPCB = new PCB(stackSize, timeSlice, this);
}

void dispatch() {
#ifndef BCC_BLOCK_IGNORE
    asm cli;
#endif
    System::context_switch_requested = true;
    timer();
#ifndef BCC_BLOCK_IGNORE
    asm sti;
#endif
}
