#ifndef gew
#define gew
#include "philothread.h"
#include "generic.h"
int init_Gewichte();
int get_weights(int neededWeights, int myTakenWeights[gewichteAnzahl]);
int display_Gewichtsstatus();
int put_weights(int myweights[gewichteAnzahl]);

int routineTEST(philothread* p);
#endif
