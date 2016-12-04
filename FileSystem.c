
#include "FileSystem.h"
void initFileSystem(FileSystem ** address)
{
	*address = (FileSystem*)malloc(sizeof(FileSystem));
	FileSystem * fileSystem = *address;
	int i;
	for(i = 0; i < DATABLOCK_SIZE; i++)	//Data Block Setting
	{
		initDataBlock(&(fileSystem -> dataBlock[i]));
		fileSystem -> dataBlock[i] -> num = i;
	}
	for(i = 0; i < INODE_SIZE; i++)	//Inode Setting
	{
		initInode(&(fileSystem -> inode[i]));
		fileSystem -> inode[i] -> num = i;
	}
	initSuperBlock(&(fileSystem -> superBlock));
}
void initDataBlock(DataBlock ** address)
{
	*address = (DataBlock*)malloc(sizeof(DataBlock));
	DataBlock * dataBlock = *address;
//	dataBlock -> type = 0;	//defalut = 0
	int i;
	for(i = 0; i < DATABLOCK_BITE; i++)
		dataBlock -> data[i] = 0;	//defalut = 0
}
void initInode(Inode ** address)
{
	*address = (Inode*)malloc(sizeof(Inode));
	Inode *inode = *address;
	inode -> type = 0;	//defalut = 0 is file
	inode -> lastUsedTime = NULL;
	inode -> size = 0;	//defalut = 0	
	int i;
	initDataBlockList(&(inode -> data));
}
void initSuperBlock(SuperBlock ** address)
{
	*address = (SuperBlock*)malloc(sizeof(SuperBlock));
	SuperBlock *superBlock = *address;
	int i = 0;
	for(i = 0; i < 	DATABLOCK_CHARSIZE; i++)
		superBlock -> dataBlockCheck[i] = 0;
	for(i = 0; i < 	INODE_CHARSIZE; i++)
		superBlock -> inodeCheck[i] = 0;		
}


DataBlock * getEmptyDataBlock(FileSystem * fileSystem)
{
	SuperBlock *superBlock = fileSystem -> superBlock;
	int i = -1;
	while(getBit(superBlock -> dataBlockCheck , ++i));	// 0�� ���ö� ����
	setBit(superBlock -> dataBlockCheck , i , 1); 
	return fileSystem -> dataBlock[i];
}
Inode * getEmptyInode(FileSystem * fileSystem)
{
	SuperBlock *superBlock = fileSystem -> superBlock;
	int i = -1;
	while(getBit(superBlock -> inodeCheck , ++i));	// 0�� ���ö� ����
	setBit(superBlock -> inodeCheck , i , 1);
	return fileSystem -> inode[i];
}
//���ο� ��Ÿ����� 

//������ ����Ʈ �ʱ⼳�� 
void initDataBlockList(DataBlockList ** address)
{
	*address = (DataBlockList*)malloc(sizeof(DataBlockList));
	DataBlockList * list = *address;
	list -> head = (DataBlockNode*)malloc(sizeof(DataBlockNode));	
	list -> tail = (DataBlockNode*)malloc(sizeof(DataBlockNode));
	list -> head -> next = list -> tail;
	list -> head -> prev = list -> head;
	list -> tail -> next = list -> tail;
	list -> tail -> prev = list -> head;
	list -> size = 0;
}

