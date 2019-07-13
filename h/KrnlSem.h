#ifndef _KERNEL_SEMAPHORE_H_
#define _KERNEL_SEMAPHORE_H_

/*
 * Needed for:
 * INT_MAX and INT_MIN
 */
#include <LIMITS.H> 

#include "semaphor.h"

#include "bcconsts.h"
#include "pcb.h"
#include "queue.h"

#include "compares.h"

#include <IOSTREAM.H>

class KernelSem {
public:
    KernelSem(int initial_value, Semaphore *my_semaphore);
    
    int wait(Time max_time_to_wait);
    int signal(int n);

    int value() const;

    ~KernelSem();
private:
    Semaphore *my_semaphore_;
    int value_;

    struct TimeBlockedThread {
        PCB *blocked_pcb;
        volatile unsigned long long absolute_time_to_wake_up;
        TimeBlockedThread(PCB *pcb, unsigned long long absolute_time_when_to_wake_up)
            : blocked_pcb(pcb)
            , absolute_time_to_wake_up(absolute_time_when_to_wake_up)
        {}

        bool operator == (const TimeBlockedThread & other) const { return false; }
        struct LessTimeBlockedThread {
            bool operator () (const TimeBlockedThread & lhs, const TimeBlockedThread & rhs) {
                return lhs.absolute_time_to_wake_up < rhs.absolute_time_to_wake_up;
            }

        };

    };

    Queue<PCB*> blocked_threads_;
    PriorityQueue<TimeBlockedThread, TimeBlockedThread::LessTimeBlockedThread> time_blocked_threads_;

    friend void interrupt timer(...);
    void time_wake_threads();
};

#endif
