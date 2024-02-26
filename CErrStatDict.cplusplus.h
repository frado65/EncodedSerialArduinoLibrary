#pragma once
// #include "CErrStatDict.cplusplus.h"

#include <map>
#include <cstdint> // Per uint16_t
#include <string>  // Per std::string
#include <sstream> // Per std::stringstream
#include <cstring>    // Per strncpy e strcpy
#include <algorithm>  // Per std::min

#include "IErrStat.h"

class CErrStatDict : public IErrStat {
private:
    class CErr {
    public:
        unsigned long m_cnt;
        const UErr& m_err; // Riferimento all'oggetto UErr

        // Costruttore che inizializza m_err con un riferimento a un oggetto UErr
        CErr(const UErr& err) : m_err(err), m_cnt(0) {} // Inizia il conteggio da 1
    };

    std::map<uint16_t, CErr> m_counters; // Mappa ora contiene oggetti CErr

public:
    unsigned long inc(const UErr& err) {
        auto it = m_counters.find(err.getErrNum());
        if (it == m_counters.end()) {
            // Se non esiste, inserisce un nuovo oggetto CErr e ottiene l'iteratore al nuovo elemento inserito
            auto insertResult = m_counters.insert({err.getErrNum(), CErr(err)});
            it = insertResult.first; // 'it' ora punta al nuovo elemento inserito
        }

        // Incrementa il contatore per l'errore specificato
        it->second.m_cnt++;
        return it->second.m_cnt;
    }

    // Resetta tutti i contatori a zero
    void reset() {
        m_counters.clear(); // Rimuove tutti gli elementi dalla mappa, distruggendo gli oggetti CErr
    }

    // Restituisce una stringa che rappresenta la mappa ordinata "chiave: valore"
    std::string toString() {
        std::stringstream ss;
        for (const auto& pair : m_counters) {
            ss << pair.first << ": " << pair.second.m_cnt << ", Error: " << pair.second.m_err.getErrMsg() << "\n";
        }
        return ss.str();
    }

char* toString(char* strBuff, size_t maxLen) override {
    std::stringstream ss;
    for (const auto& pair : m_counters) {
        ss << pair.first << ": " << pair.second.m_cnt << ", Error: " << pair.second.m_err.getErrMsg() << "\n";
    }

    std::string str = ss.str();
    size_t copyLen = std::min(str.length(), maxLen - 1); // Assicura di lasciare spazio per '\0'
    
    if (maxLen > 5 && str.length() >= maxLen) {
        // Se il buffer è troppo piccolo per contenere l'intera stringa, appendi " ... "
        copyLen = maxLen - 6; // Lascia spazio per " ... " e '\0'
        strncpy(strBuff, str.c_str(), copyLen);
        strcpy(strBuff + copyLen, " ...");
    } else {
        // Se c'è abbastanza spazio, copia l'intera stringa
        strncpy(strBuff, str.c_str(), copyLen);
        strBuff[copyLen] = '\0'; // Assicura che la stringa sia terminata correttamente
    }

    return strBuff;
    }
};