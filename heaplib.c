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
  	heap_header *heap = (heap_header *)heapptr; //set initial pointer to the heap_header

    if((*(int *)&heap) % ALIGNMENT != 0){ //if the heap pointer is not aligned properly
                                         //then align it
      heap = (heap_header *)((char *)heap + ALIGNMENT - (*(int*)&heap) % ALIGNMENT);
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
  	//if(block_size < 0 || block_size < 2*sizeof(block_header)){
	if(block_size <= 0){
  		return 0;
  	}
  	unsigned int total_size = block_size + 2*sizeof(block_header);
  	if(total_size % ALIGNMENT != 0){
     	total_size += ALIGNMENT - (total_size % ALIGNMENT);
    }

  	heap_header *head = (heap_header *)heapptr;
  	char *first = head->first_free;
  	free_block_header *header = (free_block_header *)first;
  	char *f_free = NULL;
  	//do while loop to see if any free blocks can fit the requested block
  	do{
		if (header->size >= total_size){
			char *prev = header->prev_free;
			char *next = header->next_free;
			free_block_header *prev_header = (free_block_header *)prev;
			free_block_header *next_header = (free_block_header *)next;
  			if (header->size == total_size){
  				//if sizes match, only need to change pointers of previous and next free blocks
  				if (prev != NULL){
  					prev_header->next_free = header->next_free;
  				}
  				if (next != NULL){
  					next_header->prev_free = header->prev_free;
  					f_free = next;//in case current is first free, set new first free to next
  				}
			}
			else{
				//change free block header
				char* shifted = ADD_BYTES(header, total_size);
				free_block_header *new_header = (free_block_header *)shifted;
				new_header->size = header->size - total_size;
				new_header->next_free = header->next_free;
				new_header->prev_free = header->prev_free;
				//set footer for free block
				block_header *new_back_header = (block_header *)
				  ADD_BYTES(header, header->size - sizeof(unsigned int));
				new_back_header->size = header->size - total_size;
				//set new pointers for previous and next free blocks
				if (prev != NULL){
  					prev_header->next_free = shifted;
  				}
  				if (next != NULL){
  					next_header->prev_free = shifted;
  				}
  				f_free = shifted;
			}
			//new header for the alloc'd block, taking into account the allocated flag by adding 1
			block_header *new_alloc_front_header = (block_header *)header;
			new_alloc_front_header->size = total_size + 1;

			//new footer for the alloc'd block, taking into account the allocated flag by adding 1
			block_header *new_alloc_back_header = (block_header *)
			  ADD_BYTES(header, total_size - sizeof(unsigned int));
			new_alloc_back_header->size = total_size + 1;

			//update first_free field in heap_header if changed
			if(first == (char *)header){
				head->first_free = f_free;
			}

			return (void *)header;
		}
  		header = (free_block_header *)header->next_free;  		
  	} while (header != NULL);

  	return 0;
}

/* Releases the block of previously allocated memory pointed to by blockptr.
 * Nop if blockptr == 0.
 */
