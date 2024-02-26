#pragma once
// #include "IErrStat.h"

class IErrStat {
    public:
        virtual unsigned long inc(const UErr& err) = 0;
        virtual void reset() = 0;
        virtual char* toString(char* strBuff, size_t maxLen) = 0;
        virtual ~IErrStat() {} 
        static IErrStat * const DUMMY;
};

class CErrStateDummy : public IErrStat {
public:
    //CErrStateDummy() = default; // Costruttore di default esplicitamente dichiarato
    virtual ~CErrStateDummy() = default; // Distruttore di default esplicito

    // Implementazione di inc che semplicemente restituisce 0
    unsigned long inc(const UErr& err) override {
        // Implementazione dummy, sostituisci con la logica appropriata
        return 0;
    }

    // Implementazione di reset che non fa nulla
    void reset() override {
        // Implementazione dummy, sostituisci con la logica appropriata
    }

    // Implementazione di toString che restituisce una stringa fissa
    // Assicurati che maxLen sia sufficientemente grande per contenere la stringa fissa più il carattere terminatore '\0'
    char* toString(char* strBuff, size_t maxLen) override {
        // const char* dummyStr = "ErrStateDummy";
        // if (maxLen > strlen(dummyStr)) {
        //     strcpy(strBuff, dummyStr);
        // }
        return strBuff;
    }
};

// Funzione inline per inizializzare DUMMY
inline IErrStat* createDummyInstance() {
    static CErrStateDummy dummyInstance;
    return &dummyInstance;
}

// Inizializzazione del puntatore const DUMMY con la funzione sopra.
IErrStat* const IErrStat::DUMMY = createDummyInstance();

