#include "VirtualM.h"

VirtualM::VirtualM(){}

VirtualM::VirtualM(TABLE dirGral, CTS ctes, vector<Cuadruplo> p){
	prog = p;
	cmlb = cmld = cmli = cmls = cmtb = cmtd = cmti = cmts = 0;
	for(TABLE::iterator iG = dirGral.begin(); iG != dirGral.end(); iG++){
		for(CTABLE::iterator iC = iG->second.attributes.begin(); iC != iG->second.attributes.end(); iC++){
			if(iC->second.att_mtd == MET){
				switch (iC->second.att_type){
				case INT:	m_global.m_i[iC->second.dirMem] = -1; 
					break;
				case DOUBLE:	m_global.m_d[iC->second.dirMem] = -1.0; 
					break;
				case STRING:	m_global.m_s[iC->second.dirMem] = "";
					break;
				case BOOLEAN:	m_global.m_b[iC->second.dirMem] = NULL;
					break;
				default:
					break;
				}
			}
		}
	}

	for(CTS::iterator it = ctes.begin(); it != ctes.end(); it++){
		switch (it->second.type) {
		case INT:	m_constante.m_i[it->second.dir] = atoi(&(it->first)[0]); 
			break;
		case DOUBLE:	m_constante.m_d[it->second.dir] = atof(&(it->first)[0]); 
			break;
		case STRING:	m_constante.m_s[it->second.dir] = strClean(it->first);
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
	if(cmli > 2000 || cmld > 2000 || cmls > 2000 || cmlb > 200 ||
		cmti > 2000 || cmtd > 2000 || cmts > 2000 || cmtb > 200){
			error(0);
	}
	if(dir >= bgi && dir < bli){
		return GLOBAL;
	}else if(dir >= bli && dir < bti){
		return LOCAL;
	}else if(dir >= bti && dir < bci){
		return TEMPORAL;
	}else if(dir >= bci && dir <= lcb){
		return CONSTANT;
	}else if(dir >= btp && dir <= ltp){
		return POINTER;
	}else if(dir >= bmo && dir <= lmo){
		return OBJECTS;
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
	for(cIt = 0; cIt < prog.size(); cIt++){
		current = prog.at(cIt);
		//cout << cIt << '\t' << current.operador << '\t' << current.op1 << '\t' << current.op2 << '\t' << current.res << '\n';
	}
	for(cIt = prog.at(0).op1; cIt < prog.size(); cIt++){
		current = prog.at(cIt);
		//cout << cIt << '\t' << current.operador << '\t' << current.op1 << '\t' << current.op2 << '\t' << current.res << '\n';
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
		case ACC : access();break;
		case NOT: negation(); break;
		case END: exit(0);
		default:
			error(99);
			break;
		}
	}
}

void VirtualM::sum(){
	double o1, o2;	
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = m_local.m_i[p]; 
			break;
		case DOUBLE: o2 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case INT: m_temporal.m_i[current.res] = o1 + o2;
			break;
		case DOUBLE: m_temporal.m_d[current.res] = o1 + o2;
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case INT: m_local.m_i[current.res] = o1 + o2;
			break;
		case DOUBLE: m_local.m_d[current.res] = o1 + o2;
			break;
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::substration(){
	double o1, o2;
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;

	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = m_local.m_i[p]; 
			break;
		case DOUBLE: o2 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case INT: m_temporal.m_i[current.res] = o1 - o2;
			break;
		case DOUBLE: m_temporal.m_d[current.res] = o1 - o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case INT: m_local.m_i[current.res] = o1 - o2;
			break;
		case DOUBLE: m_local.m_d[current.res] = o1 - o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::multiplication(){
	double o1, o2;
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = m_local.m_i[p]; 
			break;
		case DOUBLE: o2 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case INT: m_temporal.m_i[current.res] = o1 * o2;
			break;
		case DOUBLE: m_temporal.m_d[current.res] = o1 * o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case INT: m_local.m_i[current.res] = o1 * o2;
			break;
		case DOUBLE: m_local.m_d[current.res] = o1 * o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::division(){
	double o1, o2;
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 = m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = m_local.m_i[p]; 
			break;
		case DOUBLE: o2 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	if(o2 != 0.0){
		switch (memory_type(current.res)){
		case TEMPORAL: 
			switch(mvalue_type(current.res, TEMPORAL)){
			case INT: m_temporal.m_i[current.res] = o1 / o2;
				break;
			case DOUBLE: m_temporal.m_d[current.res] = o1 / o2;
				break;
			default:
				error(1);
				break;
			}
			break;
		case LOCAL: 
			switch(mvalue_type(current.res, LOCAL)){
			case INT: m_local.m_i[current.res] = o1 / o2;
				break;
			case DOUBLE: m_local.m_d[current.res] = o1 / o2;
				break;
			default:
				error(1);
				break;
			}
			break;
		default:
			error(0);
			break;
		}
	}else{
		error(2);
	}
}

void VirtualM::module(){
	double o1, o2;	
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = m_local.m_i[p]; 
			break;
		case DOUBLE: o2 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case INT: m_temporal.m_i[current.res] = (int)o1 % (int)o2;
			break;
		case DOUBLE: m_temporal.m_d[current.res] = (int)o1 % (int)o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case INT: m_local.m_i[current.res] = (int)o1 % (int)o2;
			break;
		case DOUBLE: m_local.m_d[current.res] = (int)o1 % (int)o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::and(){
	bool o1, o2;	
	int  p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case BOOLEAN: o1 = m_global.m_b[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case BOOLEAN: o1 = m_local.m_b[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case BOOLEAN: o1 =m_temporal.m_b[current.op1];
			break;

		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case BOOLEAN: o1 = m_constante.m_b[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case BOOLEAN: o1 = m_local.m_b[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case BOOLEAN: o2 = m_global.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case BOOLEAN: o2 = m_local.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case BOOLEAN: o2 = m_temporal.m_b[current.op2];
			break;		
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case BOOLEAN: o2 = m_constante.m_i[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case BOOLEAN: o2 = m_local.m_b[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case BOOLEAN: m_temporal.m_b[current.res] = o1 && o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case BOOLEAN: m_local.m_b[current.res] = o1 && o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::or(){
	bool o1, o2;
	int  p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case BOOLEAN: o1 = m_global.m_b[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case BOOLEAN: o1 = m_local.m_b[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case BOOLEAN: o1 =m_temporal.m_b[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case BOOLEAN: o1 = m_constante.m_b[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case BOOLEAN: o1 = m_local.m_b[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case BOOLEAN: o2 = m_global.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case BOOLEAN: o2 = m_local.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case BOOLEAN: o2 = m_temporal.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case BOOLEAN: o2 = m_constante.m_i[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case BOOLEAN: o2 = m_local.m_b[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case BOOLEAN: m_temporal.m_b[current.res] = o1 || o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case BOOLEAN: m_local.m_b[current.res] = o1 || o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::less_than(){
	double o1, o2;
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = m_local.m_i[p]; 
			break;
		case DOUBLE: o2 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case BOOLEAN: m_temporal.m_b[current.res] = (o1 < o2);
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case BOOLEAN: m_local.m_b[current.res] = o1 < o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::greater_than(){
	double o1, o2;
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = m_local.m_i[p]; 
			break;
		case DOUBLE: o2 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case BOOLEAN: m_temporal.m_b[current.res] = o1 > o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL:
		switch(mvalue_type(current.res, LOCAL)){
		case BOOLEAN: m_local.m_b[current.res] = o1 > o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::equal_equal(){
	double o1, o2;
	bool ob1, ob2;
	string os1, os2;
	int p = 0;
	int t = -1;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			t = 1;
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			t = 1;
			break;
		case STRING: os1 = m_global.m_s[current.op1];
			t = 2;
			break;
		case BOOLEAN: ob1 = m_global.m_b[current.op1];
			t = 3;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			t = 1;
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			t = 1;
			break;
		case STRING: os1 = m_local.m_s[current.op1];
			t = 2;
			break;
		case BOOLEAN: ob1 = m_local.m_b[current.op1];
			t = 3;
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			t = 1;
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			t = 1;
			break;
		case STRING: os1 = m_temporal.m_s[current.op1];
			t = 2;
			break;
		case BOOLEAN: ob1 = m_temporal.m_b[current.op1];
			t = 3;
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			t = 1;
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			t = 1;
			break;
		case STRING: os1 = m_constante.m_s[current.op1];
			t = 2;
			break;
		case BOOLEAN: ob1 = m_constante.m_b[current.op1];
			t = 3;
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			t = 1;
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			t = 1;
			break;
		case STRING: os1 = m_local.m_s[p];
			t = 2;
			break;
		case BOOLEAN: ob1 = m_local.m_b[p];
			t = 3;
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: os2 = m_global.m_s[current.op2];
			break;
		case BOOLEAN: ob2 = m_global.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: os2 = m_local.m_s[current.op2];
			break;
		case BOOLEAN: ob2 = m_local.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: os2 = m_temporal.m_s[current.op2];
			break;
		case BOOLEAN: ob2 = m_temporal.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: os2 = m_constante.m_s[current.op2];
			break;
		case BOOLEAN: ob2 = m_constante.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = m_local.m_i[p]; 
			break;
		case DOUBLE: o2 = m_local.m_d[p]; 
			break;
		case STRING: os2 = m_local.m_s[p];
			break;
		case BOOLEAN: ob2 = m_local.m_b[p];
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	bool resB;
	if(t == 1){
		resB = (o1 == o2);
	}else if(t == 2){
		resB = (os1.compare(os2) == 0)? true : false ;
	}else{
		resB = (ob1 == ob2);
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case BOOLEAN: m_temporal.m_b[current.res] = resB;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case BOOLEAN: m_local.m_b[current.res] = resB;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::diference(){
	double o1, o2;
	bool ob1, ob2;
	string os1, os2;
	int p = 0;
	int t = -1;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			t = 1;
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			t = 1;
			break;
		case STRING: os1 = m_global.m_s[current.op1];
			t = 2;
			break;
		case BOOLEAN: ob1 = m_global.m_b[current.op1];
			t = 3;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			t = 1;
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			t = 1;
			break;
		case STRING: os1 = m_local.m_s[current.op1];
			t = 2;
			break;
		case BOOLEAN: ob1 = m_local.m_b[current.op1];
			t = 3;
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			t = 1;
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			t = 1;
			break;
		case STRING: os1 = m_temporal.m_s[current.op1];
			t = 2;
			break;
		case BOOLEAN: ob1 = m_temporal.m_b[current.op1];
			t = 3;
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			t = 1;
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			t = 1;
			break;
		case STRING: os1 = m_constante.m_s[current.op1];
			t = 2;
			break;
		case BOOLEAN: ob1 = m_constante.m_b[current.op1];
			t = 3;
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			t = 1;
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			t = 1;
			break;
		case STRING: os1 = m_local.m_s[p];
			t = 2;
			break;
		case BOOLEAN: ob1 = m_local.m_b[p];
			t = 3;
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		case STRING: os2 = m_global.m_s[current.op2];
			break;
		case BOOLEAN: ob2 = m_global.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		case STRING: os2 = m_local.m_s[current.op2];
			break;
		case BOOLEAN: ob2 = m_local.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		case STRING: os2 = m_temporal.m_s[current.op2];
			break;
		case BOOLEAN: ob2 = m_temporal.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		case STRING: os2 = m_constante.m_s[current.op2];
			break;
		case BOOLEAN: ob2 = m_constante.m_b[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = m_local.m_i[p]; 
			break;
		case DOUBLE: o2 = m_local.m_d[p]; 
			break;
		case STRING: os2 = m_local.m_s[p];
			break;
		case BOOLEAN: ob2 = m_local.m_b[p];
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	bool resB;
	if(t == 1){
		resB = (o1 != o2);
	}else if(t == 2){
		resB = (os1.compare(os2) != 0)? true : false ;
	}else{
		resB = (ob1 != ob2);
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case BOOLEAN: m_temporal.m_b[current.res] = resB;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case BOOLEAN: m_local.m_b[current.res] = resB;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::greater_equal(){
	double o1, o2;	
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = m_local.m_i[p]; 
			break;
		case DOUBLE: o2 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case BOOLEAN: m_temporal.m_b[current.res] = o1 >= o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case BOOLEAN: m_local.m_b[current.res] = o1 >= o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::less_equal(){
	double o1, o2;
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = m_global.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_global.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = m_local.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_local.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = m_temporal.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_temporal.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = m_constante.m_i[current.op2];
			break;
		case DOUBLE: o2 = m_constante.m_d[current.op2];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = m_local.m_i[p]; 
			break;
		case DOUBLE: o2 = m_local.m_d[p]; 
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case BOOLEAN: m_temporal.m_b[current.res] = o1 <= o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case BOOLEAN: m_local.m_b[current.res] = o1 <= o2;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::append(){
	string o1, o2;	
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = ftoa(m_global.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_global.m_d[current.op1]);
			break;
		case STRING: o1 = m_global.m_s[current.op1];
			break;
		case BOOLEAN: o1 = btoa(m_global.m_b[current.op1]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = ftoa(m_local.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_local.m_d[current.op1]);
			break;
		case STRING: o1 = m_local.m_s[current.op1];
			break;
		case BOOLEAN: o1 = btoa(m_local.m_b[current.op1]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 = ftoa(m_temporal.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_temporal.m_d[current.op1]);
			break;
		case STRING: o1 = m_temporal.m_s[current.op1];
			break;
		case BOOLEAN: o1 = btoa(m_temporal.m_b[current.op1]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = ftoa(m_constante.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_constante.m_d[current.op1]);
			break;
		case STRING: o1 = m_constante.m_s[current.op1];
			break;
		case BOOLEAN: o1 = btoa(m_constante.m_b[current.op1]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = ftoa(m_local.m_i[p]); 
			break;
		case DOUBLE: o1 = ftoa(m_local.m_d[p]); 
			break;
		case STRING: o1 = m_local.m_s[p];
			break;
		case BOOLEAN: o1 = btoa(m_local.m_b[p]);
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.op2)){
	case GLOBAL: 
		switch(mvalue_type(current.op2, GLOBAL)){
		case INT: o2 = ftoa(m_global.m_i[current.op2]);
			break;
		case DOUBLE: o2 = ftoa(m_global.m_d[current.op2]);
			break;
		case STRING: o2 = m_global.m_s[current.op2];
			break;
		case BOOLEAN: o2 = btoa(m_global.m_b[current.op2]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op2, LOCAL)){
		case INT: o2 = ftoa(m_local.m_i[current.op2]);
			break;
		case DOUBLE: o2 = ftoa(m_local.m_d[current.op2]);
			break;
		case STRING: o2 = m_local.m_s[current.op2];
			break;
		case BOOLEAN: o2 = btoa(m_local.m_b[current.op2]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op2, TEMPORAL)){
		case INT: o2 = ftoa(m_temporal.m_i[current.op2]);
			break;
		case DOUBLE: o2 = ftoa(m_temporal.m_d[current.op2]);
			break;
		case STRING: o2 = m_temporal.m_s[current.op2];
			break;
		case BOOLEAN: o2 = btoa(m_temporal.m_b[current.op2]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op2, CONSTANT)){
		case INT: o2 = ftoa(m_constante.m_i[current.op2]);
			break;
		case DOUBLE: o2 = ftoa(m_constante.m_d[current.op2]);
			break;
		case STRING: o2 = m_constante.m_s[current.op2];
			break;
		case BOOLEAN: o2 = btoa(m_constante.m_b[current.op2]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op2];
		switch (mvalue_type(p, LOCAL)){
		case INT: o2 = ftoa(m_local.m_i[p]); 
			break;
		case DOUBLE: o2 = ftoa(m_local.m_d[p]); 
			break;
		case STRING: o2 = m_local.m_s[p];
			break;
		case BOOLEAN: o2 = btoa(m_local.m_b[p]);
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case STRING: o1.append(o2);
			m_temporal.m_s[current.res] = o1;
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case STRING: o1.append(o2);
			m_local.m_s[current.res] = o1;
			break;
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}	
}

void VirtualM::asign(){
	string os1;	
	double od1;	
	bool ob1;
	int p = 0;

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
			error(1);
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
			error(1);
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
			error(1);
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
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: od1 = m_local.m_i[p]; 
			break;
		case DOUBLE: od1 = m_local.m_d[p]; 
			break;
		case STRING: os1 = m_local.m_s[p];
			break;
		case BOOLEAN: ob1 = m_local.m_b[p];
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
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
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case INT: m_local.m_i[current.res] = od1;
			break;
		case DOUBLE: m_local.m_d[current.res] = od1;
			break;
		case STRING: m_local.m_s[current.res] = os1;
			break;
		case BOOLEAN: m_local.m_b[current.res] = ob1;
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
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
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.res, CONSTANT)){
		case INT: m_constante.m_i[current.res] = od1;
			break;
		case DOUBLE: m_constante.m_d[current.res] = od1;
			break;
		case STRING: m_constante.m_s[current.res] = os1;
			break;
		case BOOLEAN: m_constante.m_b[current.res] = ob1;
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.res];
		switch(mvalue_type(p, LOCAL)){
		case INT: m_local.m_i[p] = od1;
			break;
		case DOUBLE: m_local.m_d[p] = od1;
			break;
		case STRING: m_local.m_s[p] = os1;
			break;
		case BOOLEAN: m_local.m_b[p] = ob1;
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}
}

void VirtualM::go_to_true(){
	bool cond = NULL;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case BOOLEAN: cond = m_global.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case BOOLEAN: cond = m_local.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case BOOLEAN: cond =m_temporal.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case BOOLEAN: cond = m_constante.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	if(cond){
		cIt = current.res - 1;
	}
}

void VirtualM::go_to_false(){
	bool cond = NULL;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case BOOLEAN: cond = m_global.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case BOOLEAN: cond = m_local.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case BOOLEAN: cond =m_temporal.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case BOOLEAN: cond = m_constante.m_b[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case INT: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	if(!cond){
		cIt = current.res - 1;
	}
}

void VirtualM::go_to(){
	cIt = current.op1 - 1 ;
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
				error(1);
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
				error(1);
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
				error(1);
				break;
			}
			break;
		case CONSTANT: 
		case ERROR:
		default:
			error(0);
			break;
		}
	}catch (exception& e){
		error(1);
	}
}

void VirtualM::write(){
	string o1 = "";
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = ftoa(m_global.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_global.m_d[current.op1]);
			break;
		case STRING: o1 = m_global.m_s[current.op1];
			break;
		case BOOLEAN: o1 = btoa(m_global.m_b[current.op1]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = ftoa(m_local.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_local.m_d[current.op1]);
			break;
		case STRING: o1 = m_local.m_s[current.op1];
			break;
		case BOOLEAN: o1 = btoa(m_local.m_b[current.op1]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 = ftoa(m_temporal.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_temporal.m_d[current.op1]);
			break;
		case STRING: o1 = m_temporal.m_s[current.op1];
			break;
		case BOOLEAN: o1 = btoa(m_temporal.m_b[current.op1]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = ftoa(m_constante.m_i[current.op1]);
			break;
		case DOUBLE: o1 = ftoa(m_constante.m_d[current.op1]);
			break;
		case STRING: o1 = m_constante.m_s[current.op1];
			break;
		case BOOLEAN: o1 = btoa(m_constante.m_b[current.op1]);
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = ftoa(m_local.m_i[p]); 
			break;
		case DOUBLE: o1 = ftoa(m_local.m_d[p]); 
			break;
		case STRING: o1 = m_local.m_s[p];
			break;
		case BOOLEAN: o1 = btoa(m_local.m_b[p]);
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}
	cout << o1 << '\n';
}

void VirtualM::era(){
	tLocal = Memoria();
	tTemp = Memoria();
	cmli += m_local.m_i.size();
	cmld += m_local.m_d.size();
	cmls += m_local.m_s.size();
	cmlb += m_local.m_b.size();
	cmti += m_temporal.m_i.size();
	cmtd += m_temporal.m_d.size();
	cmts += m_temporal.m_s.size();
	cmtb += m_temporal.m_b.size();	
}

void VirtualM::go_sub(){
	sLocal.push(m_local);
	sTemp.push(m_temporal);
	m_local = tLocal;
	m_temporal = tTemp;
	slines.push(cIt);
	cIt = current.op1 - 1;
}

void VirtualM::end_module(){
	if(sLocal.size() > 0) {
		cmli += m_local.m_i.size();
		cmld += m_local.m_d.size();
		cmls += m_local.m_s.size();
		cmlb += m_local.m_b.size();
		m_local = sLocal.top();
		sLocal.pop();
	}else{
		error(3);
	}

	if(sTemp.size() > 0) {
		cmti += m_temporal.m_i.size();
		cmtd += m_temporal.m_d.size();
		cmts += m_temporal.m_s.size();
		cmtb += m_temporal.m_b.size();
		m_temporal = sTemp.top();
		sTemp.pop();
	}else{
		error(3);
	}

	if(slines.size() > 0) {
		cIt = slines.top();
		slines.pop();
	}else{
		error(3);
	}
}

void VirtualM::return_value(){
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
			error(1);
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
			error(1);
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
			error(1);
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
			error(1);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
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
		default: error(1);
			break;
		}
		break;
	case LOCAL: 
	case TEMPORAL: 
	case CONSTANT: 
	case ERROR:
	default:
		error(0);
		break;
	}
}

void VirtualM::parameter(){
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
			error(1);
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
			error(1);
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
			error(1);
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
			error(1);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case INT: tTemp.m_i[current.res] = od1;
			break;
		case DOUBLE: tTemp.m_d[current.res] = od1;
			break;
		case STRING: tTemp.m_s[current.res] = os1;
			break;
		case BOOLEAN: tTemp.m_b[current.res] = ob1;
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case INT: tLocal.m_i[current.res] = od1;
			break;
		case DOUBLE: tLocal.m_d[current.res] = od1;
			break;
		case STRING: tLocal.m_s[current.res] = os1;
			break;
		case BOOLEAN: tLocal.m_b[current.res] = ob1;
			break;
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case GLOBAL: 
	case CONSTANT: 
	case ERROR:
	default:
		error(0);
		break;
	}
}

void VirtualM::limits_array(){
	int sub;
	int li = current.op2;
	int ls = current.res;

	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: sub = m_global.m_i[current.op1];
			break;
		case DOUBLE:
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: sub = m_local.m_i[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: sub = m_temporal.m_i[current.op1];
			break;
		case DOUBLE:
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: sub = m_constante.m_i[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	if (!(sub > li && sub < ls)){
		error(4);
	}
}

void VirtualM::access(){
	int sub = -1;
	int base = current.op2;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: sub = m_global.m_i[current.op1];
			break;
		case DOUBLE:
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: sub = m_local.m_i[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: sub = m_temporal.m_i[current.op1];
			break;
		case DOUBLE:
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: sub = m_constante.m_i[current.op1];
			break;
		case DOUBLE: 
		case STRING: 
		case BOOLEAN: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case POINTER: 
		m_pointer[current.res] = base + sub;
		break;
	default:
		error(0);
		break;
	}
}

void VirtualM::negation(){
	double o1;	
	bool o;
	int p = 0;
	switch (memory_type(current.op1)){
	case GLOBAL: 
		switch(mvalue_type(current.op1, GLOBAL)){
		case INT: o1 = m_global.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_global.m_d[current.op1];
			break;
		case BOOLEAN: o = m_global.m_b[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.op1, LOCAL)){
		case INT: o1 = m_local.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_local.m_d[current.op1];
			break;
		case BOOLEAN: o = m_local.m_b[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case TEMPORAL: 
		switch(mvalue_type(current.op1, TEMPORAL)){
		case INT: o1 =m_temporal.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_temporal.m_d[current.op1];
			break;
		case BOOLEAN: o = m_temporal.m_b[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case CONSTANT: 
		switch(mvalue_type(current.op1, CONSTANT)){
		case INT: o1 = m_constante.m_i[current.op1];
			break;
		case DOUBLE: o1 = m_constante.m_d[current.op1];
			break;
		case BOOLEAN: o = m_constante.m_b[current.op1];
			break;
		default:
			error(1);
			break;
		}
		break;
	case POINTER: p = m_pointer[current.op1];
		switch (mvalue_type(p, LOCAL)){
		case INT: o1 = m_local.m_i[p]; 
			break;
		case DOUBLE: o1 = m_local.m_d[p]; 
			break;
		case BOOLEAN: o = m_local.m_b[p];
			break;
		default:
			error(5);
			break;
		}
		break;
	case ERROR:
	default:
		error(0);
		break;
	}

	switch (memory_type(current.res)){
	case TEMPORAL: 
		switch(mvalue_type(current.res, TEMPORAL)){
		case INT: m_temporal.m_i[current.res] = o1 * -1;
			break;
		case DOUBLE: m_temporal.m_d[current.res] = o1 * -1;
			break;
		case BOOLEAN: m_temporal.m_b[current.res] = !o;
			break;
		case STRING: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	case LOCAL: 
		switch(mvalue_type(current.res, LOCAL)){
		case INT: m_local.m_i[current.res] = o1 * -1;
			break;
		case DOUBLE: m_local.m_d[current.res] = o1 * -1;
			break;
		case BOOLEAN: m_local.m_b[current.res] = !o;
			break;
		case STRING: 
		case ERROR:
		default:
			error(1);
			break;
		}
		break;
	default:
		error(0);
		break;
	}
}

string VirtualM::strClean(string s){
	return s.substr(s.find_first_of('"')+1, s.find_last_of('"')-1);
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

void VirtualM::error(int e) {
	string desc = "";
	switch (e) {
	case 0: desc = "Memory out of bounds."; break;
	case 1: desc = "Type mismatch."; break;
	case 2: desc = "Division by zero."; break;
	case 3: desc = "Memory Error."; break;
	case 4: desc = "Array out of bounds."; break;
	case 5: desc = "Memory Array error."; break;
	case 99: desc = "Unexpected error."; break;
	default:
		break;
	}
	cout << desc << '\n';
	exit(EXIT_FAILURE);
}