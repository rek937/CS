/********************
* author : @rek937
* date : 2023-8-22
* code : GB2312
**********************/
#include <stdio.h>
#include "SinglyCircularLinkTable.h"

// Create a node of link table
CLinkTable* CreateNode(int value) {
	CLinkTable* node = (CLinkTable*)malloc(sizeof(CLinkTable));
	if (node == NULL) {
		printf("Sry, Failed to create the node, try again!\n");
		return NULL;
	}
	node->data = value;
	// Every node in singly circular link table points to itself.
	// Or they are the headnode of themselves.
	node->next = node;
}

// Judge if the link table is empty
/**************************
* The circular link table node points to itself immediately.
* Therefore determin if the node points itself, we can judge if the link table is empty.
***************************/
int IsEmptyLinkTable(CLinkTable* linktable) {
	if (linktable == NULL) {
		return 0;
	}
	// the linktable node points to itself immediately.
	else if (linktable->next == linktable) {
		return 1;
	}
	else if (linktable->next != linktable) {
		return 2;
	}
}

// Init the data of linktable
/***************************
* The situation which the current node is the head node represents that the link table has been traversed once,
****************************/
CLinkTable* InitLinkTable(CLinkTable* linktable, int* array, int len) {
	CLinkTable* headnode = linktable;
	CLinkTable* currentnode = headnode;
	// The return value of the IsEmptyLinkTable function is stored by the variable 'flag'.
	int flag = IsEmptyLinkTable(linktable);
	if (flag == 0) {
		printf("Sry, the incoming headnode is NULL,try again!\n");
		return NULL;
	}
	else if (flag == 1) {
	// The link table consists of a signal node.The value of array[0] can be assigned by the node.
		linktable->data = array[0];
		return headnode;
	}
	else if (flag == 2) {
	// The link table contains multiple node.
		for (int i = 0; i < len; i++) {
			if (currentnode == headnode) {
			// The link table has been traversed once.
				return headnode;
			}
			currentnode->data = array[i];
			currentnode = currentnode->next;
		}
		return headnode;
	}
}

// Insert a node into the link table
/***************************
* Due to the nature of a singly link table, it's not possible to index the target node directly by a non-fixed head node.
* Therefore we need to fixed a headnode to facilitate our subsequent indexing operation.
* The head node represents the first position of indexing operation,
****************************/
CLinkTable* InsertNode(CLinkTable* node, CLinkTable* linktable, int index) {
	CLinkTable* headnode = linktable;
	CLinkTable* currentnode = headnode;
	int flag = IsEmptyLinkTable(linktable);
	if (flag == 0) {
	// The incoming head node is NULL.
		printf("Sry, the incoming headnode is NULL,try again!\n");
		return NULL;
	}
	else if (flag == 1) {
	// The link table consists of a signal node.
	// Just judge the value of the index to determine whether the node insert the front of the head node or farward.
		if (index == -1) {
			node->next = headnode;
			headnode->next = node;
		}
		else if (index == 1) {
			headnode->next = node;
			node->next = headnode;
		}
		else {
			printf("Sry, there are some errors, input the index again\n");
			return headnode;
		}
		return headnode;
	}
	else if (flag == 2) {
	// The link table consists of multiple nodes.
		for (int i = 0; i < index - 1; i++) {
			currentnode = currentnode->next;
			if (currentnode == headnode) {
			// The link table has been traversed once.
				printf("Sry, the index is out of the link table,try again!\n");
				return headnode;
			}
		}
		node->next = currentnode->next;
		currentnode->next = node;
		return headnode;
	}
}

// Delete a node from the link table
/***************************
* Sucess: return 1
* Fail: return 0
****************************/
int DeleteNode(CLinkTable* linktable, int value) {
	CLinkTable* headnode = linktable;
	CLinkTable* currentnode = headnode;
	int flag = IsEmptyLinkTable(linktable);
	if (flag == 0) {
		printf("Sry, the head node is NULL, try again!\n");
		return 0;
	}
	else if (flag == 1) {
		if (headnode->data == value) {
			free(headnode);
			return 1;
		}
		else {
			printf("Sry, can't find the value, try again!\n");
			return 0;
		}
	}
	else if (flag == 2) {
		while (currentnode != headnode) {
			if (currentnode->data == value) {
				return 1;
			}
			else {
				currentnode = currentnode->next;
			}
		}
		printf("Sry, the value is not in the link table!\n");
		return 0;
	}
}

// Search a node from the link table
/***************************
* 
****************************/
CLinkTable* SearchNode(CLinkTable* linktable, int value) {
	CLinkTable* headnode = linktable;
	CLinkTable* currentnode = headnode;
	int flag = IsEmptyLinkTable(linktable);
	if (flag == 0) {
		printf("Sry, the head node is NULL, try again!\n");
		return NULL;
	}
	else if (flag == 1) {
		if (headnode->data == value) {
			return headnode;
		}
		else {
			printf("Sry, can't find the value, try again!\n");
			return NULL;
		}
	}
	else if (flag == 2) {
		while (currentnode != headnode) {
			if (currentnode->data == value) {
				return currentnode;
			}
			else {
				currentnode = currentnode->next;
			}
		}
		printf("Sry, the incoming value is not in the link table!\n");
		return NULL;
	}
}


