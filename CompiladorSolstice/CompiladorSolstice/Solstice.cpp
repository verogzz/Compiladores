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
		string a = argv[1] + string("3.sol");
		wchar_t *fileName = coco_string_create(&a[0]);
		Scanner *scanner = new Scanner(fileName);	
		Parser *parser = new Parser(scanner);
		parser->Parse();
		//if(!parser->err){
			VirtualM machine = VirtualM(parser->dirGral, parser->cts, parser->gen);
			delete parser;
			delete scanner;
			machine.run();
		//}else{
			//exit(EXIT_FAILURE);
		//}
	} else {
		printf("No se especifico el archivo fuente o ejecuto mas de uno.\n");
	}	
	return 0;
}