void hl_release(void *heapptr, void *blockptr) {
	if(blockptr == 0){
		return;
	}

	int is_alloc_before, is_alloc_after = 1; //booleans to determine the allocation status
                                          //of the blocks before and after this block.
	                                      //1 if allocated, 0 if free

	//Get the size of the allocated block
	unsigned int block_size = ((block_header *)blockptr)->size;

    //Point to the footer of the block right before this block
	unsigned int *before_size_ptr = (unsigned int *)ADD_BYTES(blockptr, -sizeof(unsigned int));

    //Should be true if not equivalent to 0 mod 2 and false otherwise
	if(before_size_ptr == NULL || *(int *)before_size_ptr % 2 == 0){
		is_alloc_before = 0;
	}

	//Should point to the header of the next block
	unsigned int* after_size_ptr = (unsigned int*)ADD_BYTES(blockptr, block_size);

	//Same as above
	if(after_size_ptr == NULL || *(int *)after_size_ptr % 2 == 0){
		is_alloc_after = 0;
	}

	//We need the first free block after blockptr. This is used down below in one
	//of the edge cases (line 208)
	int keep_going = 1;
	char *first_free_block_after;
	char *current = (char *)blockptr;
	while(keep_going == 1){
		current = ADD_BYTES(current, *current);

		if(is_alloc_after == 0){
			keep_going = 0;
			first_free_block_after = current;
		}

		if(current == NULL){
			keep_going = 0;
			first_free_block_after = NULL;
		}

		if(*(int *)current % 2 == 0){
			keep_going = 0;
			first_free_block_after = current;
		}

	}

	//If both blocks before and after blockptr are free
	if(is_alloc_before == 0 && is_alloc_after == 0){

		//The total size of the newly freed, coalesced block
		unsigned int total_free_size = *before_size_ptr + block_size + *after_size_ptr - 1;

		//set the next_free pointer for the free block directly before blockptr
		//don't need to change the prev_free pointer at all

		char *before_ptr_char = ADD_BYTES(blockptr, -*before_size_ptr);
		free_block_header *before_ptr = (free_block_header *)before_ptr_char;
		before_ptr->size = total_free_size;
		before_ptr->next_free = ((free_block_header *)ADD_BYTES(blockptr, block_size))->next_free;
	}


	if(is_alloc_before == 1){
     //We need to keep going back in the heap until we hit the first free block
	//Note: check the NULL case

	int keep_going_back = 1;
	unsigned int *current = before_size_ptr;

		while(keep_going_back == 1){
			//Move the current pointer back to the footer of the next block 
			current = (unsigned int*)ADD_BYTES(current, -*current);

			//If current == NULL then this newly freed block will be the first free block in the heap.
			//So new_prev_free = NULL and new_next_free = the address of the first free block AFTER this block
			//
			if(current == NULL){
				keep_going_back = 0;
			 	free_block_header *new_free = (free_block_header *)blockptr;
			 	new_free->prev_free = NULL;

			 	//Update the first_free field of the heapptr
			 	heap_header *heap = (heap_header *)heapptr;
			 	heap->first_free = (char *)blockptr;

			 	if(is_alloc_after){
			 	//Need the address of the first free block AFTER blockptr
			 		new_free->next_free = first_free_block_after;
			 		new_free->size = block_size;

			 	}

			 	else{
			 		free_block_header *after_block = (free_block_header *)after_size_ptr;
			 		new_free->next_free = after_block->next_free;
			 		new_free->size = block_size + *after_size_ptr;
			 	}

			}

			//If we hit a free block
			else if(*(int *)current % 2 == 0){
			 	keep_going_back = 0;

			 	//Set the newly freed block's prev_free to the other free block
			 	free_block_header *new_free = (free_block_header *)blockptr;
			 	new_free->prev_free = ADD_BYTES(current, -(*current - sizeof(unsigned int)));

			 	//This is the found free block
			 	free_block_header *found_free = (free_block_header *)ADD_BYTES(current, -(*current - sizeof(unsigned int)));

			 	if(is_alloc_after == 1){
			 		//Set the newly freed block's next_free to other free block's next_free
			 		new_free->next_free = found_free->next_free;
			 		found_free->next_free = (char *)new_free;

			 		//Now need to change the size field in the newly freed block
			 		new_free->size = block_size - 1;
			 	}

			 	else{
			 		unsigned int after_block_size = *after_size_ptr;
			 		unsigned int total_free_size = block_size + after_block_size - 1;

			 		free_block_header *after_block = (free_block_header *)after_size_ptr;
			 		new_free->next_free = after_block->next_free;
			 		new_free->size = total_free_size;
			 		found_free->next_free = (char *)new_free;
			 	}
			 }

		}
	}

	//Case where before block is free and after is alloc'd
	//Don't need to change pointers. Just need to change size
	else{

		//Total size of the newly freed, coalesced block
		unsigned int total_free_size = block_size + *before_size_ptr - 1;

		free_block_header *before_block = (free_block_header *)ADD_BYTES(blockptr, -*before_size_ptr);
		before_block->size = total_free_size;
	}
}

/* Changes the size of the memory block pointed to by blockptr,
 * returning a pointer to the new block, or 0 if the request cannot be
 * satisfied. The contents of the block should be preserved (even if
 * the location of the block changes).  If blockptr == 0, function should
 * behave like alloc().
 */
void *hl_resize(void *heapptr, void *blockptr, unsigned int new_block_size) {
	if(new_block_size <= 0 || blockptr == 0){
  		return NULL;
 	}
 	void *new_alloc = hl_alloc(heapptr, new_block_size);
 	if(new_alloc == 0){
 		return new_alloc;
 	}
 	block_header *old_block = (block_header *)blockptr;
 	void *copied = memcpy(new_alloc, blockptr, old_block->size - 1);
 	hl_release(heapptr, blockptr);
  	return copied;
}
