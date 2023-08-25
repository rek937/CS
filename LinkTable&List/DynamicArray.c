/********************
* author : @rek937
* date : 2023-8-24
* code : GB2312
**********************/
#include <stdio.h>
#include "DynamicArray.h"

// Create a dynamic array
dynaarr* CreateArray() {
	dynaarr* list = (dynaarr*)malloc(sizeof(dynaarr));
	list->capacity = 10;
	list->array = (int*)malloc(list->capacity * sizeof(int));
	list->size = 0;
	list->increasement = 2;
	return list;
}

// Destroy a dynamic array
void DestroyArray(dynaarr* list) {
	free(list->array);
	free(list);
}

// Get the capacity of the array
int GetCapacity(dynaarr* list) {
	return list->capacity;
}

// Get the size of the array
int GetSize(dynaarr* list) {
	return list->size;
}

// Search the data of the array by the incoming parameters 'index'.
int SearchData(dynaarr* list, int index) {
	assert(index >= 0 && index < list->size);
	return list->array[index];
}

// Update the data of the element is pointed by the index
void UpdateDate(dynaarr* list, int index, int data) {
	asser(index >= 0 && index < list->size);
	list->array[index] = data;
}

// Expand the array.
/*******************
* Input the original array.
* Create a new array gt the incoming array.
* Copy the old array to the new array.
* return the new array.
********************/
dynaarr* ExpandArray(dynaarr* list) {
	// Allocate the space for the new array.
	dynaarr* newlist = (dynaarr*)malloc(sizeof(dynaarr));
	newlist->increasement = list->increasement;
	newlist->size = list->size;
	newlist->capacity = newlist->increasement * list->capacity;
	newlist->array = (int*)malloc(newlist->capacity * sizeof(int));
	// Copy the original array to the new array.
	for (int i = 0; i < list->size; i++) {
		newlist->array[i] = list->array[i];
	}
	// Free the space of the original array.
	free(list->array);
	free(list);
	return newlist;
}

// Insert a element at the end of the array.
void EInsertElement(dynaarr* list, int data) {
	if (list->size == list->capacity) {
		list = ExpandArray(list);
	}
	list->array[list->size] = data;
	list->size++;
}

// Insert a element at the middle of the array.
/********************
* judge if expand the array.
* Insert the element before where be pointed by the index.
* Let the element which is pointed by the index move back.
*********************/ 
void MInsertElement(dynaarr* list, int index, int data) {
	assert(index >= 0 && index < list->size);
	if (list->size == list->capacity) {
		list = ExpandArray(list);
	}

	for (int i = list->size; i >= index; i--) {
		list->array[i + 1] = list->array[i];
	}
	list->array[index] = data;
}



