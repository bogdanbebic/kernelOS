#include "event.h"

#include "KrnlEvnt.h"

Event::Event(IVTNo ivtNo) {
    lock;
    this->myImpl = new KernelEv(ivtNo);
    unlock;
}

Event::~Event() {
    lock;
    delete this->myImpl;
    unlock;
}

void Event::wait() {
    this->myImpl->wait();
}

void Event::signal() {
    this->myImpl->signal();
}
