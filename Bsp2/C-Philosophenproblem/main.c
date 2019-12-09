#include <stdio.h>
#include "generic.h"
#include "gewichte.h"
#include "routine.h"
#include "controller.h"
#include "err.h"
//int threadArgInit(struct philothread *philo, int neededWeights, int myID);

struct philothread Anna;
pthread_t annaThread;
sem_t annaLock;

struct philothread Bernd;
pthread_t berndThread;
sem_t berndLock;

struct philothread Clara;
pthread_t claraThread;
sem_t claraLock;

struct philothread Dirk;
pthread_t dirkThread;
sem_t dirkLock;

struct philothread Emma;
pthread_t emmaThread;
sem_t emmaLock;

struct philothread* philoSammlung[anzahlPhilosophen];

pthread_mutex_t displaymutex = PTHREAD_MUTEX_INITIALIZER;

char philothreadArguments[anzahlPhilosophen] = {'n','n','n','n','n'};

/*struct philothread{
    pthread_t *thread;
    sem_t myLock;
    void*(*function)(void*); ;//functionpointer 
    int neededWeight;
    int myID; //ID fuere Global Array    int myNeededWeights
    int block_flag;
    int procceed_flag;
    int myTakenWeights[gewichteAnzahl];
    };*/
static int threadArgInit(struct philothread *philo, int neededWeights, int myID, sem_t* lock){
    philo->neededWeight = neededWeights;
    err(sem_init(lock,0,1),"fehler bei seminit\n",FATAL_ERROR); //binary
    philo->myLock = lock;
    philo->myID = myID;
    philo->block_flag = false;
    philo->procceed_flag = false;
    //globalArray[BEZEICHNER] = philo;
    for(int i = 0; i<gewichteAnzahl;i++){
        philo->myTakenWeights[i]=0;
        philo->tempWeights[i]=0;
        }
    philo->addiertesGewicht=0;
    philoSammlung[myID] = philo;
    return 0;

    }
    
static void cleanUpMutex(void* arg){
    pthread_mutex_unlock(&displaymutex);
    }
    
int display_all(){
    pthread_mutex_lock(&displaymutex);
    pthread_cleanup_push(cleanUpMutex,NULL);
    pthread_testcancel();
    int a = 0;
    for(int i = 0; i < anzahlPhilosophen; i++){
        printf("%d",philoSammlung[i]->myID);
        printf("(%d)",philoSammlung[i]->neededWeight);
        printf("-%c-", philothreadArguments[philoSammlung[i]->myID]);
        printf("%c:", philoSammlung[i]->myRoutine);
        a += display_person(philoSammlung[i]->myTakenWeights);
        }
    
    int b = display_leftover();
    if(a+b!=45){
        printf("\n-----------------ERROR-----------------\n");
        } 
    pthread_cleanup_pop(1);//remove cleanup handler
    //pthread_mutex_unlock(&displaymutex);
    
    return 0;
    }
    
int deconstructor(){
    destroy_gewichte();
    err(pthread_mutex_destroy(&displaymutex), "fehler bei mutexDestroyGewichte3\n",false);
    
    for(int i = 0; i<anzahlPhilosophen;i++){
        err(sem_destroy(philoSammlung[i]->myLock), "fehler bei semdestroy\n",false);
        }
    
    return 0;
    }


static int init_Philothreads(){
    
    threadArgInit(&Anna,6,0,&annaLock);
    threadArgInit(&Bernd,8,1,&berndLock);
    threadArgInit(&Clara,12,2,&claraLock);
    threadArgInit(&Dirk,32,3,&dirkLock);
    threadArgInit(&Emma,14,4,&emmaLock);
    
    err(pthread_create((pthread_t*)&annaThread,NULL,routine,(struct philothread*)&Anna), "pthread create error",NOT_FATAL);
 
    err(pthread_create((pthread_t*)&berndThread,NULL,routine,(struct philothread*)&Bernd), "pthread create error",NOT_FATAL);
        
    err(pthread_create((pthread_t*)&claraThread,NULL,routine,(struct philothread*)&Clara), "pthread create error",NOT_FATAL);
        
    err(pthread_create((pthread_t*)&dirkThread,NULL,routine,(struct philothread*)&Dirk), "pthread create error",NOT_FATAL);
        
    err(pthread_create((pthread_t*)&emmaThread,NULL,routine,(struct philothread*)&Emma), "pthread create error",NOT_FATAL);
    
    return 0;
    }
    
int unlockThread(int index){
    sem_post(philoSammlung[index]->myLock);
    return 0;
    }
    
int main(int argc, char **argv){
    init_Gewichte();
    init_Philothreads();
    
    controller(&philoSammlung[anzahlPhilosophen]);
    
    pthread_cancel(annaThread);
    pthread_cancel(berndThread);
    pthread_cancel(claraThread);
    pthread_cancel(dirkThread);
    pthread_cancel(emmaThread);
    
    pthread_join(annaThread,0);
    pthread_join(berndThread,0);
    pthread_join(claraThread,0);
    pthread_join(dirkThread,0);
    pthread_join(emmaThread,0);
    
    deconstructor();
    }
    
    