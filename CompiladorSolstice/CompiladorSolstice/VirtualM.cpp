#include "VirtualM.h"

VirtualM::VirtualM(){}

VirtualM::VirtualM(TABLE dirGral, CTS ctes, vector<Cuadruplo> p){
	dirClasses = dirGral;
	prog = p;

	for(CTS::iterator it = ctes.begin(); it != ctes.end(); it++){
		switch (it->second.type) {
		case INT:	m_constante.m_i[it->second.dir] = atoi(&(it->first)[0]); 
					break;
		case DOUBLE:	m_constante.m_d[it->second.dir] = atof(&(it->first)[0]); 
						break;
		case STRING:	m_constante.m_s[it->second.dir] = it->first;
						break;
		case BOOLEAN:	if(it->first.compare("true") == 0){
							m_constante.m_b[it->second.dir] = true;
						}else{
							m_constante.m_b[it->second.dir] = false;
						}
						break;
		default:
			break;
		}
	}
}

int VirtualM::memory_type(int dir){
	if(dir >= bgi && dir < bli){
		return GLOBAL;
	}else if(dir >= bli && dir < bti){
		return LOCAL;
	}else if(dir >= bti && dir < bci){
		return TEMPORAL;
	}else if(dir >= bci && dir <= lcb){
		return CONSTANT;
	}else{
		return ERROR;
	}
}

int VirtualM::mvalue_type(int dir, int mt){
	switch (mt)	{
	case GLOBAL: if(dir >= bgi && dir < bgd){
					return INT;
				}else if(dir >= bgd && dir < bgs){
					return DOUBLE;
				}else if(dir >= bgs && dir < bgb){
					return STRING;
				}else if(dir >= bgb && dir <= lgb){
					return BOOLEAN;
				}else{
					return ERROR;
				}
				break;
	case LOCAL: if(dir >= bli && dir < bld){
					return INT;
				}else if(dir >= bld && dir < bls){
					return DOUBLE;
				}else if(dir >= bls && dir < blb){
					return STRING;
				}else if(dir >= blb && dir <= llb){
					return BOOLEAN;
				}else{
					return ERROR;
				}
				break;
	case TEMPORAL: if(dir >= bti && dir < btd){
					return INT;
				}else if(dir >= btd && dir < bts){
					return DOUBLE;
				}else if(dir >= bts && dir < btb){
					return STRING;
				}else if(dir >= btb && dir <= ltb){
					return BOOLEAN;
				}else{
					return ERROR;
				}
				break;
	case CONSTANT: if(dir >= bci && dir < bcd){
					return INT;
				}else if(dir >= bcd && dir < bcs){
					return DOUBLE;
				}else if(dir >= bcs && dir < bcb){
					return STRING;
				}else if(dir >= bcb && dir <= lcb){
					return BOOLEAN;
				}else{
					return ERROR;
				}
				break;
	case ERROR: 
	default:
		return ERROR;
		break;
	}
}

void VirtualM::run(){
	for(cIt = prog.at(0).op1; cIt < prog.size(); cIt++){
		current = prog.at(cIt);
		switch (current.operador) {
		case SUM : sum(); break;
		case SUB : substration(); break;
		case MUL : multiplication(); break;
		case DIV : division(); break;
		case MOD : module(); break;
		case AND : and(); break;
		case OR : or(); break;
		case LT : less_than(); break;
		case GT : greater_than(); break;
		case EQ : equal_equal(); break;
		case DIF : diference(); break;
		case GEQ : greater_equal(); break;
		case LEQ : less_equal(); break;
		case APP : append(); break;
		case ASI : asign(); break;
		case GTT : go_to_true(); break;
		case GTF : go_to_false(); break;
		case GTO : go_to(); break;
		case REA : read(); break;
		case WRI : write(); break;
		case ERA : era(); break;
		case GSU : go_sub(); break;
		case RET : end_module(); break;
		case MR : return_value(); break;
		case PAR : parameter(); break;
		case VER : limits_array(); break;
		default:
			break;
		}
	}
}

