#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "heaplib.h"
#include "sizetask.h"

#define ITERS          64
#define REQ_SIZE       64

/* This is just an example task. Replace it with your code.
 * 
 * Provide a short description of your task here:
 * 
 *
 */
bool pass_sizetask(unsigned int heap_size) {
  
	void *heap, *ptrs[ITERS];
	bool success_f = true;
    int i;
	
	heap = malloc(heap_size); // allocate the heap

	success_f = hl_init(heap, heap_size);
	for (i = 0; i < ITERS && success_f; i++) {
		ptrs[i] = hl_alloc(heap, REQ_SIZE);
		success_f = (ptrs[i] != NULL);
	}

	free(heap); // free the heap
	return success_f;	
}
