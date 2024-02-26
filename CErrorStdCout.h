#pragma once
// UUID-20240210164040
// #include "CErrorStdCout.h"

#include <iostream>
#include <cstdint> // per uint8_t

// Assicurati che IError sia definito da qualche parte nel tuo progetto o includi la sua definizione qui

class CErrorStdCout : public IError {
public:
    // Il costruttore specifico per la porta seriale viene rimosso poiché non è più necessario

    void error(const uint8_t errnum) override {
        std::cout << "Errore: " << +errnum << std::endl; // Usa std::cout per stampare l'errore
    }

    void error(const uint8_t errnum, const char* message) override {
        std::cout << "Errore: " << +errnum << " - " << message << std::endl; // Stampa codice di errore e messaggio
    }
};
