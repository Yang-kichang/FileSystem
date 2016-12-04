#include "Util.h"
int getBit(void * buffer,int n)	//buf ���մ� n ��° ��Ʈ�� ���´� 
{
	char *buf = (char*)buffer;
	int arrayNumber = n / 8;	// char buf �迭�� arrayNumber���� ��ġ 
	int rNumber = n % 8;		//	�������ǿ� �ִ� rNumber��° ��Ʈ 
	char mask;
	
	mask = (1 << (7 - rNumber));
	if(buf[arrayNumber] & mask)	// 0 ���� ũ�� 1 
		return 1;
	else			//�ƴϸ� 0 
		return 0;
}
void setBit(void * buffer,int n,int value)	//buf �� �ִ� n ��° ��Ʈ�� value ���� 
{
	
	char *buf = (char*)buffer;
	int arrayNumber = n / 8;	// char buf �迭�� arrayNumber���� ��ġ 
	int rNumber = n % 8;		//	�������ǿ� �ִ� rNumber��° ��Ʈ
	char mask;
	mask = 1 << (7 - rNumber);
	if(value == 1)
		buf[arrayNumber] |= mask;	// OR 
	else
		buf[arrayNumber] &= (~mask);	// NAND 
}
int stringToInt(char * str)	//���ڿ��� ���ڷ� �ٲ��ִ� �� 
{
	int len = strlen(str);
	int i  = 0;
	int ten = 1;
	int r = 0;
	for(i = len - 1; i >= 0; i--)
	{
		r += ten * (str[i] - '0');
		ten *= 10;
	}
	return r;
}
//source : �ҽ� target : Ÿ�� fromTarget : Ÿ�ٿ��� ���Ʈ ������ �κк��� ����
//fromSource : �ҽ����� ���Ʈ ������ �κк��� ���� n ���� 
void copyBit(void * targetVoid , void * sourceVoid,int fromTarget,int fromSource, int n)
{
	char * target = (char*)targetVoid;
	char * source = (char*)sourceVoid;
	int i;
	for(i = 0; i < n; i++)
	{
		setBit(target,fromTarget + i,getBit(source,i + fromSource));
	}
}
//Buffer is Empty?
int isEmptyBuffer(void * buffer , int nBit)
{
	int r = 1;
	int i;
	for(i = 0; i < nBit; i++)
		if(getBit(buffer , i) == 1)
		{
			r = 0;
			break;
		}
	return r;
}
void printBit(void * source , int from , int n)
{
	int i = 0;
	while(i < n)
	{
		printf("%d ",getBit(source,i++ + from));
		if(i % 8 == 0)
		{
			printf("\n");
		}
		
	}
}
