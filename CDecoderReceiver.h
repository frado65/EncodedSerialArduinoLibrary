#pragma once
// #include "CDecoderReceiver.h"

#ifdef ARDUINO
  #include <Arduino.h> // Specifico per Arduino
#else
  #include <cstdint> // per uint8_t, uint16_t
  //#include <cstddef> // Per std::size_t
  using byte = std::byte;
#endif

#include "IError.h"
#include "CDecodedBuffer.h"
#include "CCircularbufferofByte.h"
#include "IErrStat.h"
//#include ".h"
//#include ".h"

class CDecoderReceiver {
  private:
    CDecodedBuffer m_decodedBuffer;
    CCircularbufferofByte m_circBuffer;
    IError& m_error;
    IErrStat& m_errStat;
  public:
    CDecoderReceiver(
        IError& error,
        IErrStat& errStat,
        const uint8_t bufferMaxLen = 50,
        const bool verifyCntByte = true,
        const byte escapeByte = static_cast<byte>(0),
        const byte startByte = static_cast<byte>(1),
        const byte stopByte = static_cast<byte>(2)
      ) 
    : m_error(error)
    , m_errStat(errStat)
    , m_decodedBuffer(200, escapeByte, startByte, stopByte, verifyCntByte, bufferMaxLen)
    , m_circBuffer(bufferMaxLen)
    {
      this->reset();
    }

    void reset() {
      m_decodedBuffer.reset();
      m_circBuffer.reset();
    }

    void appendByte(byte recivedByte) {
      m_circBuffer.append(recivedByte);

      while(!m_circBuffer.isEmpty()) {
        const UErr *_error = &UErr::NoErr;
        m_decodedBuffer.reset();

        for(uint8_t i=0; i<m_circBuffer.getSize(); i++) {
          const byte _byte = m_circBuffer.getOldest(i);
          m_decodedBuffer.append(_byte);
          if (m_decodedBuffer.isStopOrError()) {
            break; 
          }
        }
        _error = &m_decodedBuffer.getError();
        if (_error->isError()) {
          if(m_errStat.inc(*_error) < 5) {
            m_error.error(_error->getErrNum(), _error->getErrMsg());
          }
          m_circBuffer.removeOldest();
        }
        else if (m_decodedBuffer.isStop()) {
          //z_messageReceived(expectedValue, m_decodedBuffer.getBuffer());
          m_circBuffer.reset();
        }
        else{
          break; // message is not complete, need more data.
        }
      }//while;
    }

    bool isMessageAvailable() const {
      return m_decodedBuffer.isReady();
    }

    const byte * getMessage() const {
      return m_decodedBuffer.getBuffer();
    }
};

