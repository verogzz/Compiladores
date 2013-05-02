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
	Cuadruplo current;
	TABLE dirClasses;
	CTS dirCtes;	
	vector<Cuadruplo> prog;
	Memoria m_global;
	Memoria m_local;
	Memoria m_temporal;
	Memoria m_constante;
	map<int, int> m_pointer;
	map<int, int> m_obj;
	unsigned int cIt;
	stack<int> slines;
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
private:
	stack<Memoria> sTemp;
	stack<Memoria> sLocal;
	Memoria tTemp;
	Memoria tLocal;
	string strClean(string s);
	string ftoa(float f);
	string btoa(bool b);
	void error(int e);
};

#endif