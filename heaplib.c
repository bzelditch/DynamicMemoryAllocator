#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "heaplib.h"

#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))

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
	char* prev_free;
} free_block_header;

typedef struct block_header {
	unsigned int size;
} block_header;

/* Sets up a new heap.  'heapptr' points to a chunk of memory (i.e. the heap)
 * of size 'heap_size' bytes.  Returns 0 if setup fails, nonzero if success.
 */
int hl_init(void *heapptr, unsigned int heap_size) {
//
  	heap_header *heap = (heap_header *)heapptr; //set initial pointer to the heap_header

    if((*(int *)heap) % ALIGNMENT != 0){ //if the heap pointer is not aligned properly
                                         //then align it
      heap = (heap_header *)((char *)heap + ALIGNMENT - (*(int*)heap) % ALIGNMENT);
    }

    heap->size = heap_size; //set the size of the entire heap
    heap->first_free = ADD_BYTES(heap, sizeof(heap_header)); //point to the first free block

    if(heap_size < sizeof(heap_header)){
      return 0;
    }

    free_block_header *root = (free_block_header *)heap->first_free; //set the root of the linkedlist
    root->size = heap_size + sizeof(free_block_header) + sizeof(heap_header);
    root->next_free = NULL; //set the next_free pointer to NULL
    root->prev_free = NULL;

    block_header *end_header = (block_header *)ADD_BYTES(root, root->size - 4);
    end_header->size = heap_size;

    return 1;

}

/* Allocates a block of memory of the given size from the heap starting at 
 * 'heapptr'. Returns a pointer to the block of memory on success; returns 0 
 * if the allocator cannot satisfy the request.
 */
void *hl_alloc(void *heapptr, unsigned int block_size) {
	//8 byte aligned block size
  	if(block_size < 0 || block_size < 2*sizeof(block_header)){
  		return 0;
  	}
  	heap_header *head = (heap_header *)heapptr;
  	char *first = head->first_free;
  	free_block_header *header = (free_block_header *)first;
  	unsigned int total_size = block_size + 2*sizeof(block_header);
  	do{
		if (header->size >= total_size){
			//void* back_pointer = (void *)ADD_BYTES(header, 8);
			//free_block_header *back_header = (free_block_header *)back_pointer;
			//void* prev = back_header->prev_free;
			void* prev = header->prev_free;
			free_block_header *prev_header = (free_block_header *)prev;

			free_block_header *next_header = (free_block_header *)header->next_free;
			//void* next_back_pointer = next_header->prev_free;
			//void* next_back_pointer = (void *)ADD_BYTES(next_header, 8);
			//free_block_header *next_back_header = (free_block_header *)next_back_pointer;

  			if (header->size == total_size){
  				next_header->prev_free = header->prev_free;
  				prev_header->next_free = header->next_free;

  				//change free pointers for pointers in front of and behind
			}
			else{
				void* shifted = (void *)ADD_BYTES(header, total_size);
				free_block_header *new_header = (free_block_header *)shifted;
				new_header->size = header->size - total_size;
				new_header->next_free = header->next_free;
				new_header->prev_free = header->prev_free;

				block_header *new_back_header = (block_header *)ADD_BYTES(header, header->size - 4);
				new_back_header->size = header->size - total_size;

				block_header *new_alloc_front_header = (block_header *)header;
				new_alloc_front_header->size = total_size + 1;

				block_header *new_alloc_back_header = (block_header *)ADD_BYTES(header, total_size - 4);
				new_alloc_back_header->size = total_size + 1;

				next_header->prev_free = shifted;
  				prev_header->next_free = shifted;
			}
			return (void *)header;
		}
  		header = (free_block_header *)header->next_free;  		
  	} while (header != NULL);
 
  	return NULL; // Failed

    //NOTE: Probably need to include an alloc_header in this function
    //Also, check the alignment
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
 * the location of the block changes).  If blockptr == 0, function should
 * behave like alloc().
 */
void *hl_resize(void *heapptr, void *blockptr, unsigned int new_block_size) {
	block_header *old_block = (block_header *)blockptr;
  	if(new_block_size <= 0){
  		return NULL;
 	}
 	void *new_alloc = hl_alloc(heapptr, new_block_size);
 	if(blockptr == 0 || new_alloc == 0){
 		return new_alloc;
 	}
 	void *copied = memcpy(new_alloc, blockptr, old_block->size);
 	hl_release(heapptr, blockptr);
  	return copied;
}
