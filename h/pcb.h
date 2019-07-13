#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "bcconsts.h"
#include "System.h"
#include "KrnlSem.h"
#include "list.h"

#ifdef THREAD_SIGNALS

#include "queue.h"

#endif

class PCB {
public:
    PCB(StackSize stack_size, Time time_slice, Thread *my_thread);
    
    ~PCB();

    enum State { NEW, READY, RUNNING, BLOCKED, COMPLETED };

    void start();

    void waitToComplete();

    ID id() const;

    static void wrapper();

#ifdef THREAD_SIGNALS

    enum Signals 
    { 
        SIG0, SIG1, SIG2, SIG3, SIG4,
        SIG5, SIG6, SIG7, SIG8, SIG9,
        SIG10, SIG11, SIG12, SIG13, SIG14, SIG15,
        NUMBER_OF_SIGNALS
    };

    void signal(SignalId signal);

    void registerHandler(SignalId signal, SignalHandler handler);
    void unregisterAllHandlers(SignalId id);
    void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
    
    void blockSignal(SignalId signal);
    static void blockSignalGlobally(SignalId signal);
    void unblockSignal(SignalId signal);
    static void unblockSignalGlobally(SignalId signal);
#endif

protected:

private:
    friend userMain(int argc, char *argv[]);
    friend void interrupt timer(...);
    friend void System::initialize();
    friend Thread *Thread::getThreadById(ID id);
    friend class KernelSem;
    friend class KernelEv;
    bool woken_by_signal_;

    static ID next_id;
    
    Thread *my_thread_;

    ID id_;
    StackSize stack_size_;
    State state_;
    Time time_slice_;
    bool is_unbounded_;
    Word *stack_;
    Word stack_segment_;
    Word stack_offset_;
    Word base_pointer_;

    ForwardList<PCB*> waiting_on_this_;

#ifdef THREAD_SIGNALS
    PCB *parent_;

    Queue<SignalId> signals_received_;
    Queue<SignalHandler> signal_handlers_[NUMBER_OF_SIGNALS];

    bool is_signal_blocked_[NUMBER_OF_SIGNALS];
    static bool is_signal_blocked_globally[NUMBER_OF_SIGNALS];

    void handle_signals();

#endif

};

#endif
