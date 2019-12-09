#ifndef gew_h
#define gew_h
#include "generic.h"

int init_Gewichte();

int get_weights(int neededWeights, int myTakenWeights[gewichteAnzahl], int* addiertesGewicht, int tempWeights[gewichteAnzahl]);

int put_weights(int myweights[gewichteAnzahl]);
    
int display_leftover();

int display_person(int myweights[gewichteAnzahl]);

int destroy_gewichte();


#endif