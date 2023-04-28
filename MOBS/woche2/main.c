#include <stdio.h>
#include <stdlib.h>
#include "guessing.h"


int counter = 0;
int target;
int results[100];

int evaluate(int guess) {
	counter++;
	if (guess < target)
		return 1;
	if (guess > target)
		return -1;
	return 0;
}

int create_random_digit(int lower, int upper){
	int random = rand()%upper+lower;
	return random;
}

int create_random_code(){
	int code = create_random_digit(1,9) * 10000;
	code += create_random_digit(0,9) * 1000;
	code += create_random_digit(0,9) * 100;
	code += create_random_digit(0,9) * 10;
	code += create_random_digit(0,9);
	return code;
}

int main(void) {
	srand(42);
	int i;
	for(i = 0; i < 100; i++){
		counter = 0;
		target = create_random_code();
		int result = guess_the_number();
		printf("Counter: %d\n", counter);
		if(result == target){
			results[i] = counter;
		} else {
			results[i] = 99999;
		}
	}
	int sum = 0;
	for(i = 0; i < 100; i++){
		sum += results[i];
	}
	printf("Average: %d\n", sum/100);
	return EXIT_SUCCESS;
}
