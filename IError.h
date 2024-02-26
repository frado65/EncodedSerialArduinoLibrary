#pragma once
// UUID-20240210164040
// #include "IError.h"

class IError {
public:
  // Metodo virtuale puro per segnalare un errore solo con il 
  // codice di errore.
  virtual void error(const uint8_t errnum) = 0;

  // Metodo virtuale puro per segnalare un errore con codice e 
  // messaggio di errore.
  virtual void error(const uint8_t errnum, const char* message) = 0;

  // Distruttore virtuale per garantire una corretta deallocazione 
  // delle risorse delle classi derivate.
  virtual ~IError() {}
};

/**
Versione che non fa nulla (gestione errori disabilitata).
In AppError.h => ErrorSerial __g_err = CErrorNone();
*/
class CErrorNone : public IError {
  public:
    void error(const uint8_t errnum) override {
    }

    void error(const uint8_t errnum, const char* message) override {
    }
};