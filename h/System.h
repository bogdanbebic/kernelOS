#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "bcconsts.h"
#include "thread.h"
#include "semaphor.h"

#include "list.h"

class PCB;

class IVTEntry;
#define NUMBER_OF_IVT_ENTRIES 256

#define lock System::lock_flag = 0

#define unlock \
System::lock_flag = 1;\
if (System::context_switch_requested) dispatch()

typedef void interrupt (*pInterrupt)(...);

void interrupt timer(...);

class System {
public:

	static ForwardList<PCB*> all_pcbs;
	static ForwardList<KernelSem*> all_kernel_sems;

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
	static PCB main_pcb;

	class IdleThread : public Thread {
	public:
		IdleThread() : Thread(defaultStackSize, 1U) { is_active_ = true; }
		~IdleThread() { is_active_ = false; waitToComplete(); }
	protected:
		friend void interrupt timer(...);
		bool is_active_;
		void run();
	};

	static IdleThread idle_thread;

	friend void interrupt timer(...);
	
	static volatile unsigned long long ticks_since_boot;

	static IVTEntry *ivt_entries[NUMBER_OF_IVT_ENTRIES];
};

#endif // _SYSTEM_H_
