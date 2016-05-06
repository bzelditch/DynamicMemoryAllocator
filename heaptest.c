#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "heaplib.h"

/* This test supports standalone tests. You might want to create and
 * debug tests here before inserting them into your correctness tests
 * or your speed/size tests.
 */ 
void my_test() {

	char heap[100];
	
	hl_init(heap, 100 * sizeof(char));
	
	char* ptr = (char *) hl_alloc(heap, 13 * sizeof(char));
	
	ptr[0] = 'h';
	ptr[1] = 'e';
	ptr[2] = 'l';
	ptr[3] = 'l';
	ptr[4] = 'o';
	ptr[5] = ' ';
	ptr[6] = 'w';
	ptr[7] = 'o';
	ptr[8] = 'r';
	ptr[9] = 'l';
	ptr[10] = 'd';
	ptr[11] = '!';
	ptr[12] = '\0';
	
	printf("%s\n", ptr);

}


int main() {

    my_test();

	return 0;
}

