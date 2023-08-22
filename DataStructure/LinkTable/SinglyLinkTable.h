#include <stdio.h>

// define a struct of linktable
typedef struct node{
	int data;
	struct node* next;
}FLinkTable;

// Create a node of linktable
FLinkTable* CreateNode(int value);

// judge if the linktable is empty
int IsEmptyLinkTable(FLinkTable* linktable);

// Init the data of linktable
FLinkTable* InitLinkTable(int* array, FLinkTable* linktable, int len);

// Insert a node into the linktable
FLinkTable* InsertNode(FLinkTable* linktable, FLinkTable* insernode, int index);

// Delete the node of linktable
FLinkTable* DeleteNode(FLinkTable* linktable, int index);

// Search the node of linktable
FLinkTable* SearchNode(FLinkTable* linktable, int value);