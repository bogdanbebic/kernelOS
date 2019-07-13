#include "System.h"

#include <dos.h>
#include <iostream.h>
#include "SCHEDULE.H"
#include "thread.h"
#include "pcb.h"
#include "KrnlSem.h"

ForwardList<PCB*> System::all_pcbs;
ForwardList<KernelSem*> System::all_kernel_sems;

volatile unsigned long long System::ticks_since_boot = 0L;

volatile PCB *System::running_pcb = nullptr;


int System::context_switch_requested = 0;
volatile int System::lock_flag = 1;

const Time System::main_time_slice = 2U;
PCB System::main_pcb(defaultStackSize, main_time_slice, nullptr);

Time System::running_pcb_time_slice = System::main_time_slice;

System::IdleThread System::idle_thread;

IVTEntry *System::ivt_entries[NUMBER_OF_IVT_ENTRIES] = { nullptr };

pInterrupt old_timer_interrupt;

/**
 * Needed in tests
 */
void tick();

#include <IOSTREAM.H>

void interrupt timer(...) {
	if (!System::context_switch_requested) {
		tick();
		int i = 0;
		for (ForwardList<KernelSem*>::Iterator it = System::all_kernel_sems.begin(); i < System::all_kernel_sems.size(); i++, ++it) {
			(*it)->time_wake_threads();
		}
		
		System::ticks_since_boot++;
		if (!System::running_pcb->is_unbounded_) {
			System::running_pcb_time_slice--;
		}

	}
	
	if (System::context_switch_requested || System::running_pcb_time_slice == 0 && !System::running_pcb->is_unbounded_) {
		if (System::lock_flag) {
			System::context_switch_requested = false;
			System::running_pcb->stack_offset_ = _SP;
			System::running_pcb->stack_segment_ = _SS;
			System::running_pcb->base_pointer_ = _BP;

			if (System::running_pcb->state_ != PCB::COMPLETED && System::running_pcb->state_ != PCB::BLOCKED) {
				System::running_pcb->state_ = PCB::READY;
				Scheduler::put((PCB*)System::running_pcb);
			}

			do {
				System::running_pcb = Scheduler::get();
				if (System::running_pcb == nullptr) {
					System::running_pcb = System::idle_thread.myPCB;
				}

				System::running_pcb->state_ = PCB::RUNNING;
				_SP = System::running_pcb->stack_offset_;
				_SS = System::running_pcb->stack_segment_;
				_BP = System::running_pcb->base_pointer_;
				System::running_pcb_time_slice = System::running_pcb->time_slice_;

				#ifdef THREAD_SIGNALS
				lock;
				((PCB*)System::running_pcb)->handle_signals();
				if (System::running_pcb->state_ == PCB::COMPLETED) {
					delete [] System::running_pcb->stack_;
					System::running_pcb->stack_ = nullptr;
				}
				unlock;
				#endif
				
			} while (System::running_pcb->state_ == PCB::COMPLETED);

		}
		else {
			System::context_switch_requested = true;
		}

	}

	if (!System::context_switch_requested) {
		old_timer_interrupt();
	}

}


void System::initialize() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	old_timer_interrupt = getvect(0x8);
	setvect(0x8, timer);
#endif
	main_pcb.state_ = PCB::RUNNING;
	main_pcb.stack_segment_ = _SS;
	main_pcb.stack_offset_ = _SP;
	main_pcb.base_pointer_ = _BP;
	main_pcb.my_thread_ = &idle_thread;
	running_pcb = &main_pcb;

#ifdef THREAD_SIGNALS
	for (int i = 0; i < PCB::NUMBER_OF_SIGNALS; i++) {
		PCB::is_signal_blocked_globally[i] = false;
	}

#endif

#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif
}

void System::restore() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(0x8, old_timer_interrupt);
#endif

	for (ForwardList<PCB*>::Iterator it_pcbs = all_pcbs.begin(); it_pcbs != all_pcbs.end(); ++it_pcbs) {
		delete *it_pcbs;
	}

	for (ForwardList<KernelSem*>::Iterator it_sems = all_kernel_sems.begin(); it_sems != all_kernel_sems.end(); ++it_sems) {
		delete *it_sems;
	}

#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif
}

void System::dispatch() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	context_switch_requested = true;
	timer();
	asm sti;
#endif
}

void System::IdleThread::run() {
	while (this->is_active_);	
}
