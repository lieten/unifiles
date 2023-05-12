#include "counting.h" //nicht aendern!

#include <stdio.h>
#include <stdlib.h>

int alphabet[26]; //nicht aendern!

void count(const char* file_name){ // Name der Inputdatei
	//Hier koennte Ihre Loesung stehen.
	FILE *textfile;
	char *text;
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
	
	printf(text);
	//Bis hierhin wird die Datei eingelesen
}

int main () {
	count("input.txt");
	return 0;
}
