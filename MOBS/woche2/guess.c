#include "guessing.h"

int guess_the_number() {
	/*
	 * Idee: wir naehern uns von der Mitte aus in Halbschritten der gesuchten Zahl.
	 */
	int currentGuess = 50000; //Anfangen in der Mitte von 5-stelligen Zahlen
	int valueAdjust = currentGuess; //mit Startwert initialisieren, da im ersten Durchlauf bereits halbiert wird
	int higherOrLower = evaluate(currentGuess);
	while(higherOrLower) {
		valueAdjust = valueAdjust / 2;
		if(!valueAdjust) { //Verhindert, dass der Wert durch Abrundung zu 0 wird.
			valueAdjust++;
		}
		if(higherOrLower < 0) {
			currentGuess -= valueAdjust;
		}
		else {
			currentGuess += valueAdjust;
		}
		higherOrLower = evaluate(currentGuess);
	}
	return currentGuess; //Return the correct number
}
