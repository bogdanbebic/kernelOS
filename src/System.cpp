#include "System.h"

#include <dos.h>
#include <iostream.h>
#include "SCHEDULE.H"

pInterrupt old_timer_interrupt;

/**
 * Needed in tests
 */
void tick();

void interrupt timer(...) {
	if (!System::context_switch_requested) {
		tick();
		System::ticks_since_boot++;
		System::running_pcb_time_slice--;
	}
	
	if (System::context_switch_requested || System::running_pcb_time_slice == 0) {
		System::running_pcb->stack_pointer_ = _SP;
		System::running_pcb->stack_segment_ = _SS;
		System::running_pcb->base_pointer_ = _BP;

		if (System::running_pcb->state_ != PCB::COMPLETED) {
			Scheduler::put((PCB*)System::running_pcb);
		}

		System::running_pcb = Scheduler::get();
		// TODO: check nullptr

		System::running_pcb_time_slice = System::running_pcb->time_slice_;
		_SP = System::running_pcb->stack_pointer_;
		_SS = System::running_pcb->stack_segment_;
		_BP = System::running_pcb->base_pointer_;
	}

	if (!System::context_switch_requested) {
		old_timer_interrupt();
	}

	cout << "Timer interrupt!\n";
	System::context_switch_requested = false;
}


void System::initialize() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	old_timer_interrupt = getvect(0x8);
	setvect(0x8, timer);
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

volatile long long System::ticks_since_boot = 0L;

volatile PCB *System::running_pcb = nullptr;

Time System::running_pcb_time_slice = 0;

bool System::context_switch_requested = false;

static const Time main_time_slice = 20;
static const PCB *main_pcb = nullptr;
