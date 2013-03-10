#include "Stdio.h"
#include <iostream>
#include "Parser.h"
#include "Scanner.h"
#include <sys/timeb.h>
#include <wchar.h>
using namespace std;
int main (int argc, char *argv[]) {
	if (argc == 2) {
		wchar_t *fileName = coco_string_create(argv[1]);
		Scanner *scanner = new Scanner(fileName);	
		Parser *parser = new Parser(scanner);
		parser->Parse(); // Compilador se ejecuta y termina
		//if (parser->errors->count == 0) {
			printf("Begin\n");		
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
