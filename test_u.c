#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "heaplib.h"
#include "sizetask.h"

// DO NOT MODIFY THIS FILE!

#define MIN_HEAP_SIZE       512
#define MAX_HEAP_SIZE  16777216
#define GRANULARITY         128

/* 
 * Finds the first heap size that successfully completes the task
 */
int find_upperbound() {

    int size = MIN_HEAP_SIZE;
	
    while (size < MAX_HEAP_SIZE) {
		if(pass_sizetask(size)) {			
            #ifdef PRINT_DEBUG
			    printf("upper bound = %d bytes\n", size);
            #endif

			return size;
		}
		else {
			size *= 2;
		}
	}
	return MAX_HEAP_SIZE;
}


/*  
 * Finds the best heap size that successfully completes the task
 */
int find_best_size(int upper) {

	int lower = upper/2;
	int size = lower + lower/2; // start searching in the middle

	// let the binary search begin!
	while ((upper - lower) > GRANULARITY) {
		if(pass_sizetask(size)) {
            #ifdef PRINT_DEBUG
			    printf("upper bound = %d bytes\n", size);
            #endif
			upper = size;
		}
		else {
            #ifdef PRINT_DEBUG
			    printf("%d bytes was not enough for the task\n", size);
            #endif
			lower = size;
		}
		size = lower + (upper - lower)/2;
	}
	return upper;
}


/* 
 * The goal is to find the smallest heap size which satisfies the
 * space requirements of the funtion task().  Begin with some minimum
 * heap size and if that isn't enough, double the heap.  When the
 * first successful heap size is found, perform a binary search to
 * find a more specific heap estimate, according to the given
 * GRANULARITY.
 */
int main() {
	
	int size;
	
	printf("Beginning Utilization Test!\n---------------------------\n");
	
	size = find_upperbound();
	
	if (size != MIN_HEAP_SIZE) {
		size = find_best_size(size);
	}

	printf("Required_Size_in_Bytes: %d\n", size);
	return 0;
}
