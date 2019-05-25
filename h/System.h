#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "bcconsts.h"
#include "pcb.h"

#define lock() System::lock_flag = 0

#define unlock() \
System::lock_flag = 1;\
if (System::context_switch_requested) dispatch()

typedef void interrupt (*pInterrupt)(...);

void interrupt timer(...);

class System {
public:

	/**
	 * Initializes the kernel of the system
	 */
	static void initialize();

	/**
	 * Restores all resources changed by initialize
	 */
	static void restore();

	static void dispatch();

	/**
	 * Running PCB
	 */
	static volatile PCB *running_pcb;

	static Time running_pcb_time_slice;

	static bool context_switch_requested;
	static volatile int lock_flag;

	static const Time main_time_slice;
	static const PCB *main_pcb;

	friend void interrupt timer(...);

private:
	static volatile long long ticks_since_boot;
};

#endif // _SYSTEM_H_
