#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"

/*
 * Include a description of your approach here.
 *
 */

typedef struct heap_header {
	unsigned int size;
	char *first_free;
} heap_header ;

typedef struct free_block_header {
	unsigned int size;
	char* next_free;
} free_block_header;

typedef struct alloc_block_header {
	unsigned int size;
} alloc_block_header;

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
  	if(block_size < 0){
  		return NULL;
  	}
  	heap_header *head = (heap_header *)heapptr;
  	char *first = head->first_free;
  	free_block_header *header = (free_block_header *)first;
  	while (header->next_free != NULL){
  		if (header->size >= block_size){
  			if (header->size == block_size){
  				//change free pointers for pointers in front of and behind

  			}
  			else{
  				header->size = header->size - sizeof(alloc_block_header) - block_size;
  				return (void *)header;
  			}
  		}
  		header = (free_block_header *)header->next_free;
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

