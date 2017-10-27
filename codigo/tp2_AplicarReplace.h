#ifndef TP2_APLICARREPLACE_H_INCLUDED
#define TP2_APLICARREPLACE_H_INCLUDED

#include "tp2_QueueBlocked.h"
#include "tp2_Thread.h"
#include <string>
#include <vector>

class AplicarReplace: public Thread {
private:
    std::string wordFind;
    std::string wordReplace;
    QueueBlocked *queueEntrada;
    QueueBlocked *queueSalida; 
    int etapa;
    std::vector<std::string> &vectorDebug;
    std::string &modo;
    std::mutex &mutexReplace;
    static int cantObjetosReplace;
    std::string nombreEtapa;
    int numeroComando;

public:
    AplicarReplace(std::string find,
        std::string replace, 
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
