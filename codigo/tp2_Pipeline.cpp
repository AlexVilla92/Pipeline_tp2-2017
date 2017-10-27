#include "tp2_Pipeline.h"
#include <string>
#include <vector>
	
Pipeline::Pipeline(int argc, char *argv[]) {
	this->colaEntrada = new QueueBlocked();
	this->colaSalida = new QueueBlocked();
	this->cantComandos = 0;
	this->CargarComandos(argc,argv);
	this->cantEtapas = 0;
	this->creoThreads = true;
	this->sigo = true;
	this->modo = VACIO;
}

int Pipeline::ProcesarDatos(int argc, char *argv[], std::string unModo) {
	int codigoRetorno;
	this->modo = unModo;
	std::string nameFile(VACIO);
	if (this->modo == "debug") {
		std::string flag = std::string(argv[2]);
		if (argc > 3) {
			nameFile = std::string(argv[3]);
		}
		codigoRetorno = this->LeerDatos(flag, nameFile);
	} else {
		std::string flag = std::string(argv[1]);
		if (argc > 2) {
			nameFile = std::string(argv[2]);
		}
		codigoRetorno = this->LeerDatos(flag,nameFile);
	}
	if (codigoRetorno == EXITO) {
		this->MostrarSalida(argc,argv);
		if (this->modo == "debug") {
			this->MostrarSalidaStandarError();
		}
		return EXITO;
	} else {
		return ERROR;
	}
}

int Pipeline::LeerDatos(std::string flag, std::string nameFile) {
	int codigoRetorno;
	if (flag == FLAG_INPUT) {
		codigoRetorno = this->LeerArchivoInput(nameFile);
	} else {
		codigoRetorno = this->LeerEntradaStandar();
	}
	return codigoRetorno;
}

void Pipeline::MostrarSalida(int argc, char *argv[]) {
	if (this->modo == "debug") {
		if (argc > 5) {
			this->MostrarOutput(std::string(argv[2]),std::string(argv[3]),
								std::string(argv[4]),std::string(argv[5]));
		} else {
			this->Salida();
		}
	} else {
		if (argc > 4) {
			this->MostrarOutput(std::string(argv[1]),std::string(argv[2]),
								std::string(argv[3]),std::string(argv[4]));
		} else {
			this->Salida();
		}
	}
}

void Pipeline::MostrarOutput(std::string out1, std::string name1,
						   std::string out2, std::string name2) {
	if ((out1 == FLAG_OUTPUT) || (out2 == FLAG_OUTPUT)) {
		std::string fileName = (out1 == FLAG_OUTPUT) ? name1:name2;
		std::ofstream fileOutput;	
		fileOutput.open(fileName);
		this->Salida(fileOutput);
		fileOutput.close();
	} else {
		this->Salida();
	}
}

void Pipeline::Salida() {
	unsigned int tam = this->colaSalida->Tamanio();
	for (unsigned int i = 0; i < tam; ++i) {
		std::string unaLinea = this->colaSalida->Primero();
		if (unaLinea != std::string(TERMINE)) {
			std::cout << unaLinea << std::endl;
		}
	}
}

void Pipeline::Salida(std::ofstream &fileOutput) {
	unsigned int tam = this->colaSalida->Tamanio();
	for (unsigned int i = 0; i < tam; ++i) {
		std::string unaLinea = this->colaSalida->Primero();
		if (unaLinea != std::string(TERMINE)) {
			fileOutput << unaLinea + "\n";
		}
	}
}

int Pipeline::LeerEntradaStandar() {
	std::string flujoDatos;
	while (std::getline(std::cin,flujoDatos)) {
		this->LanzarThreads(flujoDatos);
	}
	this->colaEntrada->Insertar(std::string(TERMINE));
	this->DestruirHilos();
	return EXITO;
}

int Pipeline::LeerArchivoInput(std::string &nameFile) {
	std::string linea;
	std::ifstream file(nameFile);
	if (file.is_open()) {
		while (getline(file,linea)) {
			this->LanzarThreads(linea);
		}
		this->colaEntrada->Insertar(std::string(TERMINE));
		this->DestruirHilos();
		file.close();
		return EXITO;
	} else {
		return ERROR;
	}
}

void Pipeline::EjecutarUnComando(std::string &linea) {
	this->colaEntrada->Insertar(linea);
	if (this->creoThreads) {
		this->cantEtapas++;
		this->creoThreads = false;
		std::string unComando = this->comandos.front();
		unsigned int pos = 0;
		Thread *unThread = this->CrearUnicoThread(this->colaEntrada,
												  this->colaSalida,unComando,
												  pos);
		this->vectorThreads.push_back(unThread);
		this->vectorThreads[0]->Start();
	}
}

void Pipeline::EjecutarVariosComandos(std::string &linea) {
	this->colaEntrada->Insertar(linea);
	if (this->creoThreads) {
		this->creoThreads = false;
		this->CrearThreads();
		unsigned int tam = this->vectorThreads.size();
		for (unsigned int i = 0; i < tam; ++i) {
			this->vectorThreads[i]->Start();
		}
	}
}

