#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define DATABLOCK_SIZE 1024
#define DATABLOCK_CHARSIZE SIZETOCHAR(DATABLOCK_SIZE)
#define INODE_SIZE 512
#define INODE_CHARSIZE SIZETOCHAR(INODE_SIZE)
#define DATABLOCK_BITE 128
#define MAX_INDIRECT 102

typedef struct FileSystem_ FileSystem;	// ���� �ý��� 
typedef struct Inode_ Inode;			// ���̳�� 
typedef struct DataBlock_ DataBlock;	
typedef struct SuperBlock_ SuperBlock;
typedef struct Time_ Time;
typedef struct DataBlockNode_ DataBlockNode;
typedef struct DataBlockList_ DataBlockList;
typedef struct FileSystem_
{
	unsigned int bootBlock : 16;
	DataBlock * dataBlock[DATABLOCK_SIZE];
	Inode * inode[INODE_SIZE];
	SuperBlock * superBlock;
}FileSystem;
typedef struct Inode_
{
	int num;
	bool type;
	Time *lastUsedTime;
	int size;
	DataBlockList * data;
}Inode;
typedef struct DataBlock_
{
	int plus;
	int num;
	char data[DATABLOCK_BITE];	
}DataBlock;
typedef struct SuperBlock_
{
	char inodeCheck[INODE_CHARSIZE];
	char dataBlockCheck[DATABLOCK_CHARSIZE];
}SuperBlock;
typedef struct DataBlockNode_
{
	DataBlock * value;
	struct DataBlockNode_ * next;
	struct DataBlockNode_ * prev;
}DataBlockNode;
typedef struct DataBlockList_
{
	int size;
	DataBlockNode * head;
	DataBlockNode * tail;
}DataBlockList;
typedef struct Time_
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int sec;	
}Time;

void initFileSystem(FileSystem ** address);
void initDataBlock(DataBlock ** address);
void initInode(Inode ** address);
void initSuperBlock(SuperBlock ** address);

void addDataBlockList(Inode * inode, DataBlock * data);
DataBlock * getEmptyDataBlock(FileSystem * fileSystem);	//��� �ִ� ������ �� ���
Inode * getEmptyInode(FileSystem * fileSystem);	//����մ� ���̳�� ������


void initDataBlockList(DataBlockList ** address);
//������ ����Ʈ�� ������ ��� �ִ� �Լ� 
void pushDataBlockList(DataBlockList * list, DataBlock * dataBlock);
// �̱� �δ��̷�Ʈ �� ���� 
void makeSingleIndirect(FileSystem * fileSystem , Inode * inode);
// ���� �δ��̷�Ʈ �� ���� 
void makeDoubleIndirect(FileSystem * fileSystem , Inode * inode);
// �δ��̷�Ʈ ������ ������ �����ͺ� ������ 
DataBlock * getDataBlockIndirect(FileSystem * fileSystem , DataBlock * dataBlock , int n);
// �δ��̷�Ʈ ������ ���� ��� ��ȣ �����ϱ� 
void setDataBlockIndirect(DataBlock * dataBlock , int n , int num);
//���̳�忡 �����ͺ� �����ϱ� 
void pushDataBlockInode(FileSystem * fileSystem ,Inode * inode , DataBlock *dataBlock);
// ���̳�� ���� n��° ������ ������ 
DataBlock * getDataBlockInode(FileSystem * fileSystem , Inode * inode , int n);
char * getDataInode(FileSystem * fileSystem , Inode * inode , int fromBit,  int nBit);	//fromBit ���� nBit��ŭ�� ������ ��� 

void setDataInode(FileSystem * fileSystem, Inode * inode, int fromBit, int nBit, int fromSourceBit ,void * source);

#endif
