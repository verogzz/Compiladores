#ifndef MEMORIA_H
#define MEMORIA_H
#include <stdio.h>
#include <string>
#include <map>
#include <wchar.h>


using namespace std;

#define bgi 0
#define bgd 2000
#define bgs 4000
#define bgb 6000
#define bli 8000
#define bld 10000
#define bls 12000
#define blb 14000
#define bti 16000
#define btd 18000
#define bts 20000
#define btb 22000
#define bci 24000
#define bcd 25000
#define bcs 26000
#define bcb 27000
#define btp 27002
#define bmo 28000
#define lgi 1999
#define lgd 3999
#define lgs 5999
#define lgb 7999
#define lli 9999
#define lld 11999
#define lls 13999
#define llb 15999
#define lti 17999
#define ltd 19999
#define lts 21999
#define ltb 23999
#define lci 24999
#define lcd 25999
#define lcs 26999
#define lcb 27001
#define ltp 27999
#define lmo 39999

class Memoria{
public:
	map<int, string> m_s;
	map<int, int> m_i;
	map<int, bool> m_b;
	map<int, double> m_d;
	Memoria();
	~Memoria();
};

#endif