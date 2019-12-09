#ifndef PHILOTHREAD_H
#define PHILOTHREAD_H
#include "generic.h"


class philothread
{
public:
    philothread* selfptr;
    philothread(int weight,int idnum);
    ~philothread();
    int lock_my_mutex();
    int unlock_my_mutex();
    int myID; //ID fuere Global Array
    int myNeededWeights;
    int change_block_flag();
    int change_proceed_flag();
    bool return_Blocked();
    bool return_Proceed();
    void start();
    int myTakenWeights[gewichteAnzahl];
    thread p_thread;
private:
    mutex myMutex;
    bool block_flag = false;
    bool procceed_flag = false;
    
};

#endif // PHILOTHREAD_H
