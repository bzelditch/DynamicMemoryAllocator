#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "heaplib.h"
#include "speedtask.h"

// task specific, feel free to remove these
#define ITERS       4096
#define REQ_SIZE1   48
#define REQ_SIZE2   64

/* NOTICE: this file determines the size of the heap that is created
 * and passed to the standalone_speedtask function via the function get_heap_size
 * whether you choose to have this as a #define or a local variable is
 * totally up to you
 *
 * Since you don't know much about the overhead of the library code
 * written by others, you should be a bit generous with the heap size
 * if your task is making many individual calls to hl_alloc
 */
#define HEAP_SIZE (2 * ITERS * (REQ_SIZE1 + REQ_SIZE2))

// you might not want to #define this if the actual number of 
// operations could vary and you want to keep track as the program runs. 
unsigned int num_ops = 1 + (3 * ITERS);


// however you store/calculate it, you must support this function call
unsigned int get_heap_size() {
	return HEAP_SIZE;
}

// however you store/calculate it, you must support this function call
unsigned int get_num_ops() {
	return num_ops;
}


/* This is just an example task. Replace it with your code.
 * NOTE: your code will test others' library code. If YOUR test segfaults,
 * that will be bad for YOU. So program defensively.
 * 
 * Provide a short description of your task here:
 * 
 */
void standalone_speedtask(void *heap) {
  int i;
  void *ptrs[ITERS];
  
  if (!hl_init(heap, HEAP_SIZE))
	  return;

  for (i = 0; i < ITERS; i++) {
	  ptrs[i] = hl_alloc(heap, REQ_SIZE1);
  }
  
  for (i = 0; i < ITERS; i++) {
	  if (ptrs[i])
		  ptrs[i] = hl_resize(heap, ptrs[i], REQ_SIZE2);
  }
  
  for (i = 1; i < ITERS; i+=2) {
	  if (ptrs[i])
		  hl_release(heap, ptrs[i]);
  }
  
}
