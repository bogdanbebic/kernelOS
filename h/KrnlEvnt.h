#ifndef _KERNEL_EVENT_H_
#define _KERNEL_EVENT_H_

#include "bcconsts.h"

#include "event.h"

class PCB;

class KernelEv {
public:
    KernelEv(IVTNo ivt_number);
    ~KernelEv();

    void wait();
    void signal();
private:
    IVTNo ivt_number_;
    PCB *owner_;
};

#endif
