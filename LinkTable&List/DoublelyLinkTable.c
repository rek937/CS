/********************
* author : @rek937
* date : 2023-8-23
* code : GB2312
**********************/
#include <stdio.h>
#include "DoublelyLinkTable.h"

// Create a node of link table
FLinkTable* CreateNode(int value) {
	FLinkTable* node = (FLinkTable*)malloc(sizeof(FLinkTable));
	if (node == NULL) {
		printf("Sry,failed to create a node of link table,try again !\n");
		return NULL;
	}
	node->data = value;
	node->next = NULL;
	node->prev = NULL;
}

// Judge if the link table is empty
/****************************
* parameters : link table(head node)
* just only judge if the next of head node is NULL without checking the prev of head node.
*****************************/
int IsEmptyLinkTable(FLinkTable* linktable) {
	// The head node is NULL.
	if (linktable == NULL) {
		return 0;
	}
	// The link table is empty.
	else if (linktable->next == NULL) {
		return 1;
	}
	// The link table is not empty.
	else if (linktable->next != NULL) {
		return 2;
	}
}

// Init the data of linktable
/****************************
* Index the link table forward and store the corresponding array values in the link table.
*****************************/
FLinkTable* InitLinkTable(FLinkTable* linktable, int* array, int len) {
	// Determine whether the link table is empty or the head node is NULL by the value of 'flag'
	// retrieve the return value of IsEmptyLinkTable function by the variable 'flag'
	int flag = IsEmptyLinkTable(linktable);
	FLinkTable* headnode = linktable;
	FLinkTable* currentnode = headnode->next;
	if (flag == 0) {
		printf("Sry, the head node is NULL, try again!\n");
		return headnode;
	}
	else if (flag == 1) {
		printf("Sry, the link table is empty, try again!\n");
		return headnode;
	}
	else if (flag == 2) {
		for (int i = 0; i < len; i++) {
			if (currentnode == NULL) {
				return headnode;
			}
			currentnode->data = array[i];
			currentnode = currentnode->next;
		}
		return headnode;
	}
}

// Insert a node into the link table
/****************************
* The index represents that calculate the offset starting of the next node of the head node of the link table.
* The 'node' parameter will be determined the position in the link table by the 'index' parameter. 
* The 'index' parameter stands for the next of the position which be pointed by the 'index'.
*****************************/
FLinkTable* InsertNode(FLinkTable* node, FLinkTable* linktable, int index) {
	FLinkTable* headnode = linktable;
	FLinkTable* currentnode = headnode->next;
	// define 'flag' variable to store the return value of IsEmptyLinkTable function
	int flag = IsEmptyLinkTable(linktable);
	// The incoming variable 'linktable' has some trouble.
	if (flag == 0) {
		printf("Sry, the head node is NULL, try again!\n");
		return headnode;
	}
	// The link table is empty. There are 2 situations.
	else if (flag == 1) {
		// The node is to be stored in the next of head node.
		if (index == 0) {
			headnode->next = node;
			node->prev = headnode;
			return headnode;
		}
		// The node intends to be stored in a position other than the next node of the head node, but it doesn't match the current state of the link table.
		else {
			printf("Sry, the index doesn't match the current state of the link table, try again!\n");
			return headnode;
		}
	}
	// The link table contains multiple nodes.Assess whether the index aligns with the current state of the link table.
	// Include the check to verify if the index exceeds the length of the link table.
	else if (flag == 2) {
		for (int i = 0; i < index; i++) {
			if (currentnode == NULL) {
				return headnode;
			}
			currentnode = currentnode->next;
		}
		if (currentnode->next == NULL) {
			currentnode->next = node;
			node->prev = currentnode;
		}
		else if (currentnode->next != NULL) {
			node->prev = currentnode;
			node->next = currentnode->next;
			node = node->next->prev;
			node = currentnode->next;
		}
	}
}

// Delete a node from the link table
/****************************
* The index point position of the node which will be deleted. 
*****************************/
FLinkTable* DeleteNode(FLinkTable* linktable, int index) {
	FLinkTable* headnode = linktable;
	FLinkTable* currentnode = headnode->next;
	// define 'flag' variable to store the return value of IsEmptyLinkTable function
	int flag = IsEmptyLinkTable(linktable);
	// The incoming variable 'linktable' has some trouble.
	if (flag == 0) {
		printf("Sry, the head node is NULL, try again!\n");
		return headnode;
	}
	// The link table is empty.
	else if (flag == 1) {
		printf("Sry, the link table is empty, try again!\n");
		return headnode;
	}
	// Include the check to verify if the index exceeds the length of the link table.
	else if (flag == 2) {
		for (int i = 0; i < index; i++) {
			currentnode = currentnode->next;
		}
		currentnode->next->prev = currentnode->prev;
		currentnode->prev->next = currentnode->next;
		free(currentnode);
		return headnode;
	}
}

// Search a node from the link table
/****************************
* To find a node which contains the data value is equal to the value of the 'value' parameter.
*****************************/
FLinkTable* SearchNode(FLinkTable* linktable, int value) {
	FLinkTable* headnode = linktable;
	FLinkTable* currentnode = headnode->next;
	// define 'flag' variable to store the return value of IsEmptyLinkTable function
	int flag = IsEmptyLinkTable(linktable);
	// The incoming variable 'linktable' has some trouble.
	if (flag == 0) {
		printf("Sry, the head node is NULL, try again!\n");
		return headnode;
	}
	// The link table is empty.
	else if (flag == 1) {
		printf("Sry, the link table is empty, try again!\n");
		return headnode;
	}
	else if (flag == 2) {
		while (currentnode->data != value && currentnode != NULL) {
			currentnode = currentnode->next;
		}
		return currentnode;
	}
}