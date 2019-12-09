#include "controller.h"
#include <string.h>
#define buffersize 3
//we only want 2 byte to be read
char buffer[buffersize];

int controller(struct philothread* philoSammlung[anzahlPhilosophen]){

    while(1){
        fgets(buffer,buffersize+1,stdin);
        
        //BLOCK
        if(strcmp(buffer,"0b\n") == 0){
            philothreadArguments[0]='b';
            }
        if(strcmp(buffer,"1b\n") == 0){
            philothreadArguments[1]='b';
            }
        if(strcmp(buffer,"2b\n") == 0){
            philothreadArguments[2]='b';
            }
        if(strcmp(buffer,"3b\n") == 0){
            philothreadArguments[3]='b';
            }
        if(strcmp(buffer,"4b\n") == 0){
            philothreadArguments[4]='b';
            }
            
            //UNBLOCK
        if(strcmp(buffer,"0u\n") == 0){
            philothreadArguments[0]='n';
            unlockThread(0);
            }
        if(strcmp(buffer,"1u\n") == 0){
            philothreadArguments[1]='n';
            unlockThread(1);
            }
        if(strcmp(buffer,"2u\n") == 0){
            philothreadArguments[2]='n';
            unlockThread(2);
            }
        if(strcmp(buffer,"3u\n") == 0){
            philothreadArguments[3]='n';
            unlockThread(3);
            }
        if(strcmp(buffer,"4u\n") == 0){
            philothreadArguments[4]='n';
            unlockThread(4);
            }
           
        //procceed 
        if(strcmp(buffer,"0p\n") == 0){
            philothreadArguments[0]='p';
            }
        if(strcmp(buffer,"1p\n") == 0){
            philothreadArguments[1]='p';
            }
        if(strcmp(buffer,"2p\n") == 0){
            philothreadArguments[2]='p';
            }
        if(strcmp(buffer,"3p\n") == 0){
            philothreadArguments[3]='p';
            }
        if(strcmp(buffer,"4p\n") == 0){
            philothreadArguments[4]='p';
            }
      
        if(strcmp(buffer,"q\n") == 0 || strcmp(buffer,"Q\n") == 0){
            for(int i = 0; i< anzahlPhilosophen; i++){
                philothreadArguments[i]='q';
                }
            printf("\n\nINITIATING END OF PROGRAM \n");
            return 0;
            
            
            }
    
        buffer[0] = 0;
        buffer[1] = 0;
        buffer[2] = 0;

    }
}