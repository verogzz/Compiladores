#ifndef MEMORIA_H
#define MEMORIA_H
#include <stdio.h>
#include <string>
#include <map>
#include <wchar.h>


using namespace std;

#define bgi 0
#define bgd 1000
#define bgs 2000
#define bgb 3000
#define bli 4000
#define bld 6000
#define bls 8000
#define blb 10000
#define bti 12000
#define btd 14000
#define bts 16000
#define btb 18000
#define bci 20000
#define bcd 21000
#define bcs 22000
#define bcb 23000
#define lgi 999
#define lgd 1999
#define lgs 2999
#define lgb 3999
#define lli 5999
#define lld 7999
#define lls 9999
#define llb 11999
#define lti 13999
#define ltd 15999
#define lts 17999
#define ltb 19999
#define lci 20999
#define lcd 21999
#define lcs 22999
#define lcb 23999

class Memoria{
public:
	map<int, string> m_s;
	map<int, int> m_i;
	map<int, bool> m_b;
	map<int, double> m_d;
	Memoria();
};

#endif