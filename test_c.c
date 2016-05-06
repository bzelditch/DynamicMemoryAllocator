#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "heaplib.h"

#define SUCCESS 1
#define FAIL 0

// when testing for correctness make heap nice and big
// don't want bloated meta data to be the problem
#define DECENT_HEAP_SIZE 256

int num_tests = 0;

int resize_copies_values() {

	char heap[DECENT_HEAP_SIZE]; 
	char *letters, *new_letters;
	
	num_tests++; // begin each test by incrementing this counter
	
	hl_init(heap, DECENT_HEAP_SIZE);
	letters = (char *) hl_alloc(heap, 6 * sizeof(char));
	
	letters[0] = 'h';
	letters[1] = 'e';
	letters[2] = 'l';
	letters[3] = 'l';
	letters[4] = 'o';
	letters[5] = '\0';
	
	new_letters = hl_resize(heap, letters, 20 * sizeof(char));
	
	printf("%d) Resize copies values?", num_tests); 
	if (!strcmp(letters, new_letters)) {
		printf("\t\t\tPASS\n");
		return SUCCESS;
	}
	
	printf("\t\t\tFAIL\n"); 
	return FAIL;
	
}

/* Note: this test may or may not be robust. Take a look and determine
 * whether it thoroughly tests what it is trying to test.
 */
int alloc_ptr_aligned() {

	char array[DECENT_HEAP_SIZE]; 
	void *block;
	bool aligned_f = false;

	num_tests++; // begin each test by incrementing this counter

	hl_init(&array, DECENT_HEAP_SIZE - 1);
	
	block = hl_alloc(&array, 11);

	// you may find this helpful. feel free to remove
    #ifdef PRINT_DEBUG
    	printf("blockptr = %16lx\n", (unsigned long)block);
    	printf("mask =     %16lx\n", (unsigned long)(ALIGNMENT -1));
    	printf("---------------------------\n");
    	printf("ANDED =    %16lx\n", (unsigned long)block & (ALIGNMENT - 1));
    	printf("!ANDED (ALIGNED) =   %6d\n", !((unsigned long)block & (ALIGNMENT - 1)));
    #endif

	aligned_f = !((unsigned long)block & (ALIGNMENT - 1));

	printf("%d) hl_alloc return ptr has correct alignment?", num_tests); 
	if (aligned_f) {
		printf("\tPASS\n");
		return SUCCESS;
	}
	
	printf("\tFAIL\n"); 
	return FAIL;
	
}

int main() {
	
	int num_passes = 0;
	
	printf("Beginning Correctness Test!\n---------------------------\n");
	
	num_passes += resize_copies_values();
	num_passes += alloc_ptr_aligned();
	
	
	printf("Num_Passed_Tests: %d\n", num_passes);
	printf("Num_Tests: %d\n", num_tests);
	printf("Pass_Rate: %.02f\n", num_passes/(double)num_tests);
	
	return 0;
}
