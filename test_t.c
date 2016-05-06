#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "heaplib.h"
#include "speedtask.h"

int main() {

	clock_t begin, end;
	unsigned int num_clocks, num_ops;
	void *heap = malloc(get_heap_size());
	
	printf("Beginning Throughput Test!\n---------------------------\n");
	
	begin = clock();
	// ---------------- timer just started
		
	standalone_speedtask(heap);

	// ---------------- stop the timer now
	end = clock();

	num_clocks = (unsigned int)(end - begin);
	setlocale(LC_NUMERIC, "");
	free(heap);

	num_ops = get_num_ops();
	printf("num_ops: \t\t%10d\n", num_ops); 
	printf("Human_Readable_Clocks:  %'10u\n", num_clocks);
	printf("Total_Clocks: \t\t%10u\n", num_clocks); 
	printf("1K_ops_per_clock: \t%10.2f\n", 1000*get_num_ops()/(double)num_clocks); 
	
	return 0;
}
