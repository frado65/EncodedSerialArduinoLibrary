#pragma once
// #include "CArrayofCounters.h"

template<size_t N>
class CArrayofCounters {
private:
    unsigned long (&m_array)[N]; // Riferimento ad un array di unsigned long con dimensione N.

public:
    // Costruttore che accetta un riferimento ad un array di unsigned long.
    CArrayofCounters(unsigned long (&array)[N]) : m_array(array) {
        reset(); // Azzera tutti gli elementi dell'array all'inizializzazione
    }

    // Metodo per incrementare di +1 l'elemento all'indice ix, senza superare il valore massimo.
    unsigned long inc(uint8_t ix) {
        if (ix < N && m_array[ix] < ULONG_MAX) {
            m_array[ix]++;
        }
        return m_array[ix];
    }

    // Metodo per azzerare tutti gli elementi dell'array.
    void reset() {
        for (size_t i = 0; i < N; ++i) {
            m_array[i] = 0;
        }
    }
};