#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void clearInputBuffer() {
	int c;
	//loesche character vom input buffer, bis newline gefunden
	while((c = getchar()) != '\n') {}
}

int main(void) {
	pid_t pid;
	int cmdnum;
	char cmd[100], arg[100];
	int inputcheck;
	while(1) {
		printf("Command? \n");
		printf("Available commands:\n");
		printf("1: ls\n2: ps\n3: cal\n4: exit\n");
		//Zum Ueberpruefen der Eingabe auf Korrektheit
		inputcheck = scanf("%d", &cmdnum);
		
		//Wenn inputcheck < 1 ist die Eingabe ungueltig/keine Zahl/zu grosse Zahl
		if(inputcheck < 1) {
			printf("Invalid input, please try again.\n");
			clearInputBuffer(); //Input buffer muss vorher gecleared werden
			continue; //Wir fangen von Vorne an
		}
		//Ablesen der Zahl und korrektes Kommando setzen
		switch(cmdnum) {
			case 1:
				strcpy(cmd, "ls");
				break;
			case 2:
				strcpy(cmd,"ps");
				break;
			case 3:
				strcpy(cmd, "cal");
				break;
			case 4:
				exit(0);
				break;
			default:
				printf("Input number does not correspond to a listed command. Please try again.\n");
				continue; //Eingabe falsch, wir fangen von Vorne an
		}

		printf("Argument? type 'no' for no argument\n");
		//Zum Ueberpruefen, ob Eingabe zu lang ist
		int position;
		scanf("%99s%n", arg, &position);
		//Wenn die Eingabe zu lang ist, brechen wir ab und clearen den Input buffer
		if(position >= 99) {
			printf("Argument too long, command cancelled. Please try again.\n");
			//clearInputBuffer();
			continue; //Wir fangen von Vorne an
		}
		printf("\n");

		pid = fork();
		if(pid > 0) {
			int status;
			if(wait(&status) == pid && WIFEXITED(status))
			printf ("\nExit Status: %d\n", WEXITSTATUS(status));
		clearInputBuffer();
		}
		else if (pid == 0) {
			//Wenn 'no' eingegeben wurde, ohne Kommando ausfuehren
			if(strcmp(arg, "no") == 0) {
				execlp(cmd, cmd, NULL, NULL);
				printf("exec failed\n");
				clearInputBuffer();
				break; //Notwendig weil Prozess bei Fehlschlagen sonst in while-loop bleibt und man 2 exit commands brauchen wuerde
			}
			//Sonst mit Kommando
			else {
				execlp(cmd, cmd, arg, NULL);
				printf("exec failed\n");
				clearInputBuffer();
				break; //Notwendig weil Prozess bei Fehlschlagen sonst in while-loop bleibt und man 2 exit commands brauchen wuerde

			}
		}
	}
}
