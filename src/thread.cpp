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
    // delete this->myPCB;
    // this->myPCB = 0;
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

#ifdef THREAD_SIGNALS

void Thread::signal(SignalId signal) {
    lock;
    this->myPCB->signal(signal);
    unlock;
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
    lock;
    this->myPCB->registerHandler(signal, handler);
    unlock;
}

void Thread::unregisterAllHandlers(SignalId id) {
    lock;
    this->myPCB->unregisterAllHandlers(id);
    unlock;
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
    lock;
    this->myPCB->swap(id, hand1, hand2);
    unlock;
}
    
void Thread::blockSignal(SignalId signal) {
    lock;
    this->myPCB->blockSignal(signal);
    unlock;
}

void Thread::blockSignalGlobally(SignalId signal) {
    lock;
    PCB::blockSignalGlobally(signal);
    unlock;
}

void Thread::unblockSignal(SignalId signal) {
    lock;
    this->myPCB->unblockSignal(signal);
    unlock;
}

void Thread::unblockSignalGlobally(SignalId signal) {
    lock;
    PCB::unblockSignalGlobally(signal);
    unlock;
}

#endif
