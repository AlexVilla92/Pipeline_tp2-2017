#ifndef TP2_PIPELINE_H_INCLUDED
#define TP2_PIPELINE_H_INCLUDED

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <regex>
#include <queue>
#include <mutex>

#include "tp2_Thread.h"
#include "tp2_AplicarEcho.h"
#include "tp2_AplicarMatch.h"
#include "tp2_AplicarReplace.h"
#include "tp2_QueueBlocked.h"

#define ERROR 1
#define EXITO 0
#define FLAG_DEBUG "--debug"
#define FLAG_INPUT "--input"
#define FLAG_OUTPUT "--output"
#define ECHO "echo"
#define MATCH "match"
#define REPLACE "replace"
#define VACIO ""
#define TERMINE "E3C5D9D4C9D5C5"

class Pipeline {
private:
	std::vector<std::string> comandos;
	std::queue<QueueBlocked*> colaQueues;
	std::vector<Thread*> vectorThreads;
	std::vector<std::string> vectorError;
	QueueBlocked *colaEntrada, *colaSalida;
	std::mutex mutexDebug;
	int cantEtapas; 
	bool creoThreads;
	bool sigo; 
	std::string modo;
	int cantComandos;

public:
	Pipeline(int argc, char *argv[]);
	int ProcesarDatos(int argc, char *argv[], std::string unModo);
	~Pipeline();

private:
	int LeerDatos(std::string flag, std::string nameFile);
	void MostrarOutput(std::string out1, std::string name1,
					   std::string out2, std::string name2);
	void CargarComandos(int argc, char *argv[]);
	void CargarColaBloqueante(char *argv[]);
	int LeerArchivoInput(std::string &nameFile);
	int LeerEntradaStandar();
	void CargarVectorQueues();
	void MostrarSalidaStandarError();
	void LanzarThreads(std::string &linea);
	void EjecutarUnComando(std::string &linea);
	void EjecutarVariosComandos(std::string &linea);
	void CrearThreads();
	Thread* CrearUnicoThread(QueueBlocked *queueEntrada, 
							 QueueBlocked *queueSalida, 
 							 std::string unComando, unsigned int &pos);
	void DestruirHilos();
	void MostrarSalida(int argc, char *argv[]);
	void Salida(std::ofstream &fileOutput);
	void Salida();
};

#endif
