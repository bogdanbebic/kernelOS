#ifndef _SYSTEM_H_
#define _SYSTEM_H_

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

	friend void interrupt timer(...);

private:
	static volatile long long ticks_since_boot;
};

#endif // _SYSTEM_H_
