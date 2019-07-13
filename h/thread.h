#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;

const StackSize defaultStackSize = 4096;

typedef unsigned int Time; // time, x 55ms

const Time defaultTimeSlice = 2; // default = 2*55ms

typedef int ID;

class PCB; // Kernel's implementation of a user's thread

#define THREAD_SIGNALS // uncomment if thread signals should not work
#ifdef THREAD_SIGNALS

typedef void (*SignalHandler)();

typedef unsigned SignalId;

#endif

void interrupt timer(...); // Interrupt routine for timer

class Thread {
public:
    void start();

    void waitToComplete();
    
    virtual ~Thread();
    
    ID getId();
    
    static ID getRunningId();
    
    static Thread *getThreadById(ID id);

#ifdef THREAD_SIGNALS
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
    friend void interrupt timer(...); // needed for idle thread
    friend class PCB;

    Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);

    virtual void run() {}
    
private:
    PCB *myPCB;
};

void dispatch();

#endif
