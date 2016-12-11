#ifndef UIIL_H
#define UTIL_H
#include <string.h>

#define SIZETOCHAR(X) ((((X) - 1) / 8) + 1) 

//SAFE
int getBit(void * buffer,int n);	//buf ���մ� n ��° ��Ʈ�� ���´� 
void setBit(void * buffer,int n,int value);	//buf �� �ִ� n ��° ��Ʈ�� value ���� 
int stringToInt(char * str);
void copyBit(void * targetVoid , void * sourceVoid,int fromTarget,int fromSource, int n);
void printBit(void * source , int from , int n);
//Buffer is Empty?
int isEmptyBuffer(void * buffer , int nBit);
#endif
