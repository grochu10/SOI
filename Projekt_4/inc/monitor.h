#ifndef MONITOR_H
#define MONITOR_H


class Monitor
{
public:
    int fullId;
	int emptyId;
	int mutexId;
    Monitor(int q_id);
    void enter();
    void leave();
    void wait_full();
    void signal_full();
    void wait_empty();
    void signal_empty();
};

#endif