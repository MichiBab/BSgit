#include "philothread.h"
#include "gewichte.h"
//constructor
philothread::philothread(int weight,int idnum)
{
    selfptr = this;
    this->myID = idnum;
    this->myNeededWeights = weight;
    for(int i = 0; i<gewichteAnzahl;i++){
        myTakenWeights[i]=0;
        }
    
}

void philothread::start(){
    p_thread = std::thread(&routineTEST,selfptr); //<-(function,this)
    }

//destructor
philothread::~philothread()
{
    p_thread.join();
}

int philothread::lock_my_mutex(){
    
    }
        
int philothread::unlock_my_mutex(){
    
    }
        
int philothread::change_block_flag(){
    
    }
        
int philothread::change_proceed_flag(){
    
    }
    
bool philothread::return_Blocked(){
    
    }
    
bool philothread::return_Proceed(){
    
    }
    
