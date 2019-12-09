#ifndef err_h
#define err_h

#define FATAL_ERROR 100
#define NOT_FATAL 1
#define NO_ERROR 0
int err(int ERROR_NUMBER, char* ErrorMessage, int fatal);
extern int errno; //kein errno nennen, ist global definiert in c
//makro nutzen
#endif
