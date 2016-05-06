#ifndef SPEEDTASK_H
#define SPEEDTASK_H

unsigned int get_heap_size();

unsigned int get_num_ops();

/* standalone_speedtask performs a task using the heaplib functions.
 * The heap that should be given to hl_init has already been allocated
 * so as not to have a 'real' call to malloc incorporated into the runtime
 * of the task.
 * One call to this function should take enough time that it is 
 * measurable by gprof
 */
void standalone_speedtask(void *heap);

#endif /*SPEEDTASK_H*/
