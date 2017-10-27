#include "tp2_AplicarEcho.h"
#include <string>

#include <vector>

int AplicarEcho::cantObjetosEcho = 0;

AplicarEcho::AplicarEcho(QueueBlocked *entrada, QueueBlocked *salida,
    int unaEtapa, std::vector<std::string> &unVector,
    std::string &unModo, std::mutex &mu) : 
    queueEntrada(entrada), 
    queueSalida(salida),
    etapa(unaEtapa),
    vectorDebug(unVector),
    modo(unModo),
    mutexEcho(mu) {
    	this->nombreEtapa = "echo";
    	AplicarEcho::cantObjetosEcho++;
    	this->numeroComando = AplicarEcho::cantObjetosEcho;
} 

void AplicarEcho::ModoDebug(std::string linea) {
    std::unique_lock<std::mutex> myLock(this->mutexEcho);
    this->vectorDebug.push_back(std::to_string(this->etapa));
    std::string comando = this->nombreEtapa + 
    					  std::to_string(this->numeroComando);
    this->vectorDebug.push_back(comando);
    std::string lineaCompuesta = linea + " -> " + linea;
    this->vectorDebug.push_back(lineaCompuesta);
}

void AplicarEcho::run() {
    std::string linea = this->queueEntrada->Primero();
    do {
        this->queueSalida->Insertar(linea); 
        if (this->modo == "debug") {
            this->ModoDebug(linea);
        }
        linea = this->queueEntrada->Primero();
    } while (linea != std::string(TERMINE));
    this->queueSalida->Insertar(std::string(TERMINE));
}

