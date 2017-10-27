#include "tp2_AplicarReplace.h"
#include <string>
#include <vector>

int AplicarReplace::cantObjetosReplace = 0;

AplicarReplace::AplicarReplace(std::string find,
    std::string replace, 
    QueueBlocked *entrada,
    QueueBlocked *salida,
    int unaEtapa, 
    std::vector<std::string> &unVector,
    std::string &unModo,
    std::mutex &mu) : 
    wordFind(find),
    wordReplace(replace),
    queueEntrada(entrada),
    queueSalida(salida),
    etapa(unaEtapa),
    vectorDebug(unVector),
    modo(unModo),
    mutexReplace(mu) {
        this->nombreEtapa = "replace";
        AplicarReplace::cantObjetosReplace++;
        this->numeroComando = AplicarReplace::cantObjetosReplace;
}

void AplicarReplace::ModoDebug(std::string copia, std::string linea) {
    std::unique_lock<std::mutex> myLock(this->mutexReplace);
    this->vectorDebug.push_back(std::to_string(this->etapa));
    std::string comando = this->nombreEtapa + 
                          std::to_string(this->numeroComando);
    this->vectorDebug.push_back(comando);
    std::string lineaCompuesta = copia + " -> " + linea;
    this->vectorDebug.push_back(lineaCompuesta);
}     
            
void AplicarReplace::run() {
    std::string linea = this->queueEntrada->Primero();
        do { 
            std::string copia = linea;   
            std::regex wordRegex(this->wordFind); 
            linea = std::regex_replace(linea,wordRegex,this->wordReplace);
            this->queueSalida->Insertar(linea);
            if (modo == "debug") {
                this->ModoDebug(copia, linea);
            }
            linea = this->queueEntrada->Primero();
        } while (linea != std::string(TERMINE));
    this->queueSalida->Insertar(std::string(TERMINE));
}        

