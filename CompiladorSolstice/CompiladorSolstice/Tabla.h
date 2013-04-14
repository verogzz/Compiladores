#ifndef TABLA_H
#define TABLA_H
#include <stdio.h>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <wchar.h>

using namespace std;

#define PRIVATE 0
#define PROTECT 1
#define PUBLIC 2

#define INT 0
#define DOUBLE 1
#define STRING 2
#define BOOLEAN 3
#define OBJECT 4
#define VOID 5
#define MAIN 6

#define ATM 0
#define ARR 1

#define GLOBAL 0
#define LOCAL 1

#define ATT 0
#define MET 1

#define TRUE 1
#define FALSE 0

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

class Variable {
public: 
	int var_type;
	int var_dim;
	Variable();
	Variable(int t, int d);
};
typedef std::map<string , Variable> VMAP;

class Attribute {
public:
	int att_vis;
	int att_type;
	int att_mtd;
	int mtd_q;
	vector<int> params;
	VMAP vars;
	Attribute();
	Attribute(int vis, int type, int a_type, int dir);
	Attribute(int vis, int type, int a_type);
};
typedef std::map<string, Attribute> CTABLE;

class ClassStruct {
public:
	string extended;
	CTABLE attributes;
	ClassStruct();
	ClassStruct(string e, CTABLE proc);
};
typedef std::map<string, ClassStruct> TABLE;

class Cuadruplo {
public:
	int operador;
	string op1;
	string op2;
	string res;
	Cuadruplo();
	Cuadruplo(int a, string o1, string o2, string res);
};

class CVariable : public Variable{
public:
	string name;
	CVariable();
	CVariable(string n, int type, int dim);
};

class Constantes {
public:
	int type;
	int memoria;
	Constantes();
	Constantes(int t, int m);
};
typedef std::map<string, Constantes> CTS;
#endif