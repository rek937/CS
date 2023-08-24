/********************
* author : @rek937
* date : 2023-8-23
* code : GB2312
**********************/
#include <stdio.h>

typedef struct node{
	int data;
	struct node* next;
	struct node* prev;
}CLinkTable;

// Create a node for the double circular link table.
CLinkTable* CreateNode(int value);

// Check if the link table is empty.
int IsEmptyLinkTable(CLinkTable* linktable);

// Init the double circular link table.
CLinkTable* InitLinkTable(CLinkTable* linktable, int* array, int len);

// Insert a node into the link table.
CLinkTable* InsetNode(CLinkTable* linktable, CLinkTable* node, int index);

// Delete a node from a link table.
CLinkTable* DeleteNode(CLinkTable* linktable, int value);

// Search a node meets the incoming value.
CLinkTable* DeleteNode(CLinkTable* linktable, int value);
