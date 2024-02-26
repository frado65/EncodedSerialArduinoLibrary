#pragma once
// UUID-20240210164040
// #include "CErrorSerial.h"

class CErrorSerial : public IError {
  private:
    Stream& m_serialPort;
  public:
    CErrorSerial(Stream& serialPort): m_serialPort(serialPort) {}

    void error(const uint8_t errnum) override {
      m_serialPort.print("Errore: ");
      m_serialPort.println(errnum); // Invia il codice di errore via seriale
    }

    void error(const uint8_t errnum, const char* message) override {
      m_serialPort.print("Errore: ");
      m_serialPort.print(errnum);
      m_serialPort.print(" - ");
      m_serialPort.println(message); // Invia codice di errore e messaggio via seriale
    }
};