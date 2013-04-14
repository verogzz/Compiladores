#ifndef MEMORIA_H
#define MEMORIA_H
#include <stdio.h>
#include <string>
#include <vector>
#include <wchar.h>


using namespace std;

class Memoria{
public:
	vector<string> m_s;
	vector<int> m_i;
	vector<bool> m_b;
	vector<double> m_d;
	Memoria();
};

#endif