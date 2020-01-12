#include "caesar.h"
#include <stdio.h>



int main(void){
    int buffersize = 90;
    char* input = "Hello, my name is x";
    char output[buffersize];
    for(int i = 0; i < buffersize; i++){
        output[i] = 0;
    }
    encode(input, output, buffersize, 5);
    printf("%s\n",output);
    decode(output, output, buffersize, 5);
    printf("%s\n",output);
    return 0;
}



static void encode(char *input, char *output, int buffersize, int shiftNum){
    int inputSize = get_string_size(input);
    if(inputSize<buffersize){
        buffersize = inputSize;
    }
    for(int i = 0; i < buffersize; i++){
        if(i == buffersize-1){
            output[i] = '\0';
        }
        else{
            char c = input[i];
            shift_char(&c,shiftNum);
            output[i] = c;
        } 
    }
}

static void decode(char *input, char *output, int buffersize, int shiftNum){
    int inputSize = get_string_size(input);
    if(inputSize<buffersize){
        buffersize = inputSize;
    }
    for(int i = 0; i < buffersize; i++){
        if(i == buffersize-1){
            output[i] = '\0';
        }
        else{
            char c = input[i];
            unshift_char(&c,shiftNum);
            output[i] = c;
        } 
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

static int get_string_size(char* string){
    for(int i = 0; i < __UINT32_MAX__;i++){
        if(string[i] == '\0'){
            return i+1;
        }
    }
    return 0;
}

