#include "tp2_AplicarMatch.h"
#include <string>
#include <regex>
#include <vector>

int AplicarMatch::cantObjetosMatch = 0;

AplicarMatch::AplicarMatch(std::string word, 
    QueueBlocked *entrada,
    QueueBlocked *salida,
    int unaEtapa,
    std::vector<std::string> &unVector,
    std::string &unModo,
    std::mutex &mu) : 
    wordMatch(word),
    queueEntrada(entrada),
    queueSalida(salida),
    etapa(unaEtapa),
    vectorDebug(unVector),
    modo(unModo),
    mutexMatch(mu) {
        this->nombreEtapa = "match";
        AplicarMatch::cantObjetosMatch++;
        this->numeroComando = AplicarMatch::cantObjetosMatch;
}

void AplicarMatch::ModoDebug(std::string copia, std::string linea) {
    std::unique_lock<std::mutex> myLock(this->mutexMatch);
    this->vectorDebug.push_back(std::to_string(this->etapa));
    std::string comando = this->nombreEtapa + 
                          std::to_string(this->numeroComando);
    this->vectorDebug.push_back(comando);
    std::string lineaCompuesta;
    if (linea != VACIO) {
        lineaCompuesta = linea + " -> " + linea;
    } else {
        lineaCompuesta = copia + " -> " + "(Filtrado)";
    }
        
    this->vectorDebug.push_back(lineaCompuesta);
}
            
void AplicarMatch::run() {
    std::string linea = this->queueEntrada->Primero();
    do {
        std::string copia = linea;
        std::smatch match;
        if (std::regex_search(linea,match,std::regex(this->wordMatch))) {
            this->queueSalida->Insertar(linea);
        } else {
            linea = std::string(VACIO);     
        }
        if (modo == "debug") {
            this->ModoDebug(copia, linea);
        }
        linea = this->queueEntrada->Primero();
    } while (linea != std::string(TERMINE));
    this->queueSalida->Insertar(std::string(TERMINE));
}