void VirtualM::sum(){
	double o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case INT: m_temporal.m_i[current.res] = o1 + o2;
		break;
	case DOUBLE: m_temporal.m_d[current.res] = o1 + o2;
		break;
	case STRING: 
	case BOOLEAN: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::substration(){
	double o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case INT: m_temporal.m_i[current.res] = o1 - o2;
		break;
	case DOUBLE: m_temporal.m_d[current.res] = o1 - o2;
		break;
	case STRING: 
	case BOOLEAN: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::multiplication(){
	double o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case INT: m_temporal.m_i[current.res] = o1 * o2;
		break;
	case DOUBLE: m_temporal.m_d[current.res] = o1 * o2;
		break;
	case STRING: 
	case BOOLEAN: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::division(){
	double o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}
	if(o2 != 0.0){
		switch(mvalue_type(current.res, TEMPORAL)){
		case INT: m_temporal.m_i[current.res] = o1 / o2;
			break;
		case DOUBLE: m_temporal.m_d[current.res] = o1 / o2;
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
	}else{
		cout << "DIVISION BY ZERO!!!\n";
		exit(0);
	}
}

void VirtualM::module(){
	double o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case INT: m_temporal.m_i[current.res] = (int)o1 % (int)o2;
		break;
	case DOUBLE: m_temporal.m_d[current.res] = (int)o1 % (int)o2;
		break;
	case STRING: 
	case BOOLEAN: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::and(){
	bool o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case BOOLEAN: o1 = m_global.m_b[current.op1];
			break;
		case DOUBLE:
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case BOOLEAN: o1 = m_local.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case BOOLEAN: o1 =m_temporal.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case BOOLEAN: o1 = m_constante.m_b[current.op1];
			break;
		case DOUBLE:
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case BOOLEAN: o2 = m_global.m_b[current.op2];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case BOOLEAN: o2 = m_local.m_b[current.op2];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case BOOLEAN: o2 = m_temporal.m_b[current.op2];
			break;
		case DOUBLE:
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case BOOLEAN: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE:
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case BOOLEAN: m_temporal.m_i[current.res] = o1 && o2;
		break;
	case DOUBLE:
	case STRING: 
	case INT: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::or(){
	bool o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case BOOLEAN: o1 = m_global.m_b[current.op1];
			break;
		case DOUBLE:
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case BOOLEAN: o1 = m_local.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case BOOLEAN: o1 =m_temporal.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case BOOLEAN: o1 = m_constante.m_b[current.op1];
			break;
		case DOUBLE:
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case BOOLEAN: o2 = m_global.m_b[current.op2];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case BOOLEAN: o2 = m_local.m_b[current.op2];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case BOOLEAN: o2 = m_temporal.m_b[current.op2];
			break;
		case DOUBLE:
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case BOOLEAN: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE:
		case STRING: 
		case INT: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case BOOLEAN: m_temporal.m_i[current.res] = o1 || o2;
		break;
	case DOUBLE:
	case STRING: 
	case INT: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::less_than(){
	double o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case BOOLEAN: m_temporal.m_i[current.res] = o1 < o2;
		break;
	case DOUBLE:
	case STRING: 
	case INT: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::greater_than(){
	double o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case BOOLEAN: m_temporal.m_i[current.res] = o1 > o2;
		break;
	case DOUBLE:
	case STRING: 
	case INT: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::equal_equal(){
	double o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case BOOLEAN: m_temporal.m_i[current.res] = o1 == o2;
		break;
	case DOUBLE:
	case STRING: 
	case INT: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::diference(){
	double o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case BOOLEAN: m_temporal.m_i[current.res] = o1 != o2;
		break;
	case DOUBLE:
	case STRING: 
	case INT: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::greater_equal(){
	double o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case BOOLEAN: m_temporal.m_i[current.res] = o1 >= o2;
		break;
	case DOUBLE:
	case STRING: 
	case INT: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::less_equal(){
	double o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case BOOLEAN: m_temporal.m_i[current.res] = o1 <= o2;
		break;
	case DOUBLE:
	case STRING: 
	case INT: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}
}

void VirtualM::append(){
	string o1, o2;	
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case STRING: o1 = m_global.m_s[current.op1];
			break;
		case BOOLEAN: o1 = m_global.m_b[current.op1];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case STRING: o1 = m_local.m_s[current.op1];
			break;
		case BOOLEAN: o1 = m_local.m_b[current.op1];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 = m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case STRING: o1 = m_temporal.m_s[current.op1];
			break;
		case BOOLEAN: o1 = m_temporal.m_b[current.op1];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case STRING: o1 = m_constante.m_s[current.op1];
			break;
		case BOOLEAN: o1 = m_constante.m_b[current.op1];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: o2 = m_global.m_s[current.op2];
			break;
		case BOOLEAN: o2 = m_global.m_b[current.op2];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: o2 = m_local.m_s[current.op2];
			break;
		case BOOLEAN: o2 = m_local.m_b[current.op2];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: o2 = m_temporal.m_s[current.op2];
			break;
		case BOOLEAN: o2 = m_temporal.m_b[current.op2];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: o2 = m_constante.m_s[current.op2];
			break;
		case BOOLEAN: o2 = m_constante.m_b[current.op2];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch(mvalue_type(current.res, TEMPORAL)){
	case STRING: m_temporal.m_s[current.res] = o1 + o2;
		break;
	case DOUBLE:
	case INT: 
	case BOOLEAN: 
	case ERROR:
	default:
		cout << "TYPE MISMATCH!!!\n";
		break;
	}}

void VirtualM::asign(){
	string os1;	
	double od1;	
	bool ob1;

	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: od1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: od1 = m_global.m_d[current.op1];
			break;
		case STRING: os1 = m_global.m_s[current.op1];
			break;
		case BOOLEAN: ob1 = m_global.m_b[current.op1];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: od1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: od1 = m_local.m_d[current.op1];
			break;
		case STRING: os1 = m_local.m_s[current.op1];
			break;
		case BOOLEAN: ob1 = m_local.m_b[current.op1];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: od1 = m_temporal.m_i[current.op1];
			break;
		case DOUBLE: od1 = m_temporal.m_d[current.op1];
			break;
		case STRING: os1 = m_temporal.m_s[current.op1];
			break;
		case BOOLEAN: ob1 = m_temporal.m_b[current.op1];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: od1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: od1 = m_constante.m_d[current.op1];
			break;
		case STRING: os1 = m_constante.m_s[current.op1];
			break;
		case BOOLEAN: ob1 = m_constante.m_b[current.op1];
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}

	switch (memory_type(current.res)){
	case GLOBAL: 
		switch(mvalue_type(current.res, GLOBAL)){
		case INT: m_global.m_i[current.res] = od1;
			break;
		case DOUBLE: m_global.m_d[current.res] = od1;
			break;
		case STRING: m_global.m_s[current.res] = os1;
			break;
		case BOOLEAN: m_global.m_b[current.res] = ob1;
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case INT: m_local.m_i[current.res] = od1;
			break;
		case DOUBLE: m_local.m_d[current.op2] = od1;
			break;
		case STRING: m_local.m_s[current.op2] = os1;
			break;
		case BOOLEAN: m_local.m_b[current.op2] = ob1;
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: m_temporal.m_i[current.res] = od1;
			break;
		case DOUBLE: m_temporal.m_d[current.res] = od1;
			break;
		case STRING: m_temporal.m_s[current.res] = os1;
			break;
		case BOOLEAN: m_temporal.m_b[current.res] = ob1;
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: m_constante.m_i[current.res] = od1;
			break;
		case DOUBLE: m_constante.m_d[current.op2] = od1;
			break;
		case STRING: m_constante.m_s[current.op2] = os1;
			break;
		case BOOLEAN: m_constante.m_b[current.op2] = ob1;
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}
}

void VirtualM::go_to_true(){
	cIt = current.res;
}

void VirtualM::go_to_false(){
	cIt = current.res;
}

void VirtualM::go_to(){
	cIt = current.res;
}

void VirtualM::read(){
	int m1;
	double m2;
	bool m3;
	try{
		switch (memory_type(current.op1)){
		case GLOBAL: 
			switch(mvalue_type(current.op1, GLOBAL)){
			case INT: cin >> m1;
				m_global.m_i[current.op1] = m1;
				break;
			case DOUBLE: cin >> m2;
				m_global.m_d[current.op1] = m2;;
				break;
			case STRING: cin >> m_global.m_s[current.op1];
				break;
			case BOOLEAN: cin >> m3;
				if(m3){
					m_global.m_d[current.op1] = true;
				}else{
					m_global.m_d[current.op1] = false;
				}
				break;
			case ERROR:
			default:
				cout << "TYPE MISMATCH!!!\n";
				break;
			}
			break;
		case LOCAL: 
			switch(mvalue_type(current.op1, LOCAL)){
			case INT: cin >> m1;
				m_local.m_i[current.op1] = m1;
				break;
			case DOUBLE: cin >> m2;
				m_local.m_d[current.op1] = m2;
				break;
			case STRING: cin >> m_local.m_s[current.op1];
				break;
			case BOOLEAN: cin >> m3;
				if(m3){
					m_local.m_d[current.op1] = true;
				}else{
					m_local.m_d[current.op1] = false;
				}
				break;
			case ERROR:
			default:
				cout << "TYPE MISMATCH!!!\n";
				break;
			}
			break;
		case TEMPORAL: 
			switch(mvalue_type(current.op1, TEMPORAL)){
			case INT: cin >> m1;
				m_temporal.m_i[current.op1] = m1;
				break;
			case DOUBLE: cin >> m2;
				m_temporal.m_d[current.op1] = m2;
				break;
			case STRING: cin >> m_temporal.m_s[current.op1];
				break;
			case BOOLEAN: cin >> m3;
				if(m3){
					m_temporal.m_d[current.op1] = true;
				}else{
					m_temporal.m_d[current.op1] = false;
				}
				break;
			case ERROR:
			default:
				cout << "TYPE MISMATCH!!!\n";
				break;
			}
			break;
		case CONSTANT: 
		case ERROR:
		default:
			cout << "MEMORY OUT OF BOUNDS!!!\n";
			break;
		}
	}catch (exception& e){
		cout<< "TYPE MISMATCH!!!";
		exit(0);
	}
}

void VirtualM::write(){
	string o1 = "";
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = ftoa(m_global.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_global.m_d[current.op1]);
			break;
		case STRING: o1 = strClean(m_global.m_s[current.op1]);
			break;
		case BOOLEAN: o1 = btoa(m_global.m_b[current.op1]);
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = ftoa(m_local.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_local.m_d[current.op1]);
			break;
		case STRING: o1 = strClean(m_local.m_s[current.op1]);
			break;
		case BOOLEAN: o1 = btoa(m_local.m_b[current.op1]);
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 = ftoa(m_temporal.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_temporal.m_d[current.op1]);
			break;
		case STRING: o1 = strClean(m_temporal.m_s[current.op1]);
			break;
		case BOOLEAN: o1 = btoa(m_temporal.m_b[current.op1]);
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = ftoa(m_constante.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_constante.m_d[current.op1]);
			break;
		case STRING: o1 = strClean(m_constante.m_s[current.op1]);
			break;
		case BOOLEAN: o1 = btoa(m_constante.m_b[current.op1]);
			break;
		case ERROR:
		default:
			cout << "TYPE MISMATCH!!!\n";
			break;
		}
		break;
	case ERROR:
	default:
		cout << "MEMORY OUT OF BOUNDS!!!\n";
		break;
	}
	cout << o1 << '\n';
}

void VirtualM::era(){}

void VirtualM::go_sub(){
	slines.push(cIt);
	cIt = current.op1;
}

void VirtualM::end_module(){
	if(slines.size() > 0){
		cIt = slines.top();
		slines.pop();
	}else{
		cout << "Memory Error!\n";
		exit(0);
	}
}

void VirtualM::return_value(){}

void VirtualM::parameter(){}

void VirtualM::limits_array(){
	int sub = current.op1;
	int li = current.op2;
	int ls = current.res;
	if (!(sub > li && sub < ls)){
		cout << "ARRAY OUT OF BOUNDS!!!\n";
		exit(0);
	}
}

string VirtualM::strClean(string s){
	return s.substr(1, s.length()-2);
}

string VirtualM::ftoa(float f){
	std::ostringstream buff;
	buff << f;
	return buff.str();
}

string VirtualM::btoa(bool b){
	if(b)
		return "true";
	else
		return "false";
}