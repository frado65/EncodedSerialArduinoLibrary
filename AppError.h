#pragma once
// UUID-20240210164040

// #define AppError_mode 0
// #include "AppError.h"

#include "IError.h"

#if AppError_mode==0
  CErrorNone __g_err = CErrorNone();
#elif AppError_mode==1
  #include "CErrorSerial.h"
  CErrorSerial __g_err = CErrorSerial(Serial);
#elif AppError_mode==2
  #include "CErrorLed.h"
  CErrorLed __g_err = CErrorLed(10);
#elif AppError_mode==3
  #include "CErrorLeds.h"
  CErrorLeds __g_err = CErrorLeds((byte[]){10, 11, 12, 255}); // Elenco dei pin terminato da 255
#elif AppError_mode==4
  #include "CErrorWrapper.h"
  CErrorSerial __errorSerial = CErrorSerial(Serial);
  CErrorLeds __errorLeds = CErrorLeds((byte[]){10, 11, 12, 255});
  IError* handlers[] = {&__errorSerial, &__errorLeds};
  byte numHandlers = sizeof(handlers) / sizeof(handlers[0]);
  CErrorWrapper __g_err(handlers, numHandlers);
#elif AppError_mode==5
  #include "CErrorStdCout.h"
  CErrorStdCout __g_err = CErrorStdCout();
#endif

IError& g_err = __g_err;
