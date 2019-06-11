#ifndef _LIST_H_
#define _LIST_H_

#include "bcconsts.h"

template<class T>
class ForwardList {
public:

	// Nested types

	typedef unsigned size_type;

	class ForwardListNode {
	public:
		ForwardListNode(T value) : data(value) {}
		T data;
		ForwardListNode *next;
	};
	
	class Iterator {
	public:
		Iterator(ForwardListNode *elem) : elem_(elem) {}

		const T & operator * () {
			return this->elem_->data;
		}

		bool operator != (const Iterator & other) {
			return this->elem_ != other.elem_;
		}

		bool operator == (const Iterator & other) {
			return !(*this != other);
		}

		Iterator operator ++ () {
			this->elem_ = this->elem_->next;
			return *this;
		}

		Iterator operator ++ (int) {
			this->elem_ = this->elem_->next;
			return *this;
		}

	private:
		friend class ForwardList<T>;
		ForwardListNode *elem_;		
	};

	// Member functions

	ForwardList() {
		this->head_ = this->tail_ = 0;
		this->size_ = 0;
	}

	~ForwardList() {
		this->clear();
	}

	// Element access

	const T & front() const {
		return this->head_->data;
	}

	const T & back() const {
		return this->tail_->data;
	}

	// Iterators	

	Iterator begin() {
		return Iterator(this->head_);
	}

	Iterator end() {
		return this->tail_ != 0 ? Iterator(this->tail_->next) : Iterator(0);
	}

	// Capacity

	bool empty() const {
		return this->head_ == 0;
	}

	size_type size() const {
		return this->size_;
	}

	// Modifiers

	void clear();

	Iterator erase_after(Iterator position) {
		if (position == this->end()) { return this->end(); }

		ForwardListNode *old = position.elem_->next;
		if (old != 0) {
			position.elem_->next = old->next;
			delete old;
		}

		return Iterator(position.elem_->next);
	}

	void push_front(const T &value) {
		ForwardListNode *new_node = new ForwardListNode(value);
		new_node->next = this->head_;
		if (this->empty()) {
			this->tail_ = new_node;
		}
		
		this->head_ = new_node;
		this->size_++;
	}

	void pop_front() {
		if (!this->empty()) {
			ForwardListNode *old = this->head_;
			this->head_ = this->head_->next;
			delete old;
			this->size_--;
		}

	}

	void push_back(const T &value) {
		ForwardListNode *new_node = new ForwardListNode(value);
		new_node->next = 0;
		if (this->empty()) {
			this->head_ = new_node;
		} else {
			this->tail_->next = new_node;
		}

		this->tail_ = new_node;
		this->size_++;
	}

private:
	ForwardListNode *head_, *tail_;
	size_type size_;

};

template<class T>
void ForwardList<T>::clear() {
	ForwardListNode *elem = this->head_;
	while (elem != 0) {
		this->head_ = elem;
		elem = elem->next;
		delete this->head_;
		this->size_ = 0;
	}

	this->head_ = this->tail_ = 0;
}

#endif // ! _LIST_H_
