#pragma once
// #include "CCircularbufferofByte.h"

// Definizione della classe CCircularbufferofByte.
// Questa classe implementa un buffer circolare di byte.
class CCircularbufferofByte {
private:
    byte* const m_buffer; // Puntatore costante al buffer di byte.
    const uint8_t m_bufSize; // Dimensione costante del buffer.
    uint8_t m_head; // Indice dell'elemento più recente nel buffer.
    uint8_t m_tail; // Indice dell'elemento più vecchio nel buffer.
    bool m_isEmpty; // Flag per indicare se il buffer è vuoto.

public:
    // Costruttore che inizializza il buffer con la dimensione specificata.
    CCircularbufferofByte(const uint8_t bufSize) : m_buffer(new byte[bufSize]), m_bufSize(bufSize), m_head(0), m_tail(0), m_isEmpty(true) {}

    // Distruttore che rilascia la memoria allocata per il buffer.
    ~CCircularbufferofByte() {
        delete[] m_buffer;
    }

    // Metodo per aggiungere un byte al buffer.
    void append(const byte inByte) {
        m_buffer[m_head] = inByte;
        m_head = (m_head + 1) % m_bufSize;
        if (m_head == m_tail) { // Se head ha raggiunto tail, il buffer è pieno e tail deve avanzare.
            m_tail = (m_tail + 1) % m_bufSize; // TODO: segnalare errore?
        }
        m_isEmpty = false; // Il buffer ora contiene almeno un elemento.
    }

    // Metodo per verificare se il buffer è vuoto.
    bool isEmpty() const {
        return m_isEmpty;
    }

    // Metodo per ottenere la dimensione attuale del buffer.
    uint8_t getSize() const {
        if (m_isEmpty) {
            return 0; // Se il buffer è vuoto, la dimensione è 0.
        } else if (m_head >= m_tail) {
            return m_head - m_tail; // Se head è maggiore o uguale a tail, la dimensione è la differenza.
        } else {
            return m_bufSize - m_tail + m_head; // Se tail è avanti a head, calcola la dimensione considerando il wrapping.
        }
    }

    // Metodo per ottenere il byte in base all'indice specificato.
    byte getOldest(const uint8_t ix) const {
        if (m_isEmpty || ix >= m_bufSize) { // Se il buffer è vuoto o l'indice è fuori range, ritorna 0.
            return static_cast<byte>(0); // TODO: segnalare errore.
        }
        // Calcola l'indice effettivo del byte richiesto.
        uint8_t effectiveIndex = (m_tail + ix) % m_bufSize;
        return m_buffer[effectiveIndex];
    }

    // Metodo per rimuovere l'elemento più vecchio dal buffer.
    void removeOldest() {
        if (!m_isEmpty) { // Verifica che il buffer non sia vuoto.
            if (m_tail == m_head) { // Se c'è un solo elemento, la rimozione lo rende vuoto.
                m_isEmpty = true;
            }
            m_tail = (m_tail + 1) % m_bufSize; // Aggiorna l'indice di tail al prossimo elemento.
            
            // Se dopo la rimozione head e tail coincidono, il buffer è vuoto.
            m_isEmpty = (m_head == m_tail);
        }
        else{
            // TODO: segnala errore?
        }
    }

    // Metodo per resettare il buffer, opzionalmente riempiendolo con un byte specificato.
    void reset(const byte filler = static_cast<byte>(0)) {
        for (uint8_t i = 0; i < m_bufSize; ++i) {
            m_buffer[i] = filler;
        }
        m_head = 0;
        m_tail = 0;
        m_isEmpty = true;
    }
};
