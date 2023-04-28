#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *countdown(void *timep) {
	int time = *((int*) timep);
	while (time) {
		time--;
		//printf("%d", time);
		sleep(1);
	}
	printf("Hier könnte ihr Alarm stehen \n");
}

void createThread(int time) {
	pthread_t tid;
	int success = pthread_create(&tid, NULL, &countdown, (void*) &time);
	printf("Countdown mit Zeit %d gestartet \n", time);
}

int main(void) {
	int time;
	printf("Nenne eine Anfangszahl");
	while(1) {
		//printf("Nenne eine Zahl ");
		scanf("%d", &time);
		//printf("\n");
		createThread(time);
	}
}
