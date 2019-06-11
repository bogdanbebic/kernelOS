#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "bcconsts.h"
#include "System.h"

#include "list.h"

class PCB {
public:
    PCB(StackSize stack_size, Time time_slice, Thread *my_thread);
    
    ~PCB();

    enum State { NEW, READY, RUNNING, BLOCKED, COMPLETED };

    void start();

    void waitToComplete();

    ID id() const;

    static void wrapper();

protected:

private:
    friend void interrupt timer(...);
    friend void System::initialize();
    friend Thread *Thread::getThreadById(ID id);

    static ID next_id;
    
    Thread *my_thread_;

    ID id_;
    StackSize stack_size_;
    State state_;
    Time time_slice_;
    Word *stack_;
    Word stack_segment_;
    Word stack_offset_;
    Word base_pointer_;

    ForwardList<PCB*> waiting_on_this_;

};

#endif
