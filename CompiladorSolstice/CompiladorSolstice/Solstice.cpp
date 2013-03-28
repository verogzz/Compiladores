#include "Stdio.h"
#include <iostream>
#include <map>
#include "Parser.h"
#include "Scanner.h"
#include "Tabla.h"
#include "Cubo.h"
#include <sys/timeb.h>
#include <wchar.h>
#include <utility>
using namespace std;

int main (int argc, char *argv[]) {
	if (argc == 2) {
		wchar_t *fileName = coco_string_create(argv[1]);
		
		Scanner *scanner = new Scanner(fileName);	
		Parser *parser = new Parser(scanner);
		parser->Parse();
		
		for(int i = 0; i < parser->gen.size(); i++){
			cout << parser->gen.at(i).operador << '\t' << parser->gen.at(i).op1 << '\t' << parser->gen.at(i).op2 << '\t' << parser->gen.at(i).res << '\n';
		}
		cout << "operadores" << '\n';
		for(int i = 0; i < parser->gen.size() && !parser->oper.empty(); i++){
			cout << parser->oper.top() << '\n';
			 parser->oper.pop();
		}
		//for(SIT it = parser->dir.begin(); it != parser->dir.end(); it++){
			//cout<< it->first << '\t' << it->second.att_vis << '\t' << it->second.att_type << '\t' << it->second.att_mtd << '\n';
			//for(VMAP::iterator it2 = it->second.vars.begin(); it2 != it->second.vars.end(); it2++)
				//cout<< '\t' <<it2->first << '\t' << it2->second.var_type << '\t' << it2->second.var_dim << '\n';
		//}
			
		delete parser; // Se borra el compilador
		delete scanner;
	} else {
		printf("No se especifico el archivo fuente o ejecuto mas de uno.\n");
	}	
	return 0;
}
