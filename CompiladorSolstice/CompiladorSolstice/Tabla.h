#ifndef TABLA_H
#define TABLA_H
#include <stdio.h>
#include <map>
#include <string>
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
	string clase;
	VMAP vars;
	Attribute();
	Attribute(int vis, int type, int a_type, string clase);
	string conv(wchar_t* x);
};

typedef std::map<string, Attribute> TABLA;
typedef TABLA::iterator SIT;

#endif