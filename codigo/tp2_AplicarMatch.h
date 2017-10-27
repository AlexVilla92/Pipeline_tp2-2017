#ifndef TP2_APLICARMATCH_H_INCLUDED
#define TP2_APLICARMATCH_H_INCLUDED

#include "tp2_QueueBlocked.h"
#include "tp2_Thread.h"
#include <string>
#include <vector>

class AplicarMatch: public Thread {
private:
    std::string wordMatch;
    QueueBlocked *queueEntrada;
    QueueBlocked *queueSalida;
    int etapa;
    std::vector<std::string> &vectorDebug;
    std::string &modo;
    std::mutex &mutexMatch;
    static int cantObjetosMatch;
    std::string nombreEtapa; 
    int numeroComando;

public:
    AplicarMatch(std::string word, 
        QueueBlocked *entrada,
        QueueBlocked *salida,
        int unaEtapa,
        std::vector<std::string> &unVector,
        std::string &unModo, 
        std::mutex &mu);
    void ModoDebug(std::string copia, std::string linea);
    virtual void run() override;
};

#endif