void Pipeline::CrearThreads() {
	this->CargarVectorQueues();
	QueueBlocked *comodin = NULL;
	Thread *unThread = NULL; 
	unsigned int tam = this->comandos.size();
	unsigned int cantQueues = tam - 3;
	for (unsigned int i = 0; i < tam; ++i) {
		std::string unComando = this->comandos[i];
		QueueBlocked *primero = NULL;
		if (comodin != NULL) {
			primero = comodin;
		} else {
			primero = this->colaQueues.front();
			this->colaQueues.pop();
			this->colaQueues.push(primero);
		}
		if (i == 0) {
			this->cantEtapas++;
			unThread = this->CrearUnicoThread(this->colaEntrada,primero,
											  unComando, i);
			comodin = primero;
		} else if (i >= cantQueues) {
			this->cantEtapas++;
			unThread = this->CrearUnicoThread(primero,this->colaSalida,
											  unComando, i);
		} else {
			this->cantEtapas++;
			QueueBlocked *otraQueue = this->colaQueues.front();
			this->colaQueues.pop();
			this->colaQueues.push(otraQueue);
			unThread = this->CrearUnicoThread(primero,otraQueue,
											  unComando, i);
			comodin = otraQueue;
		}
		this->vectorThreads.push_back(unThread);
	}
}

void Pipeline::CargarVectorQueues() {
	int cantQueues = this->cantComandos - 1;
	for (int i = 0; i < cantQueues; ++i) {
		QueueBlocked *unaQueue = new QueueBlocked();
		this->colaQueues.push(unaQueue);
	}
}

void Pipeline::LanzarThreads(std::string &linea) {
	if (this->cantComandos == 1) {
		this->EjecutarUnComando(linea);
		return;
	} else {
		this->EjecutarVariosComandos(linea);
		return;
	}
}

 Thread* Pipeline::CrearUnicoThread(QueueBlocked *queueEntrada,
 								  QueueBlocked *queueSalida, 
 								  std::string unComando, unsigned int &pos) {
	Thread *unThread = NULL;
	if (unComando == ECHO) {
		unThread = new AplicarEcho(std::ref(queueEntrada),
								   std::ref(queueSalida),
								   this->cantEtapas,
								   std::ref(this->vectorError),
								   std::ref(this->modo),
								   std::ref(this->mutexDebug));
	} else if (unComando == MATCH) {
		++pos;
		std::string wordMatch = this->comandos[pos];
		unThread = new AplicarMatch(wordMatch,std::ref(queueEntrada),
									std::ref(queueSalida),
									this->cantEtapas,
									std::ref(this->vectorError),
									std::ref(this->modo),
									std::ref(this->mutexDebug));
	} else if (unComando == REPLACE) {
		++pos;
		std::string wordFind = this->comandos[pos];
		++pos;
		std::string wordReplace = this->comandos[pos];
		unThread = new AplicarReplace(wordFind, wordReplace,
									  std::ref(queueEntrada),
									  std::ref(queueSalida),
									  this->cantEtapas,
									  std::ref(this->vectorError),
									  std::ref(this->modo),
									  std::ref(this->mutexDebug));
	}
	return unThread;
}

void Pipeline::MostrarSalidaStandarError() { 
	int etapa = 1;
	std::vector<std::string>::iterator it = this->vectorError.begin();
	bool esPrimero = true;
	std::string::size_type formato;
	while (etapa <= this->cantEtapas) {
		for (; it != this->vectorError.end(); ++it) {
			if ((*it).length() == 1) {
				if (std::stoi(*it) == etapa) {
					if (esPrimero) {
						*it = VACIO; 
						++it;
						std::string etapaWord = "(" + std::to_string(etapa) + ") ";
						std::string unComando = *it;
						*it = VACIO;
						++it;
						esPrimero =  false;
						std::cerr << etapaWord << unComando << std::endl;
						std::cerr << *it << std::endl;
						*it =VACIO;
					} else {
						*it = VACIO;
						++it;
						*it = VACIO;
						++it;
						std::string unaLinea = *it;
						std::cerr << unaLinea << std::endl;
						*it = VACIO;
					}
				}
			}
		}
		it = this->vectorError.begin();
		esPrimero= true;
		++etapa;
		if (etapa <= this->cantEtapas) {
			std::cerr << std::endl;
		}
	}
}

void Pipeline::CargarComandos(int argc, char *argv[]) {
	for (int i = 0; i < argc; ++i) {
		std::string op = std::string(argv[i]);
		if ((op == ECHO) || (op == REPLACE) || (op == MATCH)) {
			if (op == ECHO) {
				this->comandos.push_back(op);
				this->cantComandos++;
			}
			if (op == MATCH) {
				this->comandos.push_back(op);
				++i;
				std::string wordMatch = std::string(argv[i]);
				this->comandos.push_back(std::string(wordMatch));
				this->cantComandos++;
			} 
			if (op == REPLACE) {
				this->comandos.push_back(op);
				++i;
				std::string wordFind = std::string(argv[i]);
				this->comandos.push_back(wordFind);
				++i;
				std::string wordReplace = std::string(argv[i]);			
				this->comandos.push_back(wordReplace);
				this->cantComandos++;
			}
		}
	}
}

void Pipeline::DestruirHilos() {
	unsigned int tam = this->vectorThreads.size();
	if (tam > 0) {
		for (unsigned int i = 0; i < tam; ++i) {
    		this->vectorThreads[i]->Join();
        	delete this->vectorThreads[i];
    	}
	}
}

Pipeline::~Pipeline() {
	delete this->colaEntrada;
	delete this->colaSalida;
	unsigned int tam = this->colaQueues.size();
	if (tam > 0) {
		 for (unsigned int i = 0; i < tam; ++i) {
		 	QueueBlocked *unaQueue = this->colaQueues.front();
		 	this->colaQueues.pop();
		 	delete unaQueue;
		 }
	}
}
