#include <iostream.h>
#include "queue.h"

void test_queue() {
	Queue<int> queue;

	cout << "empty: " << queue.empty() << "\nsize: " << queue.size() << "\n";

	cout << "Pushed:";
	for (auto i = 0; i < 10; i++) {
		queue.push(i);
		cout << " " << i;
	}

	cout << "\nFront/pop:";
	for (i = 0; i < 5; i++) {
		cout << " " << queue.front();
		queue.pop();
	}

	cout << "\nempty: " << queue.empty() << "\nsize: " << queue.size();

	cout << "\nBack: " << queue.back() << "\n";
}
