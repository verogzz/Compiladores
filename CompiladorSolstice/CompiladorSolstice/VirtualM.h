/* Listado de prototipos para la maquina virtual
 * Autores:		Jorge Salazar Saltijeral				A00945665
 *				Veronica Alejandra Gonzalez Gonzalez	A01087523
 * Compiladores Enero - Mayo 2013
 */

#ifndef VIRTUALM_H
#define VIRTUALM_H

#define GLOBAL 0
#define LOCAL 1
#define TEMPORAL 2
#define CONSTANT 3
#define POINTER 4
#define OBJECTS 5
#define ERROR 6

#include <vector>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <exception>
#include "Tabla.h"
#include "Memoria.h"

using namespace std;

class VirtualM {
public:
	Cuadruplo current;	//Cuadruplo actualmente en ejecucion
	TABLE dirClasses;	//Directorio de clases
	CTS dirCtes;		//Directorio de constantes
	vector<Cuadruplo> prog;	//Listado de cuadruplos
	Memoria m_global;	//Memoria Global
	Memoria m_local;	//Memoria Local
	Memoria m_temporal;	//Memoria Temporal
	Memoria m_constante;	//Memoria Constantes
	map<int, int> m_pointer; //Memoria Temporal Pointer
	map<int, int> m_obj;	//Memoria Objetos
	unsigned int cIt;	//Numero de cuadruplo actual
	stack<int> slines;	//Stack de saltos en cuadruplos
	VirtualM();	
	VirtualM(TABLE dirGral, CTS ctes, vector<Cuadruplo> p);
	int memory_type(int dir);
	int mvalue_type(int dir, int mt);
	void run();
	void sum();
	void substration();
	void multiplication();
	void division();
	void module();
	void and();
	void or();
	void less_than();
	void greater_than();
	void equal_equal();
	void diference();
	void greater_equal();
	void less_equal();
	void append();
	void asign();
	void go_to_true();
	void go_to_false();
	void go_to();
	void read();
	void write();
	void era();
	void go_sub();
	void end_module();
	void return_value();
	void parameter();
	void limits_array();
	void access();
	void negation();
private:
	stack<Memoria> sTemp;	//Pila de ejecucion Memoria Temporal
	stack<Memoria> sLocal;	//Pila de ejecucion Memoria Local
	Memoria tTemp;	//Memoria Auxiliar Temporal
	Memoria tLocal;	//Memoria Auxiliar Local
	int cmli;	//Contador de memoria local int
	int cmld;	//Contador de memoria local double
	int cmls;	//Contador de memoria local string
	int cmlb;	//Contador de memoria local boolean
	int cmti;	//Contador de memoria temporal int
	int cmtd;	//Contador de memoria temporal double
	int cmts;	//Contador de memoria temporal string
	int cmtb;	//Contador de memoria temporal boolean
	string strClean(string s);
	string ftoa(float f);
	string btoa(bool b);
	void error(int e);
};

#endif