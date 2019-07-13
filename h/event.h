// File: event.h
#ifndef _event_h_
#define _event_h_

#include "System.h"
#include "IVTEntry.h"

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
    Event(IVTNo ivtNo);

    ~Event();

    void wait();

protected:
    friend class KernelEv;

    void signal(); // can call KernelEv

private:
    KernelEv * myImpl;

};


#define PREPAREENTRY(ivt_number, is_old_interrupt_called)       \
void interrupt generated_interrupt_number_ ## ivt_number(...);  \
IVTEntry generated_ivt_entry ## ivt_number(ivt_number,          \
    generated_interrupt_number_ ## ivt_number);                 \
void interrupt generated_interrupt_number_ ## ivt_number(...) { \
    generated_ivt_entry ## ivt_number.signal_event();           \
    if (is_old_interrupt_called) {                              \
        generated_ivt_entry ## ivt_number.call_old_interrupt(); \
    }                                                           \
}                                                               \


#endif
