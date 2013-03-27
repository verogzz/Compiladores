#include "Tabla.h"

using namespace std;

Variable::Variable(){
	var_type = var_dim = 0;
};

Variable::Variable(int t, int d){
	var_type = t;
	var_dim = d;
};

Attribute::Attribute(){
	att_vis = att_type = att_mtd = 0;
};

Attribute::Attribute(int vis, int type, int a_type, string n_clase){
	att_vis = vis;
	att_type = type;
	att_mtd = a_type;
	clase = n_clase;
};

Cuadruplo::Cuadruplo(int a, string o1, string o2, string r){
	operador = a;
	op1 = o1;
	op2 = o2;
	res = r;
};

CVariable::CVariable(string n, int type, int dim){
	name = n;
	var_type = type;
	var_dim = dim;
};