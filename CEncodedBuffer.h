#pragma once
// #include "CEncodedBuffer.h"
//#include <stdint.h> // Per uint8_t

#include "UErr.h"

class CEncodedBuffer {
  private:
    const byte m_escapeByte;
    const byte m_startByte;
    const byte m_stopByte;
    const bool m_addCnt;
    const uint8_t m_buffer_maxLen;
    byte *const m_buffer; // Il puntatore è costante, i dati puntati no.
    const UErr m_errTryToAppendOnClosedBuffer;
    const UErr m_errTryToAppendOnFullBuffer;
    const UErr m_errGetOnIxOutOfBounds;
    const uint16_t m_errFree;
    uint8_t m_ix = 0;
    uint8_t m_cnt = 0;
    bool m_closed;
    const UErr* m_error = &UErr::NoErr;

  public:
    CEncodedBuffer(
      uint8_t errBase, 
      byte escapeByte, 
      byte startByte, 
      byte stopByte, 
      bool addCnt, 
      uint8_t buffer_maxLen
      ):
      m_escapeByte(escapeByte), 
      m_startByte(startByte), 
      m_stopByte(stopByte), 
      m_addCnt(addCnt),
      m_buffer_maxLen(buffer_maxLen),
      m_buffer(new byte[buffer_maxLen]),
      m_errTryToAppendOnClosedBuffer(errBase+0,"TryToAppendOnClosedBuffer"),
      m_errTryToAppendOnFullBuffer(errBase+1,"TryToAppendOnFullBuffer"),
      m_errGetOnIxOutOfBounds(errBase+2,"GetOnIxOutOfBounds"),
      m_errFree(errBase+3)
    {
        m_closed = false;
        m_buffer[m_ix++] = startByte;
    }

    ~CEncodedBuffer() {
        delete[] m_buffer; // Distruttore per liberare la memoria allocata
    }

    uint16_t getFreeErr() const {
      return m_errFree;
    }

    const UErr& getError() const {
      return *m_error;
    }

    bool isError() const {
      return m_error->isError();
    }

    void reset() {
      m_ix = 0;
      m_cnt = 0;
      m_closed = false;
      m_error = &UErr::NoErr;
      m_buffer[m_ix++] = m_startByte;
    }

    void append(byte data) {
      if(m_error->isError()) return;

      if(m_closed) {
        m_error = &m_errTryToAppendOnClosedBuffer; // errorHandler.error(m_errBase+1, "append: tentativo di append su buffer chiuso.");
        return; // Uscire immediatamente se il buffer è chiuso.
      }
      
      m_cnt++;

      // Gestione dei byte speciali che richiedono l'escape character.
      if (data == m_escapeByte || data == m_startByte || data == m_stopByte) {
        if (m_ix + 1 >= m_buffer_maxLen) { 
          m_error = &m_errTryToAppendOnFullBuffer; 
          return; 
        }
        m_buffer[m_ix++] = m_escapeByte; // Inserisce escape character.
        m_buffer[m_ix++] = data; // Inserisce il dato.
        return; // Uscire dopo l'aggiunta per evitare di aggiungere nuovamente `data`.
      }

      // Se `data` non è un byte speciale, controlla lo spazio rimanente.
      if (m_ix >= m_buffer_maxLen) {
        m_error = &m_errTryToAppendOnFullBuffer; 
        return; // Uscire se non c'è spazio per ulteriori dati.
      }
      
      m_buffer[m_ix++] = data; // Inserisce il dato se non è un caso speciale e c'è spazio.
    }

    void appendStop() {
      if (m_error->isError()) return;
      if(m_addCnt) z_addCnt();
      if (m_ix >= m_buffer_maxLen) { // Controlla lo spazio per il carattere di stop
        m_error = &m_errTryToAppendOnFullBuffer; 
      }
      else{
        m_buffer[m_ix++] = m_stopByte;
      }
    }

    uint8_t getSize() const {
        return m_ix;
    }

    byte getByte(uint8_t index) {
      if (index < m_ix) { // Assicura che l'indice sia nel range dei dati inseriti
        return m_buffer[index];
      }
      m_error = &m_errGetOnIxOutOfBounds; 
      return static_cast<byte>(0); // Placeholder per indice non valido
    }

  private:
    void z_addCnt() {
      if (m_ix >= m_buffer_maxLen) { // Controlla lo spazio per il carattere di stop
        m_error = &m_errTryToAppendOnFullBuffer; 
      }
      else{
        m_buffer[m_ix++] = static_cast<byte>(m_cnt);
      }
    }
};
