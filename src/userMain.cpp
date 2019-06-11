#include <iostream.h>
#include "thread.h"

#include "System.h"

void tick() {
	// empty body, will be defined in public tests
}

// ----- MARTIN TEST -----
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

int syncPrintf(const char *format, ...) {
	int res = 0;
	va_list args;
	lock;
	va_start(args, format);
	res += vprintf(format, args);
	va_end(args);
	unlock;
	return res;
}

class TestThread : public Thread {
public:
	TestThread(int id): Thread(4096, 2), myID(id) {};
	~TestThread() {	waitToComplete(); }
protected:
	int myID;
	void run();
};

class WaitThreadMarta : public Thread {
public:
	WaitThreadMarta(TestThread *t1, TestThread *t2)
		: Thread()
	{
		t1_ = t1;
		t2_ = t2;
	}

	~WaitThreadMarta() { waitToComplete(); }

protected:
	void run() {
		syncPrintf("Starting tests...\n");
		t1_->waitToComplete();
		syncPrintf("Test 1 completed!\n");
		t2_->waitToComplete();
		syncPrintf("Test 2 completed!\n");
	}

private:
	TestThread *t1_,*t2_;
};

void TestThread::run() {
	syncPrintf("Thread %d adresa:%p: loop1 starts\n", getId(), this);

	for(long i=0;i<8000;i++) { for (long j = 0; j < 32000; j++); }
	//Thread::sleep(myID*20);

	syncPrintf("Thread %d: loop1 ends, dispatch\n", myID);

	dispatch();

	syncPrintf("Thread %d: loop2 starts\n", getId());

	for (int k = 0; k < 8000; k++) { for (long j = 0; j < 32000; j++); }
	//Thread::sleep(myID*20);

	syncPrintf("Thread %d: loop2 ends\n", getId());
}

int test_marta(int argc, char** argv) {
	cout << "argc: " << argc << "\nargv:\n";
	for (auto i = 0; i < argc; i++) {
		cout << argv[i] << "\n";
	}

	syncPrintf("User main starts\n");
	TestThread *t1 = new TestThread(1),
			   *t2 = new TestThread(2),
			   *t3 = new TestThread(3);

	WaitThreadMarta *w = new WaitThreadMarta(t1,t2);
	t1->start();
	t2->start();
	t3->start();
	w->start();

	for (long k=0; k<5; k++) {
		for(long i=0;i<8000;i++) { for (long j = 0; j < 32000; j++); }
		//Thread::sleep(10);

		syncPrintf("Usermain %d running!\n", Thread::getRunningId());
		//Thread *t = Thread::getThreadById(3);
		//syncPrintf("%p\n", t);
		//dispatch();
	}

	delete t1;
	delete t2;
	delete t3;
	delete w;

	syncPrintf("User main ends\n");
	return 0;
}

// !----- MARTIN TEST KRAJ -----

int test_output_sizeof(int argc, char *argv[]) {
	cout << "argc: " << argc << "\nargv:\n";
	for (auto i = 0; i < argc; i++) {
		cout << argv[i] << "\n";
	}

	cout << "sizeof short: " << sizeof(short) << "\n";
	cout << "sizeof int: " << sizeof(int) << "\n";
	cout << "sizeof long: " << sizeof(long) << "\n";
	cout << "sizeof long long: " << sizeof(long long) << "\n";
	cout << "sizeof ushort: " << sizeof(unsigned short) << "\n";
	cout << "sizeof uint: " << sizeof(unsigned int) << "\n";
	cout << "sizeof ulong: " << sizeof(unsigned long) << "\n";
	cout << "sizeof u long long: " << sizeof(unsigned long long) << "\n";
	return 0;
}

int test_hello_world(int argc, char *argv[]) {
	cout << "argc: " << argc << "\nargv:\n";
	for (auto i = 0; i < argc; i++) {
		cout << argv[i] << "\n";
	}

	cout << "Hello, World!\n";
	return 0;
}

