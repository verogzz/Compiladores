#include "Stdio.h"
#include <iostream>
#include <map>
#include "Parser.h"
#include "Scanner.h"
#include "Cubo.h"
#include "Tabla.h"
#include <sys/timeb.h>
#include <wchar.h>
#include <utility>
using namespace std;

int main (int argc, char *argv[]) {
	if (argc == 2) {
		wchar_t *fileName = coco_string_create(argv[1]);
		Tabla dirC;
		Scanner *scanner = new Scanner(fileName);	
		Parser *parser = new Parser(scanner);
		
		//Metodo metodoP = Metodo(2, 3);
		//dirC.publico[dirC.conv(L"chingar")] = metodoP;
		
		//parser->Parse(); // Compilador se ejecuta y termina
		//if (parser->errors->count == 0) {
		//printf("%d\n", dirC.publico[dirC.conv(L"chingar")].m_type );		
		//}else{
			//printf("%s", parser->errors);
		//}
		delete parser; // Se borra el compilador
		delete scanner;
	} else {
		printf("No se especifico el archivo fuente o ejecuto mas de uno.\n");
	}	
	getchar();
	return 0;
}
