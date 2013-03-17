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

		//dir.insert(TABLA::value_type("chingar", Attribute(1, 2, 0)));
		//dir.insert(TABLA::value_type("chingar", Attribute(1, 2, 1)));
		//cout << (dir.find("chingar") == dir.end())<< '\n';
		//cout << (dir.find("chi") == dir.end()) << '\n';
		
		for(SIT it = parser->dir.begin(); it != parser->dir.end(); it++){
			cout<< it->first << '\t' << it->second.att_vis << '\n';
		//SIT it = parser->dir.begin();
			for(VMAP::iterator it2 = it->second.vars.begin(); it2 != it->second.vars.end(); it2++)
				cout<< '\t' <<it2->first << '\t' << it2->second.var_type << '\n';
		}
		Cubo x = Cubo();
		int i = x.cubo[1][1][1];
		cout << i;
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
