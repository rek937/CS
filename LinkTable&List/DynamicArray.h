/********************
* author : @rek937
* date : 2023-8-24
* code : GB2312
**********************/
#include <stdio.h>

// Define the structure of dynamic array.
struct DynamicArray {
	int* array;
	int capacity;		// The capacity of the array.
	int size;			// The quantity of the data in the array.
	int increasement;	// The times of increasing capacity.
};

typedef struct DynamicArray dynaarr;

// Create a dynamic array
dynaarr* CreateArray();

// Destroy a dynamic array
void DestroyArry(dynaarr* list);

// Get the capacity of the array
int GetCapacity(dynaarr* list);

// Get the size of the array
int GetSize(dynaarr* list);

// Search the data of the array by the incoming parameters 'index'.
int SearchData(dynaarr* list, int index);

// Update the data of the element is pointed by the index
void UpdateDate(dynaarr* list, int index, int data);

// Expand the array.
dynaarr* ExpandArray(dynaarr* list);

// Insert a element at the end of the array.
void EInsertElement(dynaarr* list, int data);

// Insert a element at the middle of the array.
void MInsertElement(dynaarr* list, int index, int data);
