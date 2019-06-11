#include "System.h"

#include <dos.h>
#include <iostream.h>
#include "SCHEDULE.H"
#include "thread.h"
#include "pcb.h"

ForwardList<PCB*> System::all_pcbs;

volatile long long System::ticks_since_boot = 0L;

volatile PCB *System::running_pcb = nullptr;


int System::context_switch_requested = 0;
volatile int System::lock_flag = 1;

const Time System::main_time_slice = 2U;
PCB System::main_pcb(defaultStackSize, main_time_slice, nullptr);

Time System::running_pcb_time_slice = System::main_time_slice;


pInterrupt old_timer_interrupt;

/**
 * Needed in tests
 */
void tick();

void interrupt timer(...) {
	//cout << System::running_pcb_time_slice << "\n";
	//asm cli;
	if (!System::context_switch_requested) {
		tick();
		System::ticks_since_boot++;
		System::running_pcb_time_slice--;	// TODO: time_slice == 0 => unbounded execution	
	}
	
	if (System::context_switch_requested || System::running_pcb_time_slice == 0) {
		if (System::lock_flag) {
			System::context_switch_requested = false;
			System::running_pcb->stack_offset_ = _SP;
			System::running_pcb->stack_segment_ = _SS;
			System::running_pcb->base_pointer_ = _BP;

			if (System::running_pcb->state_ != PCB::COMPLETED && System::running_pcb->state_ != PCB::BLOCKED) {
				System::running_pcb->state_ = PCB::READY;
				Scheduler::put((PCB*)System::running_pcb);
			}

			System::running_pcb = Scheduler::get();	// TODO: check nullptr for idle

			_SP = System::running_pcb->stack_offset_;
			_SS = System::running_pcb->stack_segment_;
			_BP = System::running_pcb->base_pointer_;
			System::running_pcb_time_slice = System::running_pcb->time_slice_;
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
	running_pcb = &main_pcb;
#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif
}

void System::restore() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(0x8, old_timer_interrupt);
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
