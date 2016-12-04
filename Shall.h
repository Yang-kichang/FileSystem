#ifndef SHALL_H
#define SHALL_H

#include <stdio.h>
#include <stdlib.h>
#include "Util.h"
#include "FileSystem.h"
typedef struct Shall_ Shall;
typedef struct DTree_ DTree;	//Directory Tree
typedef struct DNode_ DNode;//Directory Node - Tree


typedef struct Shall_
{
	char pwd[1000];	// �ڱ� ��ġ ���
	Inode * root;	//�ֻ��� ���丮 
	DTree * dTree;
	FileSystem * fileSystem;	//���� �ý��� 
}Shall;
typedef struct DTree_	//Directory Tree
{
	char fileName[5];
	int value;
	DNode * childHead;	//List
}DTree;  
typedef struct DNode_	//Directory Node - Tree
{
	DTree * value;
	struct DNode_ * next;
}DNode;


void initDTree(DTree ** address , Inode * inode , char * fileName);	// Init DirectoryTree
void pushDTree(DTree * dTree , Inode * inode , char * fileName);	//���̷�Ʈ Ʈ���� ���ο� ��� �߰� 
void initShall(Shall ** address, FileSystem * fileSystem);	//�� �ʱ�ȭ 
DTree * findDTreeNum(DTree * root , int num);	// inodeNum ���� dTree ã�� 
DTree * findDTreePath(DTree * root ,char * path);	//��η� dTree ã�� 
//���丮�� ���� �ֱ� 
void pushFileDirectory(Shall *shall , Inode * directory , Inode * file , char * fileName);
Inode * getFileDirectory(Shall *shall , Inode * directory, char * findFileName);	//NULL �� ��ȯ�ϸ� ���°Ŵ� �˰ٳ�? ;


void mymkfs(Shall ** address);
void myls(Shall * shall, char * path , char * option);
//SAFE
void printPWD(Shall * shall);
void runCommand(Shall ** address);	//��ɾ� �о���� 


#endif
