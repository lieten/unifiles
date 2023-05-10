/*
 * In dieser Datei soll nichts geaendert werden.
 */
#include "counting.h"
#include <sys/time.h>

void print_alphabet() {
	printf("\n");
	for (int i = 0; i < 25; i++) {
		printf("%d,", alphabet[i]);
	}
	printf("%d\n", alphabet[25]);
}

void reset_alphabet() {
	for (int i = 0; i < 26; i++) {
		alphabet[i] = 0;
	}
}

int main(int argc, char *argv[]) {

	char *input_file = "input.txt";

	if (argc > 1) {
		input_file = argv[1];
	}
	printf("File: %s\n", input_file);

	int runs = 10;
	long total_usec = 0;

	for (int i = 0; i < runs; i++) {
		struct timeval tval_before, tval_after;

		gettimeofday(&tval_before, NULL);
		count(input_file);
		gettimeofday(&tval_after, NULL);
		print_alphabet();
		reset_alphabet();

		long usec_diff = (tval_after.tv_sec - tval_before.tv_sec) * 1000000
				+ (tval_after.tv_usec - tval_before.tv_usec);
		printf("Duration: %f\n", (usec_diff / 1000000.0));
		total_usec += usec_diff;
	}
	printf("\nAverage: %f sec. [wallclock time]\n\n", total_usec/1000000.0/runs);
	return 0;
}
