#include <stdio.h>

// create a structure of double linktable
typedef struct linktable {
	int data;
	struct linktable* next;
	struct linktable* prev;
}FLinkTable;

// Create a node of link table
FLinkTable* CreateNode(int value);

// Judge if the link table is empty
int IsEmptyLinkTable(FLinkTable* linktable);

// Init the data of linktable
FLinkTable* InitLinkTable(FLinkTable* linktable, int* array, int len);

// Insert a node into the link table
FLinkTable* InsertNode(FLinkTable* node, FLinkTable* linktable, int index);

// Delete a node from the link table
FLinkTable* DeleteNode(FLinkTable* linktable, int index);

// Search a node from the link table
FLinkTable* SearchNode(FLinkTable* linktable, int value);
