#pragma once
// #include "CDecodedBuffer.h"
//#include <stdint.h> // Per uint8_t

#include "UErr.h"

class CDecodedBuffer {
  private:
    const byte m_escapeByte;
    const byte m_startByte;
    const byte m_stopByte;
    const bool m_verifyCntByte;
    const uint8_t m_buffer_maxLen;
    const UErr m_errTryToAppendBeforeStart;
    const UErr m_errTryToAppendStopOnClosedBuffer;
    const UErr m_errTryToAppendOnFullBuffer;
    const UErr m_errGetOnIxOutOfBounds;
    const UErr m_errLenMsgWrong;
    const uint16_t m_errFree;
    byte *const m_buffer;
    int m_ix = 0;
    bool m_escMode = false;
    bool m_startFound = false;
    bool m_stopFound = false;
    const UErr * m_error = &(UErr::NoErr);

  public:
    CDecodedBuffer(
      const uint8_t errBase, 
      const byte escapeByte, 
      const byte startByte, 
      const byte stopByte, 
      const bool verifyCntByte,
      const uint8_t buffer_maxLen
      ):
      m_escapeByte(escapeByte), 
      m_startByte(startByte), 
      m_stopByte(stopByte), 
      m_verifyCntByte(verifyCntByte), 
      m_buffer_maxLen(buffer_maxLen), 
      m_buffer(new byte[buffer_maxLen]), 
      m_errTryToAppendBeforeStart(errBase+0, "TryToAppendBeforeStart"),
      m_errTryToAppendStopOnClosedBuffer(errBase+1, "TryToAppendStopOnClosedBuffer"),
      m_errTryToAppendOnFullBuffer(errBase+2, "TryToAppendOnFullBuffer"),
      m_errGetOnIxOutOfBounds(errBase+3, "GetOnIxOutOfBounds"),
      m_errLenMsgWrong(errBase+4, "LenMsgWrong"),
      m_errFree(errBase+5)
    {
      // if(errBase<=0) {
      //   m_error = 255;
      // }
    }

    ~CDecodedBuffer() {
        delete[] m_buffer;
    }

    const byte * getBuffer() const {
      return m_buffer;
    }

    uint16_t getFreeErr() const {
      return m_errFree;
    }

    const UErr& getError() const {
      return *m_error;
    }

    void reset() {
      m_ix = 0;
      m_escMode = false;
      m_startFound = false;
      m_stopFound = false;
      m_error = &(UErr::NoErr);
    }

    void append(byte data) {
      if(m_error->isError()) return;

      if (m_stopFound) {
        m_error = &m_errTryToAppendStopOnClosedBuffer; 
        return; // Uscire immediatamente se il buffer è "chiuso".
      }

      if (m_escMode) {
        if (!m_startFound) {
          m_error = &m_errTryToAppendBeforeStart;
        }
        else if (m_ix >= m_buffer_maxLen) {
          m_error = &m_errTryToAppendOnFullBuffer;
        }
        else{
          m_buffer[m_ix++] = data;
        }
        m_escMode = false;
      }
      else if (data == m_escapeByte) {
        m_escMode = true;
      }
      else if (data == m_startByte && !m_startFound) {
        m_startFound = true;
      }
      else if (data == m_stopByte && m_startFound) {
        m_stopFound = true;
        if (m_verifyCntByte) {
          z_verifyCntByte(m_buffer[--m_ix]);// last byte is cnt;
        }
      }
      else if (!m_startFound) {
        m_error = &m_errTryToAppendBeforeStart;
      }
      else if (m_ix >= m_buffer_maxLen) {
        m_error = &m_errTryToAppendOnFullBuffer;
      }
      else{
        m_buffer[m_ix++] = data;
      }
    }

    bool isStop() const {
      return m_stopFound;
    }

    bool isError() const {
      return m_error->isError();
    }

    bool isStopOrError() const {
      return m_stopFound || m_error->isError();
    }

    bool isReady() const {
      return m_stopFound && !m_error->isError();
    }

    uint8_t getSize() const {
        return m_ix;
    }

    byte getByte(uint8_t index) {
        if (index < m_ix) {
            return m_buffer[index];
        }
        m_error = &m_errGetOnIxOutOfBounds;
        return static_cast<byte>(0); // Placeholder per indice non valido
    }
  
  private:
    void z_verifyCntByte(byte cnt) {
      if(static_cast<byte>(m_ix)!=cnt) {
        m_error = &m_errLenMsgWrong; // il byte di verifica della lunghezza è SBAGLIATO!
      }
    }
};

