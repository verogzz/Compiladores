/* Clase que lleva a cabo la ejecucion y creacion de la maquina virtual
 * Autores:		Jorge Salazar Saltijeral				A00945665
 *				Veronica Alejandra Gonzalez Gonzalez	A01087523
 * Compiladores Enero - Mayo 2013
 */
#include "VirtualM.h"

//Constructor 
VirtualM::VirtualM(){}

//Constructor Vacio
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

/* Funcion que indica que scope de memoria tiene la direccion
 * Param: int direccion de memoria
 * Return: int especificando el scope de memoria.
 */
int VirtualM::memory_type(int dir){
	//Valida que la memoria local y temporal no hayan excedido su rango de memoria
	//al momento de ejecutar diversas funciones
	if(cmli > 2000 || cmld > 2000 || cmls > 2000 || cmlb > 200 ||
		cmti > 2000 || cmtd > 2000 || cmts > 2000 || cmtb > 200){
			error(0);
	}

	//Idenfica que scope de memoria tiene la direccion
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

/* Funcion que regresa que tipo de dato representa la memoria
 * Param: int direccion
 * Param: int scope de memoria
 * Return: int tipo de dato de la memoria
 */
int VirtualM::mvalue_type(int dir, int mt){
	//En base al scope de la memoria valida los rangos de la memoria
	//para identificar el tipo de dato.
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

/* Funcion que recorre la lista de cuadruplos para ejecutarlos
 */
void VirtualM::run(){
	/*For que permite imprimir el listado de cuadruplos.
	for(cIt = 0; cIt < prog.size(); cIt++){
		current = prog.at(cIt);
		cout << cIt << '\t' << current.operador << '\t' << current.op1 << '\t' << current.op2 << '\t' << current.res << '\n';
	}*/
	for(cIt = prog.at(0).op1; cIt < prog.size(); cIt++){
		current = prog.at(cIt);
		//En base al codigo de operacion se llama a la funcion correspondiente.
		switch (current.operador) {
		case SUM : sum(); break; // Lleva a cabo la suma
		case SUB : substration(); break; //Lleva a cabo la resta
		case MUL : multiplication(); break; //Lleva a cabo la multiplicacion
		case DIV : division(); break; //Lleva a cabo la division
		case MOD : module(); break; //Lleva a cabo la module
		case AND : and(); break; //Lleva a cabo la operacion logica AND
		case OR : or(); break; //Lleva a cabo la operacion logica OR
		case LT : less_than(); break; //Lleva a cabo la comparacion menor que
		case GT : greater_than(); break; //Lleva a cabo la compracion mayor que
		case EQ : equal_equal(); break; //Lleva a cabo la igualdad
		case DIF : diference(); break; //Lleva a cabo la diferencia
		case GEQ : greater_equal(); break; //Lleva a cabo la compracion mayor igual que
		case LEQ : less_equal(); break; //Lleva a cabo la compracion menor igual que
		case APP : append(); break; //Lleva a cabo la concatenacion
		case ASI : asign(); break; //Lleva a cabo la asignacion
		case GTT : go_to_true(); break; //Lleva a cabo el salto cuando el valor es TRUE
		case GTF : go_to_false(); break; //Lleva a cabo el salto cuando el valor es FALSE
		case GTO : go_to(); break; //Lleva a cabo el salto de cuadruplos
		case REA : read(); break; //Lleva a cabo la lectura
		case WRI : write(); break; //Lleva a cabo la escritura
		case ERA : era(); break; //Lleva a cabo la preparacion de funciones
		case GSU : go_sub(); break; //Lleva a cabo la ejecucion de la funcion
		case RET : end_module(); break; //Lleva a cabo el fin de la funcion
		case MR : return_value(); break; //Lleva a cabo el registro del valor de retorno
		case PAR : parameter(); break; //Lleva a cabo la asociacion de parametros
		case VER : limits_array(); break; //Lleva a cabo la verficiacion de los limites del arreglo
		case ACC : access();break; //Lleva a cabo la suma de las direcciones de memoria para el acceso de arreglos
		case NOT: negation(); break; //Lleva a cabo la negacion de valores
		case END: exit(0); //Lleva a cabo el fin del programa
		default:
			error(99);
			break;
		}
	}
}

/* El siguiente listado de funciones llevan a cabo las operaciones marcadas por los codigos de operacion.
 * El proceso de las funciones es similar en todas.
 * 1. Obtiene el valor con el que se llevara a cabo la operacion del operando 1. Validando el scope de la memoria y el
 *    tipo de dato que posee.
 * 2. Obtiene el valor con el que se llevara a cabo la operacion del operando 2 en el caso de existir, ya que no todos los
 *    codigos de operacion utilizan dos operandos.
 * 3. Usando el valor registado como operando 3, se guarda el resultado de la operacion en ese registro de memoria.
 * -- Algunos codigos de operacion no usan todos los operandos ya que debido a su funcionalidad solo requieren 1 o 2 operandos
 *    para la ejecucion adecuada. Algunos casos particulares son el Go to, Go To False, Go To True, Era, Go Sub.
 * -- El uso del switch permite buscar el valor de los operandos segun su scope y tipo de dato en base al valor de la memoria.
 *    Dependiendo de la operacion que se llevara acabo se restringen el uso de los casos. La funciones solo marcan los casos con los 
 *	  que debe ejecutar adecuadamente como una validacion adicional de la ejecucion.
 */

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

/* Funcion que limpia los valores String omitiendo las comillas dobles
 * Param: string s String que debe ser limpiado
 * Return: string Mismo string sin las comillas al inicio y final del mismo
 */
string VirtualM::strClean(string s){
	return s.substr(s.find_first_of('"')+1, s.find_last_of('"')-1);
}

/* Funcion que convierte un valor float a string 
 * Param: float f valor numerico 
 * Return: string Valor textual del numero
 */
string VirtualM::ftoa(float f){
	std::ostringstream buff;
	buff << f;
	return buff.str();
}

/* Funcion que convierte un valor boolean a string
 * Param: bool b Valor que se debe castear a string
 * Return: string Valor boolean en formato string
 */
string VirtualM::btoa(bool b){
	if(b)
		return "true";
	else
		return "false";
}

/* Funcion que imprime los errores de ejecucion y termina la misma
 * Param: int e Tipo de error a desplegar
 */
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