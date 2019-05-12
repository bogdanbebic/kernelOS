#include <iostream.h>

void tick() {
	// empty body, will be defined in public tests
}

int test0(int argc, char *argv[]) {
	cout << "argc: " << argc << "\nargv:\n";
		for (auto i = 0; i < argc; i++) {
			cout << argv[i] << "\n";
		}

		cout << "Hello, World!\n";
		return 0;
}

int test1(int argc, char *argv[]) {
	cout << "argc: " << argc << "\nargv:\n";
		for (auto i = 0; i < argc; i++) {
			cout << argv[i] << "\n";
		}

		cout << "Hello, World!\n";
		for (volatile long j = 0; j < 30000L; j++) {
			for (volatile long k = 0; k < 30000L; k++) {

			}

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


int userMain(int argc, char *argv[]) {
	RUN_TEST(0);
	RUN_TEST(1);
	return 0;
}
