/* Clase principal con el cual se ejecuta la fase de compilacion y la fase
 * de ejecucion a traves de la maquina virtual.
 * Autores:		Jorge Salazar Saltijeral				A00945665
 *				Veronica Alejandra Gonzalez Gonzalez	A01087523
 * Compiladores Enero - Mayo 2013
 */
#include "Stdio.h"
#include <iostream>
#include <map>
#include "Parser.h"
#include "Scanner.h"
#include "VirtualM.h"
#include <sys/timeb.h>
#include <wchar.h>
#include <utility>
using namespace std;

int main (int argc, char *argv[]) {
	if (argc == 2) {
		//string a = argv[1] + string("10.sol");
		string a = argv[1];
		wchar_t *fileName = coco_string_create(&a[0]);

		//Inicia la etapa del analisis lexico.
		Scanner *scanner = new Scanner(fileName);	

		//Inicia la etapa del analisis sintactico y semantico.
		Parser *parser = new Parser(scanner);
		parser->Parse();

		//Se pasa la lista de cuadruplos, directorio de clases y directorio de constantes.
		VirtualM machine = VirtualM(parser->dirGral, parser->cts, parser->gen);

		//Se libera la memoria de los objetos.
		delete parser;
		delete scanner;

		//Se ejecuta la maquina virtual.
		machine.run();
	} else {
		printf("No se especifico el archivo fuente o ejecuto mas de uno.\n");
	}	
	return 0;
}
