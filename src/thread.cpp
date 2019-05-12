#include "thread.h"

#include "System.h"
#include "pcb.h"

void Thread::start() {
    this->myPCB->start();
}

void Thread::waitToComplete() {
    // TODO: implement
}

Thread::~Thread() {
    delete this->myPCB;
}

ID Thread::getId() {
    return this->myPCB->id();
}

ID Thread::getRunningId() {
    return ((PCB*)System::running_pcb)->id();
}

Thread *Thread::getThreadById(ID id) {
    return nullptr; // TODO: implement
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
