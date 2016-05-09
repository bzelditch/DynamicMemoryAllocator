#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"

/* Useful shorhand: casts a pointer to a (char *) before adding */
#define ADD_BYTES(base_addr, num_bytes) (((char *)(base_addr)) + (num_bytes))

/* You must implement these functions according to the specification given in
 * heaplib.h. You can define any types you like to this file.
 */

typedef struct heap_header_t {
	unsigned int size;
	int bytes_free;
	char *next_free;
} heap_header_t ;

/* This is an example of a helper print function that you may find
 * useful for programming and debugging purposes.  Reminder to always
 * wrap all print statments inside an #ifdef so that the default
 * compile does not include them.  Functton calls and printfs will
 * kill your performance and create huge output files, for which we
 * will have ZERO TOLERANCE.
 */
void print_heap(heap_header_t *heap) {
	printf("heap starts at addr %p\n"
		   "total size = %d\n"
		   "bytes free = %d\n"
		   "next free byte begins at addr %p\n",
		   heap, heap->size, heap->bytes_free, heap->next_free);
}


int hl_init(void *heapptr, unsigned int heap_size) {

	//Note:  heap_size must be large enough to contain the header

	heap_header_t *heap = (heap_header_t *)heapptr; //cast the void pointer to a pointer to a heap_header_t
	
	heap->size = heap_size; //set the size of the heap
	heap->next_free = ADD_BYTES(heap, sizeof(heap_header_t)); //gets us a pointer to the start of the actual heap
	heap->bytes_free = heap_size - sizeof(heap_header_t); //if this is <0 then there's a problem
	
    #ifdef PRINT_DEBUG
        print_heap(heap);
    #endif

   	
	if(heap->bytes_free < 0){
		return 0;
	}
 
    return 1;

}

void *hl_alloc(void *heapptr, unsigned int block_size) {

	//Note:  if block_size > bytes_free then we got a problem

	heap_header_t *heap = (heap_header_t *)heapptr; //cast the void pointer to a heap_header_t pointer
	void *blockptr = heap->next_free; //point to the next free block
	
	heap->next_free = heap->next_free + block_size; //after allocating the memory, set the next_free to the next block of memory
	heap->bytes_free = heap->bytes_free - block_size; //subtract block_size bytes from the available free memory

	if(heap->bytes_free < 0){
		return 0;
	}
	
	return blockptr; // Success!
	
}

void hl_release(void *heapptr, void *blockptr) {

}

void *hl_resize(void *heapptr, void *blockptr, unsigned int new_block_size) {

	return hl_alloc(heapptr, new_block_size);

}

