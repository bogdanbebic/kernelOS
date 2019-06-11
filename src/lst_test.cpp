#include <iostream.h>
#include "list.h"

 
void test_empty() {
    ForwardList<int> numbers;
    cout << "Initially, numbers.empty(): " << numbers.empty() << '\n';
 
    numbers.push_front(42);
    numbers.push_front(13317); 
    cout << "After adding elements, numbers.empty(): " << numbers.empty() << '\n';

    /* 
     * Expected output:
     * Initially, numbers.empty(): 1
	 * After adding elements, numbers.empty(): 0
     */
}

void test_size() {
	ForwardList<char> chars;
	chars.push_back('a');
	chars.push_back('b');
	chars.push_back('c');
	chars.push_back('d');
	cout << "Size: " << chars.size() << "\n";
}

void test_front() {
	ForwardList<char> chars;
	chars.push_back('a');
	chars.push_back('b');
	chars.push_back('c');
	chars.push_back('d');
	cout << "Front: " << chars.front() << "\n";

	/* 
     * Expected output:
     * Front: a
     */
}

void test_back() {
	ForwardList<char> chars;
	chars.push_back('a');
	chars.push_back('b');
	chars.push_back('c');
	chars.push_back('d');
	cout << "Back: " << chars.back() << "\n";

	/* 
     * Expected output:
     * Back: d
     */
}

void test_iterators_push_back() {
	ForwardList<int> list;

	for (auto i = 0; i < 6; i++) {
		list.push_back(i);
	}

	for (ForwardList<int>::Iterator it = list.begin(); it != list.end(); ++it) {
		cout << "elem = " << *it << "\n";
	}

	/* 
     * Expected output:
     * elem = 0
     * elem = 1
     * elem = 2
     * elem = 3
     * elem = 4
     * elem = 5
     */
}

void test_iterators_push_front() {
	ForwardList<long> list;

	for (auto i = 0L; i < 6L; i++) {
		list.push_front(i);
	}


	for (ForwardList<long>::Iterator it = list.begin(); it != list.end(); ++it) {
		cout << "elem = " << *it << "\n";
	}

	/* 
     * Expected output:
     * elem = 5
     * elem = 4
     * elem = 3
     * elem = 2
     * elem = 1
     * elem = 0
     */
}

void test_pop_front() {
	ForwardList<long> list;

	for (auto i = 0L; i < 6L; i++) {
		list.push_front(i);
	}

	cout << "Before pop front:";
	for (ForwardList<long>::Iterator it = list.begin(); it != list.end(); ++it) { cout << " " << *it; }

	list.pop_front();

	cout << "\nAfter pop front:";
	for (it = list.begin(); it != list.end(); ++it) { cout << " " << *it; }

	cout << "\n";
}

void test_clear() {
	ForwardList<long> list;

	for (auto i = 0L; i < 6L; i++) {
		list.push_front(i);
	}

	cout << "Before clear:";
	for (ForwardList<long>::Iterator it = list.begin(); it != list.end(); ++it) { cout << " " << *it; }
	cout << "\nEmpty: " << list.empty() << "\n";

	list.clear();
	cout << "After clear:";
	for (it = list.begin(); it != list.end(); ++it) { cout << " " << *it; }
	cout << "\nEmpty: " << list.empty() << "\n";
}

void test_erase_after() {
	ForwardList<int> list;

	for (auto i = 0; i < 6; i++) {
		list.push_back(i);
	}

	cout << "Before erase after:";
	for (ForwardList<int>::Iterator it = list.begin(); it != list.end(); ++it) { cout << " " << *it; }

	list.erase_after(list.begin());

	cout << "\nAfter erase after:";
	for (it = list.begin(); it != list.end(); ++it) { cout << " " << *it; }

	list.erase_after(list.begin()++);

	list.erase_after(list.end());
	
	cout << "\nAfter erase after:";
	for (it = list.begin(); it != list.end(); ++it) { cout << " " << *it; }

	cout << "\n";
}

void list_test() {
	cout << "Hello, World!\n";
	
	cout << "Test empty:\n";
	test_empty();

	cout << "Test size:\n";
	test_size();
	
	cout << "Test front:\n";
	test_front();

	cout << "Test back:\n";
	test_back();

	cout << "Test iterators push back:\n";
	test_iterators_push_back();

	cout << "Test iterators push front:\n";
	test_iterators_push_front();

	cout << "Test pop front:\n";
	test_pop_front();

	cout << "Test clear:\n";
	test_clear();

	cout << "Test erase after:\n";
	test_erase_after();
}
