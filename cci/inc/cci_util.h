#ifndef _CCI_UTIL_H
#define _CCI_UTIL_H

#include "rtx.h"

//gets and prints process statuses
int CCI_printProcessStatuses (char* data);

//sets the clock 
int CCI_setClock (char* timeParam, uint32_t* time);

//prints trace buffers on console given the envelope message data
int CCI_printTraceBuffers (char* data);

//set process priority based on params given provided
int CCI_setNewPriority (char* param);

//strcmp
int CCI_strcmp (char* str1, char* str2);

//converts string into int
int CCI_atoi (char* str);

//Splits input into the first word (retStr1) and the remainder (retStr2)
int splitFirstWord (char* input, char* retStr1, char* retStr2);

#endif
