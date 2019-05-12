#include "System.h"

#include <dos.h>
#include <iostream.h>

pInterrupt old_timer_interrupt;

/**
 * Needed in tests
 */
void tick();

void interrupt timer(...) {
	tick();
	System::ticks_since_boot++;
	cout << "Timer interrupt!\n";
	old_timer_interrupt();
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
