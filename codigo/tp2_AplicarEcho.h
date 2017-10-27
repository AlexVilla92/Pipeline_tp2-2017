#ifndef TP2_APLICARECHO_H_INCLUDED
#define TP2_APLICARECHO_H_INCLUDED

#include "tp2_QueueBlocked.h"
#include "tp2_Thread.h"
#include <string>
#include <vector>

class AplicarEcho: public Thread { 
private:
    QueueBlocked *queueEntrada;
    QueueBlocked *queueSalida;
    int etapa;
    std::vector<std::string> &vectorDebug;
	std::string &modo;
    std::mutex &mutexEcho;
    static int cantObjetosEcho;
    int numeroComando;
    std::string nombreEtapa;
	
public:
	AplicarEcho(QueueBlocked *entrada, QueueBlocked *salida,
        int unaEtapa, std::vector<std::string> &unVector,
        std::string &unModo, std::mutex &mu);

    void ModoDebug(std::string linea);
    virtual void run() override;
};

#endif
