/********************
* author : @rek937
* date : 2023-8-23
* code : GB2312
**********************/
#include <stdio.h>

// define a structure of link table
typedef struct node{
	int data;
	struct node* next;
}CLinkTable;

// Create a node of link table
CLinkTable* CreateNode(int value);

// Judge if the link table is empty
int IsEmptyLinkTable(CLinkTable* linktable);

// Init the data of linktable
CLinkTable* InitLinkTable(CLinkTable* linktable, int* array, int len);

// Insert a node into the link table
CLinkTable* InsertNode(CLinkTable* node, CLinkTable* linktable, int index);

// Delete a node from the link table'
int DeleteNode(CLinkTable* linktable, int value);

// Search a node from the link table
CLinkTable* SearchNode(CLinkTable* linktable, int value);