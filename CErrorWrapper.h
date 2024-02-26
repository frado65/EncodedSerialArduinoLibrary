#pragma once
// UUID-20240210164040
// #include "CErrorWrapper.h"

class CErrorWrapper : public IError {
private:
    IError** handlers; // Array di puntatori a IError
    byte numHandlers;  // Numero di handler gestiti

public:
    // Costruttore che accetta un array di puntatori a IError e il numero di handler
    CErrorWrapper(IError** errorHandlers, uint8_t count) : handlers(errorHandlers), numHandlers(count) {}

    // Implementazione dell'interfaccia IError
    void error(const uint8_t errnum) override {
        for (uint8_t i = 0; i < numHandlers; i++) {
            if (handlers[i] != nullptr) {
                handlers[i]->error(errnum);
            }
        }
    }

    void error(const uint8_t errnum, const char* message) override {
        for (uint8_t i = 0; i < numHandlers; i++) {
            if (handlers[i] != nullptr) {
                handlers[i]->error(errnum, message);
            }
        }
    }
};