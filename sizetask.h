#ifndef SIZETASK_H
#define SIZETASK_H

/* pass_sizetask tries to perform some task using the heaplib calls
 * with a heap initialized to heap_size. Returns TRUE if all calls to
 * the heaplib functions are successful, returns FALSE if a single
 * call to a heaplib function fails.
 */
bool pass_sizetask(unsigned int heap_size);

#endif /*SIZETASK_H*/
