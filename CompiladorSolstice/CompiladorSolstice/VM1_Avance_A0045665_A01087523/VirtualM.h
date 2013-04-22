#ifndef VIRTUALM_H
#define VIRTUALM_H

#define GLOBAL 0
#define LOCAL 1
#define TEMPORAL 2
#define CONSTANT 3
#define ERROR 4

#include <vector>
#include <stdio.h>
#include <iostream>
#include "Tabla.h"
#include "Memoria.h"

using namespace std;

class VirtualM {
public:
	Cuadruplo current;
	TABLE dirClasses;
	CTS dirCtes;
	vector<Cuadruplo> prog;
	Memoria m_global;
	Memoria m_local;
	Memoria m_temporal;
	Memoria m_constante;
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
};

#endif