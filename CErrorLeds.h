#pragma once
// UUID-20240210164040
// #include "CErrorLeds.h"

//#include <Arduino.h>

class CErrorLeds : public IError {
private:
    uint8_t* ledPins; // Array dinamico per memorizzare i pin dei LED
    uint8_t numLeds;  // Numero di LED gestiti

public:
	// Costruttore che accetta un array di pin terminato da 0
	CErrorLeds(uint8_t* pins) {
		// Conta quanti pin sono stati passati al costruttore
		numLeds = 0;
		while (pins[numLeds] != 0) {
			numLeds++;
		}
	
		// Alloca e inizializza l'array dei pin dei LED
		ledPins = new uint8_t[numLeds];
		for (uint8_t i = 0; i < numLeds; i++) {
			ledPins[i] = pins[i];
			pinMode(ledPins[i], OUTPUT);
		}
	}
	
	~CErrorLeds() {
		delete[] ledPins;
	}
	
	void error(const uint8_t errnum) override {
    // Verifica se errnum è 0 o superiore al massimo gestibile
    bool turnAllOn = (errnum == 0) || (errnum >= (1 << numLeds));

    for (uint8_t i = 0; i < numLeds; i++) {
      if (turnAllOn || (errnum & (1 << i))) {
        digitalWrite(ledPins[i], HIGH); // Accende il LED
      } else {
        digitalWrite(ledPins[i], LOW); // Spegne il LED
      }
    }
	}
	
	void error(const uint8_t errnum, const char* message) override {
		// Ignora il messaggio e chiama la versione di error() che non lo usa
		error(errnum);
	}
};