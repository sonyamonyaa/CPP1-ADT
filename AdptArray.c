#include <stdio.h>
#include <stdlib.h>
#include "AdptArray.h"

/***
 * implement the DS as an adaptable by size array.
 * meaning saving an array with general pointers,
 * whenever we get insertion to an index bigger than the array's size,
 * we'd reallocate the array to more memory for it to work.
 * we'd need to save current size and pointers for copying and removing elements
 *
 * any failure must return null or fail per case.
 *m
 */
typedef struct AdptArray_ {
    int size;
    COPY_FUNC fcopy;
    DEL_FUNC fdel;
    PRINT_FUNC fprint;
    PElement *elems; // inner array of elements
} adpt, *PAdptArray;

// creates an empty array
PAdptArray CreateAdptArray(COPY_FUNC copy, DEL_FUNC del, PRINT_FUNC print) {
    PAdptArray arr = (PAdptArray) malloc(sizeof(adpt) + sizeof(PElement));
    if (!arr)
        return NULL;
    arr->fcopy = copy;
    arr->fdel = del;
    arr->fprint = print;
    arr->size = 0;
    arr->elems = (PElement *) malloc(sizeof(PElement));
    return arr;
}

// frees array memory
void DeleteAdptArray(PAdptArray arr) {
    int len = arr->size;
    for (int i = 0; i < len; i++) {
        PElement temp = arr->elems[i];
        if (temp != NULL)
            arr->fdel(temp);
    }
    free(arr->elems);
    free(arr);
}

// saves a copy of the element in given index, while freeing previous element if exists
Result SetAdptArrayAt(PAdptArray arr, int index, PElement elem) {

    if (index > arr->size) {
        int new_size = index + 1; // increase size to index (where it contains it)
        // arr = (PAdptArray) realloc(arr, sizeof(PAdptArray) + sizeof(PElement) * (new_size)); 
        //ig when i realloc elems it also reallocs the arr so no need for it, also it bugged on 2nd increase
        
        arr->elems = (PElement *) realloc(arr->elems, sizeof(PElement) * (new_size));
        //init the new places in the array to be null
        for(int i = arr->size; i < new_size; i++){ //size is last-index + 1
            arr->elems[i] = NULL;
        }
        arr->size = new_size;
    }
    PElement curr = arr->elems[index];
    if (curr != NULL ) {
        arr->fdel(arr->elems[index]);
        if (arr->elems[index] != NULL)
            return FAIL;
    }
    //copy the new element and assign it at the index
    // PElement cp_elem = arr->fcopy(elem);
    arr->elems[index] = arr->fcopy(elem);
    if (arr->elems[index] == NULL)
        return FAIL;
    else
        return SUCCESS;
}

// returns a copy of an element in given index
PElement GetAdptArrayAt(PAdptArray arr, int index) {
    if(index >= arr->size)
        return NULL;
    if (arr->elems[index]== NULL)
        return NULL;
    PElement elem_temp = arr->elems[index];
    return arr->fcopy(elem_temp);
}

// returns the array size (or -1 if array has failed to create)
int GetAdptArraySize(PAdptArray arr) {
    if (arr == NULL)
        return -1;
    return arr->size;
}

// prints array's elements
void PrintDB(PAdptArray arr) {
    if (arr == NULL) {
        printf("empty array");
    }
    int len = arr->size;
    for (int i = 0; i < len; i++) {
        PElement temp = arr->elems[i];
        if (temp != NULL)
            arr->fprint(temp);
    }
}