int test_one_thread_timer_interrupt(int argc, char *argv[]) {
	cout << "argc: " << argc << "\nargv:\n";
	for (auto i = 0; i < argc; i++) {
		cout << argv[i] << "\n";
	}

	for (volatile long j = 0; j < 3000L; j++) {
		for (volatile long k = 0; k < 30000L; k++) {

		}

	}

	return 0;
}

class ThreadA : public Thread {
public:
	ThreadA(unsigned time_slice = 4U) : Thread(defaultStackSize, time_slice) {}
	void run();
	~ThreadA() { waitToComplete(); }
};

void ThreadA::run() {
	for (int i =0; i < 10; ++i) {
		lock;
		cout<<"u a() i = "<<i<<endl;
		unlock;
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}

}

class WaitThread : public Thread {
public:
	WaitThread(unsigned time_slice = 4U) : Thread(defaultStackSize, time_slice) {}
	void run();
	~WaitThread() { waitToComplete(); }
};

void WaitThread::run() {
	ThreadA a;
	a.start();

	for (int i =0; i < 10; ++i) {
		lock;
		cout<<"u wait() i = "<<i<<endl;
		unlock;
		if (i == 3 || i == 7) {
			lock;
			cout<<"treba sad da wait-uje na a()"<<endl;
			unlock;
			a.waitToComplete();
		}

		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
}

void two_threads_test() {
	for (int j = 0; j < 10; j++) {
			lock;
			cout<<"u main() i = "<<j<<endl;
			unlock;

		for (int k = 0; k < 10000; ++k) 
			for (int l = 0; l < 30000; ++l);

	}
}

int test_two_threads(int argc, char *argv[]) {
	cout << "argc: " << argc << "\nargv:\n";
	for (auto i = 0; i < argc; i++) {
		cout << argv[i] << "\n";
	}

	ThreadA thread_a;
	thread_a.start();

	two_threads_test();

	return 0;
}

int test_waiting_main(int argc, char *argv[]) {
	cout << "argc: " << argc << "\nargv:\n";
	for (auto i = 0; i < argc; i++) {
		cout << argv[i] << "\n";
	}

	ThreadA thread_a;
	thread_a.start();

	for (int j = 0; j < 10; j++) {
		lock;
		cout<<"u main() i = "<<j<<endl;
		unlock;
		if (j == 4) {
			lock;
			cout<<"treba sad da wait-uje"<<endl;
			unlock;
			thread_a.waitToComplete();
		}

		for (int k = 0; k < 10000; ++k) 
			for (int l = 0; l < 30000; ++l);

	}

	return 0;
}

int test_waiting_threads(int argc, char *argv[]) {
	cout << "argc: " << argc << "\nargv:\n";
	for (auto i = 0; i < argc; i++) {
		cout << argv[i] << "\n";
	}

	WaitThread wait_thread;
	wait_thread.start();

	for (int j = 0; j < 10; j++) {
		lock;
		cout<<"u main() i = "<<j<<endl;
		unlock;
		for (int k = 0; k < 10000; ++k) 
			for (int l = 0; l < 30000; ++l);

	}

	return 0;
}

#define str(x) #x

#define RUN_TEST(test_no) \
	{\
		cout << "*** " << str(test##test_no) << " ***\n";\
		int ret = test##test_no(argc, argv);\
		if (ret != 0) { return ret; }\
	}\
	cout << "*** " << str(test##test_no) << " *** finished successfully\n"\
\


void list_test();
void test_queue();

int userMain(int argc, char *argv[]) {
	list_test();
	test_queue();
	RUN_TEST(_output_sizeof);
	RUN_TEST(_hello_world);
	RUN_TEST(_one_thread_timer_interrupt);
	lock;
	cout << "Threads tests\n";
	unlock;
	RUN_TEST(_two_threads);
	RUN_TEST(_waiting_main);
	RUN_TEST(_waiting_threads);
	RUN_TEST(_marta);
	return 0;
}


