#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
/*
 * Programm akzeptiert eine Zahl, wartet so lange in Sekunden,
 * und loest dann einen Alarm aus.
 */
void clearBuffer() {
	int c;
	//Loesche character vom Inputbuffer, bis newline gefunden
	while((c = getchar()) != '\n'){};
}

void *countdown(void *timep) {
	int time = *((int*) timep);
	do{
		//Hier auskommentieren, um Zeit auf Konsole ausgeben zu lassen
		//printf("%d\n", time);

		sleep(1);
	} while(--time);
	printf("Hier könnte ihr Alarm stehen.\n");
}

void createThread(int time) {
	pthread_t tid;
	//Starte Thread mit Countdown methode, der die Zeit uebergeben wird
	int success = pthread_create(&tid, NULL, &countdown, (void*) &time);
	printf("Countdown mit Zeit %d gestartet.\n", time);
}

int main(void) {
	int time, inputcheck;
	printf("Nenne eine Anfangszahl: ");
	while(1) {
		inputcheck = scanf("%d", &time);
		if(inputcheck < 1) { //Wenn Eingabe keine Zahl oder Zahl zu gross
			printf("Das war keine Zahl (oder die Zahl war zu lang).\n");
			clearBuffer();
		}
		else {
			createThread(time);
		}
	}
}

