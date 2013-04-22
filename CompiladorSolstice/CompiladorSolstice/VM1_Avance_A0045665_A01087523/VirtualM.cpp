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
		case BOOLEAN:	if(it->first.compare("true")){
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
	for(unsigned int i = prog.at(0).op1; i < prog.size(); i++){
		current = prog.at(i);
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
	cout<< o1 + o2 << '\n';
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
	cout<< o1 - o2 << '\n';
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
	cout<< o1 * o2 << '\n';
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
	cout<< o1 / o2 << '\n';
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
	cout<< (int)o1 % (int)o2 << '\n';
}

void VirtualM::and(){}
void VirtualM::or(){}
void VirtualM::less_than(){}
void VirtualM::greater_than(){}
void VirtualM::equal_equal(){}
void VirtualM::diference(){}
void VirtualM::greater_equal(){}
void VirtualM::less_equal(){}
void VirtualM::append(){}
void VirtualM::asign(){
	//cout << current.op1 << '\n';
	//cout << current.op2 << '\n';
	//cout << current.res << '\n';
}
void VirtualM::go_to_true(){}
void VirtualM::go_to_false(){}
void VirtualM::go_to(){}
void VirtualM::read(){}
void VirtualM::write(){}
void VirtualM::era(){}
void VirtualM::go_sub(){}
void VirtualM::end_module(){}
void VirtualM::return_value(){}
void VirtualM::parameter(){}
void VirtualM::limits_array(){}