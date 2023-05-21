#include "counting.h" //nicht aendern!

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//Anzahl der Cores, bestimmt wie viele Threads verwendet werden
#define NUMCORES 4

int alphabet[26]; //nicht aendern!

//Die gelesene Textdatei
char *text;

//Array an Alphabeten, die die Threads benutzen
int *localAlphabetArray[NUMCORES];
//Laenge des gelesenen Textes
int textLength;

void readFile(const char* file_name) {
	FILE *textfile;
	long numbytes;
	
	textfile = fopen(file_name, "r");
	if(textfile == NULL) {
		printf("Error reading file");
		return;
	}
	fseek(textfile, 0L, SEEK_END);
	numbytes = ftell(textfile);
	fseek(textfile, 0L, SEEK_SET);
	
	text = (char*)calloc(numbytes, sizeof(char));
	if(text == NULL) {
		printf("Error reading file");
		return; 
	}

	fread(text, sizeof(char), numbytes, textfile);
	fclose(textfile);
}

void *subcount(void *a) {
    //Der Anfangsoffset
    int offset = *(int*) a;
    //Lokales Alphabet, auf dem die vom Thread gelesenen Buchstaben gezaehlt werden
    int *localAlphabet = (int*)malloc(26*sizeof(int));

    //Jeder Thread springt abhaengig von der Threadanzahl ueber den Text
	for(int i = offset; i < textLength; i += NUMCORES) {
        //Durch Ascii-Wert Platz im Array berechnen, siehe Ascii-Tabelle
        int asciiValue = text[i] - 65;
        if(asciiValue > 25) {
            asciiValue -= 32;
        }
        if(asciiValue <= 25 && asciiValue >= 0) {
            localAlphabet[asciiValue]++;
        }
	}
    //Lokales Alphabet in globale Liste der lokalen Alphabete aufnehmen
    localAlphabetArray[offset] = localAlphabet;
	free(a);
}

void count(const char* file_name) {
	readFile(file_name);
    pthread_t tidArray[NUMCORES];
    textLength = strlen(text);

	for(int i = 0; i < NUMCORES; i++) {
        int *param = malloc(sizeof(int));
        *param = i;
        //Thread mit Offset starten
        pthread_create(&tidArray[i], NULL, &subcount, (void*) param);
    }

    //Warten, bis alle Threads ferting sind
    for(int i = 0; i < NUMCORES; i++) {
        pthread_join(tidArray[i], NULL);
    }

    //Ergebnisse aus lokalen Alphabeten zusammentragen
    for(int i = 0; i < NUMCORES; i++) {
        for(int j = 0; j < 26; j++) {
            alphabet[j] += localAlphabetArray[i][j];
        }
    }
}