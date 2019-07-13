#ifndef _INTERRUPT_VECTOR_TABLE_ENTRY_H_
#define _INTERRUPT_VECTOR_TABLE_ENTRY_H_

#include "bcconsts.h"

#include "System.h"

typedef unsigned char IVTNo;

class KernelEv;

/**
 * Interrupt Vector Table Entry
 */
class IVTEntry {
public:
    IVTEntry(IVTNo ivt_number, pInterrupt new_interrupt);
    ~IVTEntry();
    void call_old_interrupt();
    void signal_event();
private:
    friend class KernelEv;
    pInterrupt old_interrupt_;
    IVTNo ivt_number_;
    KernelEv *my_event_;
};

#endif
