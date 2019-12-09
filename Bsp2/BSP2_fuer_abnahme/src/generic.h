#ifndef gen_h
#define gen_h
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#define gewichteAnzahl 3
#define gewichtsIndexing (gewichteAnzahl-1)
#define anzahlUndKilogram 2
#define anzahlindex 0
#define gewichtindex 1
#define anzahlPhilosophen 5
#define true 1
#define false 0
int unlockThread(int index);
struct philothread{
    sem_t* myLock;
    int neededWeight;
    int myID; //ID fuere Global Array    int myNeededWeights
    int myTakenWeights[gewichteAnzahl];
    int addiertesGewicht;
    char myRoutine;
    int tempWeights[gewichteAnzahl];
    };
int display_all();
extern char philothreadArguments[anzahlPhilosophen];
int deconstructor();

#endif
