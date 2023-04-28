
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int main(void) {
	pid_t pid;
	int cmdnum;
	char cmd[100], arg[100];
	while(1) {
		printf("Kommando? \n");
		printf("Verfuegbare Kommandos:\n");
		printf("1: ls\n2: ps\n3: cal\n4: exit\n");
		scanf("%d", &cmdnum);
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
				printf("Unzulaessiges Kommando\n");
				continue;
		}

		printf("Argument? type 'no' for no argument\n");
		scanf("%99s", arg);
		pid = fork();
		if(pid > 0) {
			int status;
			if(wait(&status) == pid && WIFEXITED(status))
				printf ("Exit Status: %d\n", WEXITSTATUS(status));
		}
		else if (pid == 0) {
			if(strcmp(arg, "no") == 0) {
				execlp(cmd, cmd, NULL, NULL);
				printf("exec fehlgeschlagen\n");
			}
			else {
				execlp(cmd, cmd, arg, NULL);
				printf("exec fehlgeschlagen\n");
			}
		}
	}
}
