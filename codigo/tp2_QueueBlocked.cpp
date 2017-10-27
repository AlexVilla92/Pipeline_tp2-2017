#include "tp2_QueueBlocked.h"
#include <string>

QueueBlocked::QueueBlocked() {}

void QueueBlocked::Insertar(std::string unaLinea) {
	std::unique_lock<std::mutex> myLock(this->mutexCola);
	this->cola.push(unaLinea);
	myLock.unlock();
	cv.notify_all();
}
void QueueBlocked::Eliminar() {
	std::unique_lock<std::mutex> myLock(this->mutexCola);
	while (this->cola.empty()) {
		cv.wait(myLock);
	}
	this->cola.pop();
} 

unsigned int QueueBlocked::Tamanio() {
	std::unique_lock<std::mutex> myLock(this->mutexCola);
	return this->cola.size();
}

bool QueueBlocked::EstaVacia() {
	std::unique_lock<std::mutex> myLock(this->mutexCola);
	return (this->cola.empty());
} 

std::string QueueBlocked::Primero() {
	std::unique_lock<std::mutex> myLock(this->mutexCola);
    while (this->cola.empty()) {
    	cv.wait(myLock);
    }
    std::string word = this->cola.front();
    this->cola.pop();
    return word;
}


