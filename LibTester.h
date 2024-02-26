#pragma once
// #include "LibTester.h"

#include <limits.h>

#ifdef ARDUINO
  #include <Arduino.h> // Specifico per Arduino
#else
  #include <cstdint> // per uint8_t, uint16_t
  #include <cstddef> // Per std::size_t
  using byte = std::byte;
  //#define byte uint8_t
#endif

#include "AppError.h"
#include "CEncodedBuffer.h"
#include "CDecodedBuffer.h"
#include "CCircularbufferofByte.h"
#include "CErrStatDict.cplusplus.h"
#include "CDecoderReceiver.h"
//#include "CArrayofCounters.h"

//#define UINT16_MAX 0xFFFF

class LibTester {
  private:
    static constexpr byte escapeByte = static_cast<byte>(0);
    static constexpr byte startByte = static_cast<byte>(1);
    static constexpr byte stopByte = static_cast<byte>(2);
    static constexpr uint8_t m_buffer_maxLen = 50;
    CEncodedBuffer g_encodedBuffer;
    CErrStatDict m_errStatDict; // Necesserio per m_decoderReceiver
    CDecoderReceiver m_decoderReceiver;
    CDecodedBuffer g_decodedBuffer;
    CCircularbufferofByte g_circBuffer;
    const UErr& m_errTestFailed;
    unsigned long m_numOfDecoded;

    byte m_buffer[m_buffer_maxLen];

  public:
    LibTester() : g_encodedBuffer(100, escapeByte, startByte, stopByte, true, 2*m_buffer_maxLen),
                  m_errStatDict(CErrStatDict()), // Necesserio per m_decoderReceiver
                  m_decoderReceiver(g_err, m_errStatDict),
                  //m_decoderReceiver(*IErrStat::DUMMY),
                  g_decodedBuffer(200, escapeByte, startByte, stopByte, true, m_buffer_maxLen),
                  g_circBuffer(m_buffer_maxLen),
                  m_errTestFailed(UErr(1000,"== TEST FAILED! =="))
    {
      m_numOfDecoded = 0;
    }

    void test() {
      const unsigned long maxLoop = 1000000; //<=100000000;
      //test1();
      //test2(3);
      //test2(7);
      //test2(13);
      //test2(17);
      //test2(23);
      //test2(29);
      //test2(31);
      test3(maxLoop, 0);
      test3(maxLoop, 0,0);
      test3(maxLoop, 0,1);
      test3(maxLoop, 0,2);
      test3(maxLoop, 0,3);
      test3(maxLoop, 29);
      test3(maxLoop, 29,0);
      test3(maxLoop, 29,1);
      test3(maxLoop, 29,2);
      test3(maxLoop, 29,3);
      test3(maxLoop, 29, 0, 1, 2, 3);
      test3(maxLoop, 29, 0, 1, 3, 2);
      test3(maxLoop, 29, 0, 2, 1, 3);
      test3(maxLoop, 29, 0, 2, 3, 1);
      test3(maxLoop, 29, 0, 3, 1, 2);
      test3(maxLoop, 29, 0, 3, 2, 1);
      test3(maxLoop, 29, 1, 0, 2, 3);
      test3(maxLoop, 29, 1, 0, 3, 2);
      test3(maxLoop, 29, 1, 2, 0, 3);
      test3(maxLoop, 29, 1, 2, 3, 0);
      test3(maxLoop, 29, 1, 3, 0, 2);
      test3(maxLoop, 29, 1, 3, 2, 0);
      test3(maxLoop, 29, 2, 0, 1, 3);
      test3(maxLoop, 29, 2, 0, 3, 1);
      test3(maxLoop, 29, 2, 1, 0, 3);
      test3(maxLoop, 29, 2, 1, 3, 0);
      test3(maxLoop, 29, 2, 3, 0, 1);
      test3(maxLoop, 29, 2, 3, 1, 0);
      test3(maxLoop, 29, 3, 0, 1, 2);
      test3(maxLoop, 29, 3, 0, 2, 1);
      test3(maxLoop, 29, 3, 1, 0, 2);
      test3(maxLoop, 29, 3, 1, 2, 0);
      test3(maxLoop, 29, 3, 2, 0, 1);
      test3(maxLoop, 29, 3, 2, 1, 0);
    }

    void test1(const unsigned long maxLoop) {
      // Preparazione 
      unsigned long _n;
      g_encodedBuffer.reset();
      for(_n=0; ; _n++) {
        byte * _bufEnd = z_ulongToBytes(_n, m_buffer);
        uint8_t _len = _bufEnd-m_buffer;
        z_execTest(_n, m_buffer, _len);
        if (_n == ULONG_MAX) break;
        if (_n >= maxLoop) break;
      }
    }

    void test2(const unsigned long maxLoop, unsigned int mul) {
      // Preparazione 
      unsigned long _n;
      g_encodedBuffer.reset();
      _n = mul;
      for(unsigned long _cnt = 0; _cnt<maxLoop; _cnt++) {
        _n = _n * mul;
        byte * _bufEnd = z_ulongToBytes(_n, m_buffer);
        uint8_t _len = _bufEnd-m_buffer;
        z_execTest(_n, m_buffer, _len);
      }
    }

