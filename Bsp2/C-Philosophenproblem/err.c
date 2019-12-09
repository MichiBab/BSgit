#include "err.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
int errno = 0;

int err(int Error_Numb, char *Message, int fatal_error){
    if(Error_Numb != 0 && fatal_error){
        printf("\n\n\nFATAL ERROR: %s\n\n\n",Message);
        exit(-1);
        }
    switch(Error_Numb){
            
            case NO_ERROR:
            
            return 0;
    }
    //Error, aber muss nicht explizit behandelt werden
    printf("%s\n",Message);//ErrorMessage
    return 0;
    }