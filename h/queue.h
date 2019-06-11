#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "list.h"

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

};

#endif // ! _QUEUE_H_
