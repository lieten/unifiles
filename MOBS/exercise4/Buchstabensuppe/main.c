/*
 * In dieser Datei soll nichts geaendert werden.
 */
#include "counting.h"
#include <time.h>

void print_alphabet() {
	printf("\n");
	for (int i = 0; i < 25; i++) {
		printf("%d,", alphabet[i]);
	}
	printf("%d\n", alphabet[25]);
}

void reset_alphabet(){
	for(int i = 0; i < 26; i++){
		alphabet[i] = 0;
	}
}

int main(int argc, char *argv[]) {

	char* input_file = "input.txt";

	if(argc > 1){
		input_file = argv[1];
	}
	printf("File: %s\n", input_file);

	int runs = 10;
	double total_time = 0;

	for (int i = 0; i < runs; i++) {

		clock_t begin = clock();
		count(input_file);
		clock_t end = clock();
		print_alphabet();
		reset_alphabet();
		double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
		printf("Duration: %f\n", time_spent);
		total_time += time_spent;
	}
	printf("\nAverage: %f sec. [processor time]\n\n", total_time/runs);
	return 0;
}