//������ ����Ʈ�� ������ ��� �ִ� �Լ� 
void pushDataBlockList(DataBlockList * list, DataBlock * dataBlock)
{
	DataBlockNode * newNode = (DataBlockNode*)malloc(sizeof(DataBlockNode));
	newNode -> value = dataBlock;
	newNode -> next = list -> tail;
	newNode -> prev = list -> tail -> prev;
	newNode -> prev -> next = newNode;
	list -> tail -> prev = newNode;
	list -> size++;
}
// �̱� �δ��̷�Ʈ �� ���� 
void makeSingleIndirect(FileSystem * fileSystem , Inode * inode)
{
	pushDataBlockList(inode -> data , getEmptyDataBlock(fileSystem));
	inode -> size++;
}
// ���� �δ��̷�Ʈ �� ���� 
void makeDoubleIndirect(FileSystem * fileSystem , Inode * inode)
{
	DataBlock * dataBlock = getEmptyDataBlock(fileSystem);
	pushDataBlockList(inode -> data , dataBlock);
	inode -> size ++;
	int i;
	for(i = 0; i < MAX_INDIRECT; i++)
	{
		int num = getEmptyDataBlock(fileSystem) -> num;
		copyBit(dataBlock -> data , &num , i * 10 , 22 , 10);
		inode -> size++;
	}
}
// �δ��̷�Ʈ ������ ������ �����ͺ� ������ 
DataBlock * getDataBlockIndirect(FileSystem * fileSystem , DataBlock * dataBlock , int n)
{
	int num;
	copyBit(&num , dataBlock -> data ,n * 10, 22 , 10);
	return fileSystem -> dataBlock[num];
}
// �δ��̷�Ʈ ������ ���� ��� ��ȣ �����ϱ� 
void setDataBlockIndirect(DataBlock * dataBlock , int n , int num)
{
	copyBit(dataBlock -> data , &num , n * 10 , 22 , 10);
}
//���̳�忡 �����ͺ� �����ϱ� 
void pushDataBlockInode(FileSystem * fileSystem ,Inode * inode , DataBlock *dataBlock)
{
	int size = inode -> size;
	DataBlockNode * header = inode -> data -> head;
	int a[3] = {1 , MAX_INDIRECT , MAX_INDIRECT * MAX_INDIRECT};
	int b[3] = {a[0],a[0]+a[1],a[0]+a[1]+a[2]};
	int c[3] = {b[0],b[0]+b[1],b[0]+b[1]+b[2]};
	/*������ �뷮 ä���- ����*/ 
	if(size == c[0])
		makeSingleIndirect(fileSystem , inode);
	else if(size == c[1])
		makeDoubleIndirect(fileSystem , inode);
	else if(size == c[2])
	{
		printf("Error - putDataInode");
		return;
	}
	/*������ �뷮 ä���- ��*/
	 
	 /*�ֱ�*/ 
	if(size == 0)
	{
		pushDataBlockList(inode -> data , dataBlock);
	}
	else if(size < c[1])
	{
		size -= (c[0] + b[0]);
		header = header -> next -> next;
		setDataBlockIndirect(header -> value , size, dataBlock -> num);
	}
	else if(size < c[2])
	{
		size -= (c[1] + b[1]);
		header = header -> next -> next -> next;
		DataBlock * doubleIn = getDataBlockIndirect(fileSystem , header -> value , size / a[1]);
		setDataBlockIndirect(doubleIn , size % a[0] , dataBlock -> num);
	}
	inode -> size++;
}

// ���̳�� ���� n��° ������ ������ 
DataBlock * getDataBlockInode(FileSystem * fileSystem , Inode * inode , int n)
{
	int a[3] = {1 , MAX_INDIRECT , MAX_INDIRECT * MAX_INDIRECT};
	int b[3] = {a[0],a[0]+a[1],a[0]+a[1]+a[2]};
	int c[3] = {b[0],b[0]+b[1],b[0]+b[1]+b[2]};
	DataBlockNode * header = inode -> data -> head;
	DataBlock * r = NULL;
	int size = inode -> size;	//
	if(n == 0)
		r = header -> next -> value;
	else if(n < c[1])
	{
		header = header -> next -> next;
		r = getDataBlockIndirect(fileSystem , header -> value , n - b[0]);
	}
	else if(n < c[2])
	{
		header = header -> next -> next -> next;;
		n -= b[1];
		r = getDataBlockIndirect(fileSystem , header -> value , size / a[1]);
		r = getDataBlockIndirect(fileSystem , r , size % a[1]);
	}
	return r;
}

char * getDataInode(FileSystem * fileSystem , Inode * inode , int fromBit,  int nBit)	//fromBit ���� nBit��ŭ�� ������ ��� 
{
	int i;
	char * r = (char*)malloc(sizeof(char) * SIZETOCHAR(nBit));
	DataBlock * h = getDataBlockInode(fileSystem , inode , fromBit / (DATABLOCK_BITE * 8));
	printf("test : b : %s \n",h ->data);
	for(i = 0; i < nBit; i++)
	{
		setBit(r , i , getBit(h -> data ,(fromBit + i) % (8 * DATABLOCK_BITE)));
		if(fromBit + i % (DATABLOCK_BITE * 8) == 0)	//��踦 �Ѿ�� 
			h = getDataBlockInode(fileSystem , inode , (fromBit + i) / (DATABLOCK_BITE * 8));	//���� ������ �� �Ҵ� 
	}
	return r;
}

void setDataInode(FileSystem * fileSystem, Inode * inode, int fromBit, int nBit, int fromSourceBit ,void * source)
{
	int i;
	DataBlock * h = getDataBlockInode(fileSystem , inode , fromBit / (DATABLOCK_BITE * 8));
	for(i = 0; i < nBit; i++)
	{
		setBit(h -> data , (fromBit + i) % DATABLOCK_BITE , getBit(source , i + fromSourceBit));
		if(fromBit + i % (DATABLOCK_BITE * 8) == 0)	//��踦 �Ѿ�� 
			h = getDataBlockInode(fileSystem , inode , (fromBit + i) / (DATABLOCK_BITE * 8));	//���� ������ �� �Ҵ� 
		
	}
}


