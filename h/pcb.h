#ifndef _PCB_H_
#define _PCB_H_

class Thread;

class PCB { // TODO: implement
public:
    enum State { NEW, READY, RUNNING, BLOCKED, COMPLETED };

    void start();

    void waitToComplete();

protected:

private:

};

#endif
