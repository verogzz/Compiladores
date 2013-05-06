#include "Tabla.h"

using namespace std;

Variable::Variable(){
	var_type = var_dim = dir = -1;
};

Variable::Variable(int t, int d, int dr){
	var_type = t;
	var_dim = d;
	dir = dr;
};

Attribute::Attribute(){
	att_vis = att_type = att_mtd = dir = -1;
};

Attribute::Attribute(int vis, int type, int a_type, int dr){
	att_vis = vis;
	att_type = type;
	att_mtd = a_type;
	dir = dr;
};

Attribute::Attribute(int vis, int type, int a_type, int dr, int dirM){
	att_vis = vis;
	att_type = type;
	att_mtd = a_type;
	dir = dr;
	dirMem = dirM;
};


Attribute::Attribute(int vis, int type, int a_type){
	att_vis = vis;
	att_type = type;
	att_mtd = a_type;
};

ClassStruct::ClassStruct(){
	extended = "";
};

ClassStruct::ClassStruct(string e, CTABLE proc){
	extended = e;
	attributes = proc;
};

Cuadruplo::Cuadruplo(int a, int o1, int o2, int r){
	operador = a;
	op1 = o1;
	op2 = o2;
	res = r;
};

Cuadruplo::Cuadruplo(){
	operador = -1;
	op1 = -1;
	op2 = -1;
	res = -1;
};

CVariable::CVariable(){
	name = "";
	var_type = var_dim = dir =-1;
};

CVariable::CVariable(string n, int type, int dim, int dr){
	name = n;
	var_type = type;
	var_dim = dim;
	dir = dr;
};

Constantes::Constantes(){
	type = dir = -1;
};

Constantes::Constantes(int t, int m){
	type = t;
	dir = m;
};