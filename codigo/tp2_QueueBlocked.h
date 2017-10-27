#ifndef TP2_QUEUEBLOCKED_H_INCLUDED
#define TP2_QUEUEBLOCKED_H_INCLUDED

#include <iostream>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>

class QueueBlocked {
private:
	std::queue<std::string> cola;
	std::mutex mutexCola;
	std::condition_variable cv;
public:
	QueueBlocked();
	bool EstaVacia();
	void Insertar(std::string unaLinea);
	unsigned int Tamanio();
	std::string Primero();
	void Eliminar();
};

#endif
