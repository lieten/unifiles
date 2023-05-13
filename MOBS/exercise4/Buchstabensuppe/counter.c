#include "counting.h" //nicht aendern!

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUMCORES 4

int alphabet[26]; //nicht aendern!
char *text;
/*
typedef struct params {
	char **text;
	int offset;
} params;
*/
void readFile(const char* file_name/*, char **text*/) {
	//Hier koennte Ihre Loesung stehen.
	FILE *textfile;
	//char *text;
	long numbytes;
	
	textfile = fopen(file_name, "r");
	if(textfile == NULL) {
		printf("upps1");
		return;
	}
	fseek(textfile, 0L, SEEK_END);
	numbytes = ftell(textfile);
	fseek(textfile, 0L, SEEK_SET);
	
	text = (char*)calloc(numbytes, sizeof(char));
	if(text == NULL) {
		printf("upps2");
		return; 
	}

	fread(text, sizeof(char), numbytes, textfile);
	fclose(textfile);
	
	//printf(text);
	//Bis hierhin wird die Datei eingelesen
}

void *function(void *a) {
	/*
	params *input = (params*) a;
	char **textp = input->text;
	char *text = *textp;
	int offset = input->offset;
	*/
	int offset = *((int*) a);
	long textLength;// = strlen(text);
	printf("%d\n", offset);
	for(int i = offset; i < textLength; i += NUMCORES) {
		printf("%c", text[i]);
	}
	printf("\n");
	free(a);
}

void startThread(int offset/*, char **text*/) {
	pthread_t tid;
	/*
	params *args = malloc(sizeof(params));
	args->offset = offset;
	args->text = text;
	*/
	int *param = malloc(sizeof(int));
	*param = offset;
	int success = pthread_create(&tid, NULL, &function, (void*) param);
	printf("Success: %d\n", success);
}

void count(const char* file_name) {
	//char *text;
	//readFile(file_name, &text);
	printf("%d\n", strlen(text));
	readFile(file_name);
	//printf(text);
	//startThread(0, &text);
	startThread(0);
}

int main() {
	count("input.txt");
	return 0;
}
