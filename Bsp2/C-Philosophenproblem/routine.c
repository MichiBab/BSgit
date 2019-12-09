#include "routine.h"
#include "gewichte.h"
#include "generic.h"

static int timeout(struct philothread *philo, long long int sleeptime){
   
    
    for(long long int i=0; i<sleeptime;i++){
        //sleep(1);
        if(philothreadArguments[philo->myID] == 'b'){
            sem_wait(philo->myLock);
            }
        if(philothreadArguments[philo->myID] == 'p'){
            philothreadArguments[philo->myID] = 'n';
            i=sleeptime;
            }
        
        }
   
   return 0;
    
    }


void* routine(void* arg){
    struct philothread *philo = (struct philothread*) arg;
    while(1){
    //rest
    philo->myRoutine = 'R';
    timeout(philo, REST_LOOP);
    display_all();
    
    philo->myRoutine = 'G';
    get_weights(philo->neededWeight,philo->myTakenWeights, &philo->addiertesGewicht, philo->tempWeights);
    display_all();
    
    philo->myRoutine = 'W';
    timeout(philo, WORKOUT_LOOP);
    display_all();
    
    philo->myRoutine = 'P';
    put_weights(philo->myTakenWeights);
    display_all();
    }
    return 0;
}

