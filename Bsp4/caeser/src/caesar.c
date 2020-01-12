#include "caesar.h"
#include <stdio.h>



int main(void){

    char* input = "Hallo; mein Name ist M !! :) ";
    char output[10];
    encode(input, output, sizeof(output), 5);
    printf("%s\n",output);
    decode(output, output, sizeof(output), 5);
    printf("%s\n",output);
    return 0;
}


static void encode(char *input, char *output, int outputSize, int shiftNum){
    if(sizeof(input)<sizeof(output)){
        outputSize = sizeof(input);
    }
    for(int i = 0; i < outputSize; i++){
        char c = input[i];
        shift_char(&c,shiftNum);
        output[i] = c;
    }
}

static void decode(char *input, char *output, int outputSize, int shiftNum){
    if(sizeof(input)<sizeof(output)){
        outputSize = sizeof(input);
    }
    for(int i = 0; i < outputSize; i++){
        char c = input[i];
        unshift_char(&c,shiftNum);
        output[i] = c;
    }
}

static int unshift_char(char* c, int shiftNum){
    //return if not in a-z or A-Z
    if(!is_ascii(*c)){
        return 0;
    }
    for(int i = 0; i < shiftNum; i++){
        switch (*c){
            case 'a':
                *c = 'z';
                break;
            case 'A':
                *c = 'Z';
                break;
            default:
                *c -= 1;
                break;
        }   
    }
    return 0;
}

static int shift_char(char* c, int shiftNum){
    //return if not in a-z or A-Z
    if(!is_ascii(*c)){
        return 0;
    }
    for(int i = 0; i < shiftNum; i++){
        switch (*c){
            case 'z':
                *c = 'a';
                break;
            case 'Z':
                *c = 'A';
                break;
            default:
                *c += 1;
                break;
        }   
    }
    return 0;
}

//returns 1 if is ascii, else 0
static int is_ascii(char c){
    if(c >= 'a' && c <= 'z'){
        return 1;
    }
    if(c >= 'A' && c <= 'Z'){
        return 1;
    }
    return 0;
}

