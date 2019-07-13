#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "list.h"

#include "compares.h"	// UPDATE: OVO JE DODATO

template <class T>
class Queue : private ForwardList<T> {
public:
	typedef unsigned size_type;

	// Element access

	const T & front() const {
		return ForwardList<T>::front();
	}

	const T & back() const {
		return ForwardList<T>::back();
	}

	// Capacity

	bool empty() const {
		return ForwardList<T>::empty();
	}

	size_type size() const {
		return ForwardList<T>::size();
	}

	// Modifiers

	void push(const T &value) {
		ForwardList<T>::push_back(value);
	}

	void pop() {
		ForwardList<T>::pop_front();
	}

	void clear() {
		ForwardList<T>::clear();
	}

	// OVO JE DODATO
	void swap(const T &value1, const T &value2) {
		ForwardList<T>::swap(value1, value2);
	}

};

// UPDATE: OVO JE DODATO
template<class T, class Compare>
class PriorityQueue : private ForwardList<T> {
public:
	typedef unsigned size_type;

	// Member functions

	PriorityQueue() : compare_() {}

	// Element access

	const T & top() {
		return ForwardList<T>::front();
	}

	// Capacity

	bool empty() const {
		return ForwardList<T>::empty();
	}

	size_type size() const {
		return ForwardList<T>::size();
	}

	// Modifiers

	void push(const T &value);

	void pop() {
		ForwardList<T>::pop_front();
	}

private:
	Compare compare_;
};

template<class T, class Compare>
void PriorityQueue<T, Compare>::push(const T &value) {
	if  (this->empty()) {
		this->push_back(value);
		return;
	}

	ForwardList<T>::Iterator it = this->begin();
	ForwardList<T>::Iterator prev = this->end();
	while (it != this->end() && !this->compare_(value, *it)) {
		prev = it;
		++it;
	}

	if (prev != this->end()) {
		this->insert_after(prev, value);
	}
	else {
		this->push_front(value);
	}

}

#endif // ! _QUEUE_H_
