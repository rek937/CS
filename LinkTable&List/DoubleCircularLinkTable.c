/********************
* author : @rek937
* date : 2023-8-23
* code : GB2312
**********************/
#include <stdio.h>
#include "DoubleCircularLinkTable.h"

// Create a node for the double circular link table.
CLinkTable* CreateNode(int value) {
	CLinkTable* node = (CLinkTable*)malloc(sizeof(CLinkTable));
	node->next = node;
	node->prev = node;
	node->data = value;
	return node;
}

// Check if the link table is empty.
int IsEmptyLinkTable(CLinkTable* linktable) {
	CLinkTable* headnode = linktable;
	if (headnode == NULL) {
	// The head node is not exist.
		return 0;
	}
	else if (headnode->next == headnode && headnode->prev == headnode) {
	// The link table is empty.
		return 1;
	}
	else if (headnode->next != headnode && headnode->prev != headnode) {
	// The link table consists 2 nodes at least.
		return 2;
	}
}

// Init the double circular link table.
CLinkTable* InitLinkTable(CLinkTable* linktable, int* array, int len) {
	CLinkTable* headnode = linktable;
	CLinkTable* currentnode = headnode;
	int flag = IsEmptyLinkTable(headnode);
	if (flag == 0) {
		return NULL;
	}
	else if (flag == 1) {
		headnode->data = array[0];
		return headnode;
	}
	else if (flag == 2) {
		for (int i = 0; i < len; i++) {
			currentnode->data = array[i];
			currentnode = currentnode->next;
			if (currentnode == headnode) {
				return headnode;
			}
		}
	}
}

// Insert a node into the link table.
CLinkTable* InsetNode(CLinkTable* linktable, CLinkTable* node, int index) {
	CLinkTable* headnode = linktable;
	CLinkTable* currentnode = headnode;
	int flag = IsEmptyLinkTable(headnode);
	if (flag == 0) {
		return NULL;
	}
	else if (flag == 1) {
		if (index == 1) {
			headnode->next = node;
			node->prev = headnode;
			headnode->prev = node;
			node->prev = headnode;
			return headnode;
		}
		else {
			return NULL;
		}
	}
	else if (flag == 2) {
		for (int i = 0; i < index; i++) {
			currentnode = currentnode->next;
			if (currentnode == headnode) {
				return NULL;
			}
		}
		currentnode->next->prev = node;
		node->next = currentnode->next;
		node->prev = currentnode;
		currentnode->next = node;
		return headnode;
	}
}

// Delete a node from a link table.
CLinkTable* DeleteNode(CLinkTable* linktable, int value) {
	CLinkTable* headnode = linktable;
	CLinkTable* currentnode = headnode;
	int flag = IsEmptyLinkTable(headnode);
	if (flag == 0) {
		return NULL;
	}
	else if (flag == 1) {
		if (headnode->data == value) {
			free(headnode);
			return NULL;
		}
	}
	else if (flag == 2) {
		while (currentnode != headnode) {
			if (currentnode->data == value) {
				free(currentnode);
				return headnode;
			}
			currentnode = currentnode->next;
		}
		return headnode;
	}
}

// Search a node meets the incoming value.
CLinkTable* DeleteNode(CLinkTable* linktable, int value) {
	CLinkTable* headnode = linktable;
	CLinkTable* currentnode = headnode;
	int flag = IsEmptyLinkTable(headnode);
	if (flag == 0) {
		return NULL;
	}
	else if (flag == 1) {
		if (headnode->data == value) {
			return headnode;
		}
	}
	else if (flag == 2) {
		while (currentnode != headnode) {
			if (currentnode->data == value) {
				return currentnode;
			}
			currentnode = currentnode->next;
		}
		return NULL;
	}
}