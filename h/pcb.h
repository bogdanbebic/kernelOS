#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "bcconsts.h"

class PCB {
public:
    PCB(StackSize stack_size, Time time_slice, Thread *my_thread);
    
    ~PCB();

    enum State { NEW, READY, RUNNING, BLOCKED, COMPLETED };

    void start();

    ID id() const;

    static void wrapper();

protected:

private:
    friend void interrupt timer(...);

    static long long next_id;
    
    Thread *my_thread_;

    long long id_;
    StackSize stack_size_;
    State state_;
    Time time_slice_;
    Word *stack_;
    Word stack_segment_;
    Word stack_offset_;
    Word stack_pointer_;
    Word base_pointer_;

    // Vector<PCB*> waiting_on_this;

};

#endif