    void test3(
      const unsigned long maxLoop, 
      const unsigned int mul, 
      const uint16_t b1=UINT16_MAX, 
      const uint16_t b2=UINT16_MAX, 
      const uint16_t b3=UINT16_MAX, 
      const uint16_t b4=UINT16_MAX
      ) {
      std::cout << " TEST3: " 
              << "mul = " << mul << ", "
              << "b1 = " << (b1 == UINT16_MAX ? "X" : std::to_string(b1)) << ", "
              << "b2 = " << (b2 == UINT16_MAX ? "X" : std::to_string(b2)) << ", "
              << "b3 = " << (b3 == UINT16_MAX ? "X" : std::to_string(b3)) << ", "
              << "b4 = " << (b4 == UINT16_MAX ? "X" : std::to_string(b4))
              << std::endl;
      // Preparazione 
      unsigned long _n;
      g_circBuffer.reset();
      g_encodedBuffer.reset();
      _n = mul;
      m_numOfDecoded = 0;
      for(unsigned long _cnt = 0; _cnt<maxLoop; _cnt++) {
        {
          byte * _bufEnd = z_ulongToBytes(_n, m_buffer);
          uint8_t _len = _bufEnd-m_buffer;
          const UErr* _error = &z_execTestEncoder(m_buffer, _len);
          if (_error->isError()) {
            if(m_errStatDict.inc(*_error) < 5) {
              g_err.error(_error->getErrNum(), _error->getErrMsg());
            }
            return;
          }
        }
        {
          byte * _bufEnd = m_buffer;
          if (b1<256) {*_bufEnd++ = static_cast<byte>(b1 & 0xFF);}
          if (b2<256) {*_bufEnd++ = static_cast<byte>(b2 & 0xFF);}
          if (b3<256) {*_bufEnd++ = static_cast<byte>(b3 & 0xFF);}
          if (b4<256) {*_bufEnd++ = static_cast<byte>(b4 & 0xFF);}

          for (uint8_t i=0; i<g_encodedBuffer.getSize(); i++) {
            const byte _byte = g_encodedBuffer.getByte(i);
            *_bufEnd++ = _byte;
          }
          g_encodedBuffer.reset();

          uint8_t _len = _bufEnd-m_buffer;
          z_execTest(_n, m_buffer, _len);
        }
        _n = mul==0 ? _n+1 : _n * mul;
      }

      std::cout << "ERROR TABLE:" << std::endl
      << m_errStatDict.toString() 
      << std::endl;

      std::cout << "Decoded " << m_numOfDecoded << " of " << maxLoop 
      << std::endl << std::endl;
    }


  private:
    const UErr& z_execTestEncoder(
      const byte *const inBuffer, 
      const uint8_t len
    ) {
        for (uint8_t i=0; i<len; i++) {
          const byte _byte = inBuffer[i];
          g_encodedBuffer.append(_byte);
        }
        g_encodedBuffer.appendStop();
        return g_encodedBuffer.getError();
    }

    void z_execTest(
      const unsigned long expectedValue, 
      const byte *const inBuffer, 
      const uint8_t len
    ){
      
      m_decoderReceiver.reset();
      for (uint8_t i=0; i<len; i++) {
        const byte _byte = inBuffer[i];

        //z_byteReceivedTest(expectedValue, _byte);
        m_decoderReceiver.appendByte(_byte);
        if (m_decoderReceiver.isMessageAvailable()) {
          z_messageReceived(expectedValue, m_decoderReceiver.getMessage());
        }
      }
    }

    void z_messageReceived(const unsigned long expectedValue, const byte * msgBuf) {
      unsigned long _ulvalue = z_bytesToULong(msgBuf);
      if (expectedValue!=_ulvalue) {
        if(m_errStatDict.inc(m_errTestFailed) < 5) {
          g_err.error(m_errTestFailed.getErrNum(), m_errTestFailed.getErrMsg());
        }
      }
      else{
        m_numOfDecoded++;
      }
    }

    /**
    Mette i byte di ulvalue nel buffer e ritorna il puntatore alla prima zona 
    libera del buffer.
    @param value: il valore unsigned long da convertire.
    @param ioBuffer: puntatore al buffer dove memorizzare i byte risultanti.
    @return: puntatore al buffer aggiornato.
    */
    static byte* z_ulongToBytes(unsigned long ulvalue, byte* ioBuffer) {
      constexpr size_t _len = sizeof(unsigned long);
      for (size_t i = 0; i < _len; ++i) {
        // Sposta il byte corrente nella posizione appropriata del buffer.
        ioBuffer[i] = static_cast<byte>((ulvalue >> (i * 8)) & 0xFF);
      }
      return ioBuffer + _len;
    }

    /**
    Converte una sequenza di byte in un valore unsigned long.
    @param ioBuffer: puntatore costante al buffer da cui leggere i byte.
    @return: il valore unsigned long ottenuto dalla conversione dei byte.
    */
    static unsigned long z_bytesToULong(const byte *const ioBuffer) {
      unsigned long ulvalue = 0;
      constexpr size_t _len = sizeof(unsigned long);
      for (size_t i = 0; i < _len; ++i) {
        // Ricostruisce il valore unsigned long dai byte nel buffer.
        ulvalue |= static_cast<unsigned long>(ioBuffer[i]) << (i * 8);
      }
      return ulvalue;
    }
};

