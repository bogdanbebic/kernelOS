#include "semaphor.h"

#include "KrnlSem.h"

Semaphore::Semaphore(int init) {
    this->myImpl = new KernelSem(init, this);
}

Semaphore::~Semaphore() {
    lock;
    // delete this->myImpl;
    // this->myImpl = 0;
    unlock;
}

int Semaphore::wait(Time maxTimeToWait) {
    return this->myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n) {
    return this->myImpl->signal(n);
}

int Semaphore::val() const {
    return this->myImpl->value();
}
