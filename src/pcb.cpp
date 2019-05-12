#include "pcb.h"

#include "DOS.H"
#include "SCHEDULE.H"

long long PCB::next_id = 0;

PCB::PCB(StackSize stack_size, Time time_slice, Thread *my_thread) 
    : my_thread_(my_thread)
    , id_(next_id++)  // TODO: change to pcb_vector last index (atomic)
    , stack_size_(stack_size)
    , state_(NEW)
    , time_slice_(time_slice)
{
    this->stack_ = new Word[this->stack_size_];
    this->stack_[this->stack_size_ - 1] = 0x200; // I bit in psw set
    this->stack_[this->stack_size_ - 2] = FP_SEG(PCB::wrapper);
	this->stack_[this->stack_size_ - 3] = FP_OFF(PCB::wrapper);

    this->stack_segment_ = FP_SEG(this->stack_ + this->stack_size_ - 12);
	this->stack_offset_ = FP_OFF(this->stack_ + this->stack_size_ - 12);
	this->base_pointer_ = FP_OFF(this->stack_ + this->stack_size_ - 12);
}

PCB::~PCB() {
    delete [] this->stack_;
}

void PCB::start() {
    this->state_ = READY;
    Scheduler::put(this);
}

ID PCB::id() const {
    return this->id_;
}

void PCB::wrapper() {

}
