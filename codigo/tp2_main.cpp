#include <iostream>
#include <string>
#include "tp2_Pipeline.h"

int main(int argc, char *argv[]) {
	if (argc > 1) {
		Pipeline unPipe(argc,argv);
		std::string flag(argv[1]);
		if (flag == FLAG_DEBUG) {
			std::string unModo("debug");
			return unPipe.ProcesarDatos(argc,argv,unModo);
		} else {
			std::string unModo("normal");
			return unPipe.ProcesarDatos(argc,argv,unModo);
		}
	} else {
		return ERROR;
	}
}
