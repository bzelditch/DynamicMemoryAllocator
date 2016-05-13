#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"

/*
 * Include a description of your approach here.
 *
 */

typedef struct heap_header_t {
	unsigned int size;
	char *first_free;
} heap_header_t ;

typedef struct node {
	unsigned int size;
	char* next_free;
} node;

/* Sets up a new heap.  'heapptr' points to a chunk of memory (i.e. the heap)
 * of size 'heap_size' bytes.  Returns 0 if setup fails, nonzero if success.
 */
int hl_init(void *heapptr, unsigned int heap_size) {
//
  	return 0; // Failed

}

/* Allocates a block of memory of the given size from the heap starting at 
 * 'heapptr'. Returns a pointer to the block of memory on success; returns 0 
 * if the allocator cannot satisfy the request.
 */
void *hl_alloc(void *heapptr, unsigned int block_size) {
  	if(block_size <= 0){
  		return NULL;
  	}
  	heap_header_t *head = (heap_header_t *)heapptr;
  	char *current = head->first_free;
  	node *header = (node *)current;
  	while (header->next_free != NULL){
  		if (header->size >= block_size){
  			return (void *)header;
  		}
  		header = (node *)header->next_free;
  	}

  	return NULL; // Failed
}

/* Releases the block of previously allocated memory pointed to by blockptr.
 * Nop if blockptr == 0.
 */
void hl_release(void *heapptr, void *blockptr) {
	if(blockptr == 0){
		return;
	}


}

/* Changes the size of the memory block pointed to by blockptr,
 * returning a pointer to the new block, or 0 if the request cannot be
 * satisfied. The contents of the block should be preserved (even if
 * the location of the block changes).  If mem == 0, function should
 * behave like alloc().
 */
void *hl_resize(void *heapptr, void *blockptr, unsigned int new_block_size) {
  	if(new_block_size <= 0){
  		return NULL;
 	}
  	return NULL; // Failed
}

