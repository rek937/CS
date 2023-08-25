/********************
* author : @rek937
* date : 2023-8-21
* code : GB2312
**********************/
#include <stdio.h>
#include "SinglyLinkTable.h"

// Create a node of linktable
/********************
* return a headnode 
**********************/
FLinkTable* CreateNode(int value) {
	FLinkTable* node = (FLinkTable*)malloc(sizeof(FLinkTable));
	// judge if the memory is successfully allocated
	if (node == NULL) {
		printf("Failed to allocate memory!\n");
		return NULL;
	}
	node->data = value;
	node->next = NULL;
	return node;
}

// judge if the linktable is empty
/************************
* �жϵ�����һ���ڵ��Ƿ�Ϊ�սڵ�
* �����Ӧ��Ϊͷָ��
* �Ǿͷ���1.���Ƿ���2,����ͷ�ڵ����ͷ���0
*************************/
int IsEmptyLinkTable(FLinkTable* linktable) {
	// the headnode doesn't exist
	if (linktable == NULL) {
		return 0;
	}
	// the linktable is empty
	else if (linktable->next == NULL) {
		return 1;
	}
	// the linktable is not empty
	else if (linktable->next != NULL) {
		return 2;
	}
}

// Init the data of linktable
/************************
* 1. Retrieve the return value of the IsEmptyLinkTable function and assign it to the variable 'flag' to determine whether the headnode or the linktable meets the standard
* 2. The incoming variable 'len' represents the length of the array(which must be gt or equal to the length of the link table).
*************************/
FLinkTable* InitLinkTable(int* array, FLinkTable* linktable, int len) {
	FLinkTable* headnode = linktable;	// store the head node
	FLinkTable* currentnode = headnode->next;	// The current node is defined to facilitate subsequent operations
	// when the linktable is empty and the length is determined by the value of 'len'
	if (len == 0) {
		return headnode;
	}
	// when the linktable is empty and the incoming value of 'len' has some problems can't find
	// when the flag == 1 the linktable is empty but the flag == 0 the headnode is NULL
	int flag = IsEmptyLinkTable(linktable);
	if (flag == 1) {
		return headnode;
	}
	else if (flag == 0) {
		return NULL;
	}
	// the operation of initializing the link table
	for (int i = 0; i < len; i++) {
		if (currentnode == NULL) {
			return headnode;
		}
		currentnode->data = array[i];
		currentnode = currentnode->next;
	}
	return headnode;
}

// Insert a node into the linktable
/*************************
* 1. judge if the incoming linktable is empty 
* 2. judge if the incoming linktable is the headnode(if it is, put the node on the next of the headnode without other operation)
* 3. judge if the value of len is greater than the value of index
* the index means the past position of the node you want to insert
**************************/
FLinkTable* InsertNode(FLinkTable* linktable, FLinkTable* insertnode, int index) {
	FLinkTable* headnode = linktable;
	int flag = IsEmptyLinkTable(linktable);
	if (flag == 0) {
		printf("The headnode of the linktable has something wrong or exist !\n");
		return NULL;
	}
	// flag == 1 means the linktable is empty
	// if the index is 0, insert the node immediately after the headnode without performing any other operations.
	else if (flag == 1) {
		linktable->next = insertnode;
		return headnode;
	}
	// flag == 2 means the linktable is not empty
	// judge if the index meets the requirements 
	else if (flag == 2) {
		FLinkTable* currentnode = headnode->next;
		for (int i = 0; i < index; i++) {
			currentnode = currentnode->next;
			flag = IsEmptyLinkTable(currentnode);		// judge if the next node of current is NULL
			if (flag == 1) {
				if (i == index) {
					currentnode->next = insertnode;
				}
				else {
					printf("index can't satisfies the requirements!\n");
					return headnode;
				}
			}
		}
		// the loop end
		// insert the node in the linktable
		insertnode->next = currentnode->next;
		currentnode->next = insertnode;
		return headnode;
	}
}

// Delete the node of linktable
/*************************
* 1. judge the linktable is empty or the headnode is exist
* 2. judge the index is greater than the length of linktable
**************************/
FLinkTable* DeleteNode(FLinkTable* linktable, int index) {
	FLinkTable* headnode = linktable;
	FLinkTable* currentnode = headnode->next;
	int flag = IsEmptyLinkTable(linktable);
	if (flag == 0) {
		printf("the headnode of the linktable has something wrong or exist !\n");
		return NULL;
	}
	else if (flag == 0) {
		printf("Sry, the linktable is empty !\n");
		return headnode;
	}
	for (int i = 0; i < index-1; i++) {
		if (currentnode == NULL) {
			printf("The index is gt the length of linktable !\n");
			return headnode;
		}
		else {
			currentnode = currentnode->next;
		}
	}
	// original : currentnode n1 currentnode->next->next || n1->next
	// result : currentnode currentnode->next->next(currentnode->next)
	FLinkTable* n1 = currentnode->next;
	currentnode->next = n1->next;
	free(n1);
	return headnode;
}

// Search the node of linktable
/*************************
* 1. judge the linktable is empty or the headnode is exist
**************************/
FLinkTable* SearchNode(FLinkTable* linktable, int value) {
	FLinkTable* headnode = linktable;
	FLinkTable* currentnode = headnode->next;
	int flag = IsEmptyLinkTable(linktable);
	if(flag == 0) {
		printf("the headnode of the linktable has something wrong or exist !\n");
		return NULL;
	}
	else if (flag == 0) {
		printf("Sry, the linktable is empty !\n");
		return headnode;
	}
	for (int i = 0;; i++) {
		if (currentnode == NULL) {
			printf("sry,can't find the result!\n");
			return headnode;
		}
		else if (currentnode->data == value) {
			return currentnode;
		}
		else {
			currentnode = currentnode->next;
		}
	}
}

