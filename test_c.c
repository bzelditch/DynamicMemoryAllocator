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
#define SMALL_HEAP_SIZE 32
#define ARRAY_LEN 16

int num_tests = 0;



int resize_copies_values() {

    int reference_nums[ARRAY_LEN] = { 15, 51, 76, 90, 101, 199, 23, 98, 2, 6, 23, 98, 2, 6, 34, 23 };
    char heap[DECENT_HEAP_SIZE]; 
    int *small_array, *bigger_array;
    int small_len = 6, bigger_len = 10;
    int i;

	num_tests++; // begin each test by incrementing this counter
	printf("%d) Resize copies values?", num_tests); 

	hl_init(heap, DECENT_HEAP_SIZE);
	
	small_array = hl_alloc(heap, small_len * sizeof(int));
	
	for (i = 0; i < small_len; i++) {
	   small_array[i] = reference_nums[i];
	}
	
	bigger_array = hl_resize(heap, small_array, bigger_len * sizeof(int));
	
	if (bigger_array) {
		for (i = 0; i < small_len; i++) {
			if(bigger_array[i] != reference_nums[i]) {
				printf("\t\t\tFAIL\n"); 
				return FAIL;
			}
		}
	}
	printf("\t\t\tPASS\n");
	return SUCCESS;  	
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

/**
 * Test for failures in incorrect alloc call
 */
int alloc_basic() {
	char array[DECENT_HEAP_SIZE]; 
	void *block;
	bool aligned_f = false;
	num_tests++; // begin each test by incrementing this counter
	hl_init(&array, DECENT_HEAP_SIZE - 1);
	block = hl_alloc(&array, 150);
	aligned_f = block != 0;

	printf("%d) hl_alloc can alloc a basic block?", num_tests); 
	if (aligned_f) {
		printf("\t\tPASS\n");
		return SUCCESS;
	}
	
	printf("\t\tFAIL\n"); 
	return FAIL;
}

/**
 * Test for failures in incorrect alloc call
 */
int alloc_failure() {
	char array[DECENT_HEAP_SIZE]; 
	void *block;
	bool aligned_f = false;
	num_tests++; // begin each test by incrementing this counter
	hl_init(&array, DECENT_HEAP_SIZE - 1);
	block = hl_alloc(&array, 500);
	aligned_f = block != 0;

	printf("%d) hl_alloc catches failure case?", num_tests); 
	if (!aligned_f) {
		printf("\t\tPASS\n");
		return SUCCESS;
	}
	
	printf("\t\tFAIL\n"); 
	return FAIL;
}

/**
 * Test for failures in incorrect init call
 */
int init_failure() {
	char array[DECENT_HEAP_SIZE]; 
	bool failure = false;
	int size = 4;
	num_tests++; // begin each test by incrementing this counter
	failure = hl_init(&array, size);

	printf("%d) hl_init catches failure case?", num_tests); 
	if (!failure) {
		printf("\t\tPASS\n");
		return SUCCESS;
	}
	printf("\t\tFAIL\n"); 
	return FAIL;
}

/**
 * Test for failures in incorrect resize call
 */
int resize_failure() {
	char array[DECENT_HEAP_SIZE]; 
	void *block;
	void *resized;
	bool failure = false;


	num_tests++; // begin each test by incrementing this counter

	hl_init(&array, DECENT_HEAP_SIZE - 1);
	block = hl_alloc(&array, 11);
	resized = hl_resize(&array, block, 500);
	failure = resized != 0;

	printf("%d) hl_resize catches failure case?", num_tests); 
	if (!failure) {
		printf("\t\tPASS\n");
		return SUCCESS;
	}
	printf("\t\tFAIL\n"); 
	return FAIL;
}

int heap_overflow() {
	char heap[DECENT_HEAP_SIZE]; 
	char *letters; 
	void *new_letters;

	num_tests++; // begin each test by incrementing this counter

	hl_init(heap, DECENT_HEAP_SIZE);
	letters = (char *) hl_alloc(heap, 6 * sizeof(char));

	letters[0] = 'h';
	letters[1] = 'e';
	letters[2] = 'l';
	letters[3] = 'l';
	letters[4] = 'o';
	letters[5] = '\0';

	new_letters = hl_alloc(heap, 250 * sizeof(char));
	printf("%d) Heap doesn't overflow with alloc call?", num_tests); 
	if (new_letters == 0) {
		printf("\tPASS\n");
		return SUCCESS;
	}
	
	printf("\tFAIL\n"); 
	return FAIL;
}

int release_test() {
	char heap[DECENT_HEAP_SIZE]; 
	char *letters; 
	void *new_letters;

	num_tests++; // begin each test by incrementing this counter

	hl_init(heap, DECENT_HEAP_SIZE);
	letters = (char *) hl_alloc(heap, 150 * sizeof(char));

	letters[0] = 'h';
	letters[1] = 'e';
	letters[2] = 'l';
	letters[3] = 'l';
	letters[4] = 'o';
	letters[5] = '\0';

	hl_release(heap, letters);
	new_letters = hl_alloc(heap, 150 * sizeof(char));

	printf("%d) Heap correctly allocates after release?", num_tests); 
	if (new_letters != 0) {
		printf("\tPASS\n");
		return SUCCESS;
	}
	
	printf("\tFAIL\n"); 
	return FAIL;
}

int main() {
	
	int num_passes = 0;
	
	printf("Beginning Correctness Test!\n---------------------------\n");
	
	num_passes += resize_copies_values();//fails
	num_passes += alloc_ptr_aligned();
	num_passes += alloc_failure();
	num_passes += alloc_basic();
	num_passes += init_failure();
	num_passes += resize_failure();
	num_passes += heap_overflow();
	num_passes += release_test();
	
	printf("Num_Passed_Tests: %d\n", num_passes);
	printf("Num_Tests: %d\n", num_tests);
	printf("Pass_Rate: %.02f\n", num_passes/(double)num_tests);
	
	return 0;
}
