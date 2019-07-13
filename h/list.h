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

		// OVO JE DODATO
		T & operator * () {
			return this->elem_->data;
		}

		const T & operator * () const {
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
		return this->size_ != 0 ? Iterator(this->head_) : Iterator(0);
	}

	Iterator end() {
		return this->size_ != 0 ? Iterator(this->tail_->next) : Iterator(0);
	}

	// OVO JE DODATO
	Iterator find(const T &value) {
		for (Iterator it = this->begin(); it != this->end(); ++it) {
			if (*it == value) {
				return it;
			}

		}

		return this->end();
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

	void remove(const T &value);

	Iterator insert_after(Iterator position, const T &value) {
		if (position == this->end()) {
			return this->end();
		}

		this->size_++;
		ForwardListNode *new_node = new ForwardListNode(value);
		new_node->next = position.elem_->next;
		position.elem_->next = new_node;
		if (position.elem_ == this->tail_) {
			this->tail_ = new_node;
		}

		return Iterator(new_node);
	}

	Iterator erase_after(Iterator position) {
		if (position == this->end()) { return this->end(); }

		ForwardListNode *old = position.elem_->next;
		if (old != 0) {
			position.elem_->next = old->next;
			delete old;
			this->size_--;
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

	// OVO JE DODATO
	void swap(const T &value1, const T &value2) {
		Iterator it1 = this->find(value1), it2 = this->find(value2);
		if (it1 != this->end() && it2 != this->end()) {
			T temp = *it1;
			*it1 = *it2;
			*it2 = temp;
		}

	}

protected:
	size_type size_;

private:
	ForwardListNode *head_, *tail_;

};

/*
// OVO JE DODATO
template<class T>
typename ForwardList<T>::Iterator ForwardList<T>::find(const T &value) {
	for (Iterator it = this->begin(); it != this->end(); ++it) {
		if (*it == value) {
			return it;
		}

	}

	return this->end();
}*/

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

template<class T>
void ForwardList<T>::remove(const T &value) {
	Iterator it = this->begin();
	while (it != this->end() && *it == value) {
		this->pop_front();
		it = this->begin();
	} 

    if (this->size() == 0) {
        return;
    }

    Iterator next = ++this->begin();
	while (next != this->end()) {
		if (*next == value) {
			next = this->erase_after(it);
		}
		else {
			++it;
			++next;
		}
		
	}

}

#endif // ! _LIST_H_
