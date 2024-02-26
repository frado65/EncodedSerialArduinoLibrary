#pragma once
// UUID-20240210164040
// #include "CErrorLed.h"

//#include <Arduino.h>

class CErrorLed : public IError {
private:
  uint8_t ledPin; // Pin a cui è collegato il LED
  
public:
  CErrorLed(uint8_t pin) : ledPin(pin) {
    pinMode(ledPin, OUTPUT); // Imposta il pin del LED come output
  }

  void error(const uint8_t errnum) override {
    digitalWrite(ledPin, HIGH); // Accende il LED per segnalare un errore
    // Potrebbe anche lampeggiare il LED errnum volte per indicare il codice di errore
  }

  void error(const uint8_t errnum, const char* message) override {
    digitalWrite(ledPin, HIGH); // Usa il LED per segnalare un errore
    // In questo caso, ignora il messaggio poiché non può visualizzarlo
  }
};