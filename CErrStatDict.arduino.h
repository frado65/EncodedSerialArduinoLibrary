#pragma once
// #include "CCountersDict.h"


#ifdef ARDUINO
  #include <Arduino.h> // Specifico per Arduino
#else
    #include <cstdint> // Per uint16_t
#endif

// Determina se stiamo compilando per Arduino o per un ambiente standard C++
#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h" // Include la base di Arduino
    // Definisce il tipo per le stringhe di messaggio di errore
    #define ERR_MSG_TYPE const char*
#else
    #include <string>    // Per std::string in ambiente non-Arduino
    #include <cstring>   // Per strlen e strncpy
    #define ERR_MSG_TYPE std::string
#endif

#define MAX_ERRORS 10 // Massimo numero di errori distinti da tracciare
#define MAX_ERR_MSG_LENGTH 50 // Massima lunghezza del messaggio di errore

class CCountersDict {
private:
    struct CErr {
        unsigned long m_cnt;
        uint16_t errNum;
        char errMsg[MAX_ERR_MSG_LENGTH]; // Messaggio di errore come array di caratteri

        // Costruttore per inizializzare un errore
        CErr() : m_cnt(0), errNum(0) {
            errMsg[0] = '\0'; // Inizializza una stringa vuota
        }

        // Imposta l'errore
        void setErr(const UErr& err) {
            m_cnt = 0; // Resetta il conteggio
            errNum = err.getErrNum(); // Imposta il numero di errore
            // Copia il messaggio di errore, assicurandosi di non superare MAX_ERR_MSG_LENGTH
            #if defined(ARDUINO) && ARDUINO >= 100
                strncpy(errMsg, err.getErrMsg(), MAX_ERR_MSG_LENGTH - 1);
            #else
                strncpy(errMsg, err.getErrMsg(), MAX_ERR_MSG_LENGTH - 1);
            #endif
            errMsg[MAX_ERR_MSG_LENGTH - 1] = '\0'; // Garantisce la terminazione della stringa
        }
    };

    CErr m_counters[MAX_ERRORS]; // Array di errori
    int m_numErrors = 0; // Numero attuale di errori tracciati

public:
    unsigned long inc(const UErr& err) {
        for (int i = 0; i < m_numErrors; i++) {
            if (m_counters[i].errNum == err.getErrNum()) {
                return ++m_counters[i].m_cnt; // Incrementa e ritorna il conteggio
            }
        }

        // Se l'errore non è presente, aggiungilo se c'è spazio
        if (m_numErrors < MAX_ERRORS) {
            m_counters[m_numErrors].setErr(err);
            return ++m_counters[m_numErrors++].m_cnt; // Incrementa il conteggio e il numero di errori, poi ritorna il conteggio
        }

        return 0; // Se non c'è spazio, potresti voler gestire diversamente
    }

    void reset() {
        m_numErrors = 0; // Semplicemente resetta il numero di errori per "pulire" l'array
    }

    // La funzione toString necessita di una gestione diversa in base alla piattaforma
    #if defined(ARDUINO) && ARDUINO >= 100
    void toString(char* buffer, size_t bufferLength) {
        buffer[0] = '\0'; // Inizializza il buffer come stringa vuota
        for (int i = 0; i < m_numErrors; i++) {
            char line[MAX_ERR_MSG_LENGTH + 20]; // Buffer temporaneo per una singola linea
            snprintf(line, sizeof(line), "%u: %lu, Error: %s\n", m_counters[i].errNum, m_counters[i].m_cnt, m_counters[i].errMsg);
            // Aggiunge la linea al buffer principale, controllando di non superare bufferLength
            if (strlen(buffer) + strlen(line) < bufferLength) {
                strncat(buffer, line, bufferLength - strlen(buffer) - 1);
            } else {
                break; // Esce se non c'è più spazio
            }
        }
    }
    #else
    std::string toString() {
        std::string result;
        for (int i = 0; i < m_numErrors; i++) {
            result += std::to_string(m_counters[i].errNum) + ": " + std::to_string(m_counters[i].m_cnt) + ", Error: " + m_counters[i].errMsg + "\n";
        }
        return result;
    }
    #endif
};

