#pragma once
// UUID-20240210164040
// #include "UErr.h"

#ifdef ARDUINO
  #include <Arduino.h> // Specifico per Arduino
#else
  #include <cstdint> // per uint8_t, uint16_t
  //#include <cstddef> // Per std::size_t
  //using byte = std::byte;
#endif

class UErr {
public:
    static const UErr NoErr;

    // Costruttore che inizializza i membri dati costanti
    UErr(const uint16_t errNum, const char* const errMsg)
        : m_errNum(errNum), m_errMsg(errMsg) {}

    /**
     * 
    */
    bool isError() const {
      return this->m_errNum != UErr::NoErr.m_errNum;
    }

    // Metodo getter per ottenere il numero dell'errore
    // Marcato come const perché non modifica lo stato dell'oggetto
    uint16_t getErrNum() const {
        return m_errNum;
    }

    // Metodo getter per ottenere il messaggio dell'errore
    // Marcato come const perché non modifica lo stato dell'oggetto
    const char* getErrMsg() const {
        return m_errMsg;
    }

private:
    // Membri dati costanti
    const uint16_t m_errNum;
    const char* const m_errMsg;
};

// Definizione e inizializzazione dell'oggetto statico fuori dalla classe
const UErr UErr::NoErr = UErr(0, "No error");
