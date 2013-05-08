/* Listado de prototipos para las estructuras de datos
 * Autores:		Jorge Salazar Saltijeral				A00945665
 *				Veronica Alejandra Gonzalez Gonzalez	A01087523
 * Compiladores Enero - Mayo 2013
 */
#ifndef TABLA_H
#define TABLA_H
#include <stdio.h>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <wchar.h>

using namespace std;

//Visibilidad
#define PRIVATE 0
#define PROTECT 1
#define PUBLIC 2

//Tipo de datos
#define INT 0
#define DOUBLE 1
#define STRING 2
#define BOOLEAN 3
#define VOID 4
#define MAIN 5
#define OBJECT 6

//Dimension
#define ATM 0
#define ARR 1

//Scopes
#define GLOBAL 0
#define LOCAL 1

//Tipo de attributo
#define ATT 0
#define MET 1

//Constantes boolean
#define TRUE 1
#define FALSE 0

//Codigos de operacion
#define SUM	0
#define SUB 1
#define MUL 2
#define DIV 3
#define MOD 4
#define AND 5
#define OR 6
#define LT 7
#define GT 8
#define EQ 9
#define DIF 10
#define GEQ 11
#define LEQ 12
#define APP 13
#define ASI 14
#define GTT 15
#define GTF 16
#define GTO 17
#define REA 18
#define WRI 19
#define ERA 20
#define GSU 21
#define RET 22
#define MR 23
#define PAR 24
#define VER 25
#define MAI 26
#define ACC 27
#define NOT 28
#define END 29

class Variable {
public: 
	int var_type;	//Tipo de dato de la variable
	int var_dim;	//Dimension de la variable
	int dir;		//Direccion de memoria
	//Constructores
	Variable();
	Variable(int t, int d, int dr);
};
typedef std::map<string , Variable> VMAP;	//Tabla de variables

class Attribute {
public:
	int att_vis;	//Visibilidad del atributo
	int att_type;	//Tipo de dato
	int att_mtd;	//Tipo de campo atributo/metodo
	int dir;		//Direccion de memoria
	int dirMem;		//Direccion del cuadruplo
	vector<Variable> params;	//Tabla de parametros
	VMAP vars;		//Tabla de variables
	//Constructores
	Attribute();
	Attribute(int vis, int type, int a_type, int dr);
	Attribute(int vis, int type, int a_type, int dr, int dirM);
	Attribute(int vis, int type, int a_type);
};
typedef std::map<string, Attribute> CTABLE;	//Tabla de procedimientos

class ClassStruct {
public:
	string extended;	//Clase padre
	CTABLE attributes;	//Lista de atributos/metodos
	int tamanio;		//Tamanio de clase
	int serie;			//Id de la clase
	//Constructores
	ClassStruct();
	ClassStruct(string e, CTABLE proc);
	ClassStruct(string e, CTABLE proc, int tam, int s);
};
typedef std::map<string, ClassStruct> TABLE;	//Tabla general de clases

class Cuadruplo {
public:
	int operador; //Codigo de operacion
	int op1;	//Operando 1
	int op2;	//Operando 2
	int res;	//Operando 3
	//Constructores
	Cuadruplo();
	Cuadruplo(int a, int o1, int o2, int res);
};

class CVariable : public Variable{
public:
	string name;	//Nombre de la variable
	//Constructores
	CVariable();
	CVariable(string n, int type, int dim, int dr);
};

class Constantes {
public:
	int type;	//Tipo de dato de la constante
	int dir;	//Direccion de memoria
	//Constructores
	Constantes();
	Constantes(int t, int m);
};
typedef std::map<string, Constantes> CTS; //Tabla de constantes
#endif