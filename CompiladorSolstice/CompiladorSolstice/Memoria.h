/* Listado de prototipos para la estructura de memoria
 * Autores:		Jorge Salazar Saltijeral				A00945665
 *				Veronica Alejandra Gonzalez Gonzalez	A01087523
 * Compiladores Enero - Mayo 2013
 */
#ifndef MEMORIA_H
#define MEMORIA_H
#include <stdio.h>
#include <string>
#include <map>
#include <wchar.h>


using namespace std;
//Rangos de memoria
#define bgi 0		//Base global int
#define bgd 2000	//Base global double
#define bgs 4000	//Base global string
#define bgb 6000	//Base global boolean
#define bli 8000	//Base local int
#define bld 10000	//Base local double
#define bls 12000	//Base local string
#define blb 14000	//Base local boolean
#define bti 16000	//Base temporal int
#define btd 18000	//Base temporal double
#define bts 20000	//Base temporal string
#define btb 22000	//Base temporal boolean
#define bci 24000	//Base constante int
#define bcd 25000	//Base constante double
#define bcs 26000	//Base constante string
#define bcb 27000	//Base constante boolean
#define btp 27002	//Base temporal pointer
#define bmo 28000	//Base memoria objetos
#define lgi 1999	//Limite global int
#define lgd 3999	//Limite global double
#define lgs 5999	//Limite global string
#define lgb 7999	//Limite global boolean
#define lli 9999	//Limite local int
#define lld 11999	//Limite local double
#define lls 13999	//Limite local string
#define llb 15999	//Limite local boolean
#define lti 17999	//Limite temporal int
#define ltd 19999	//Limite temporal double
#define lts 21999	//Limite temporal string
#define ltb 23999	//Limite temporal boolean
#define lci 24999	//Limite constante int
#define lcd 25999	//Limite constante double
#define lcs 26999	//Limite constante string
#define lcb 27001	//Limite constante boolean
#define ltp 27999	//Limite temporal pointer
#define lmo 39999	//Limite memoria objeto

//Bloque de memoria
class Memoria{
public:
	map<int, string> m_s; //Registros de strings
	map<int, int> m_i;	//Registros de int
	map<int, bool> m_b; //Registros de boolean
	map<int, double> m_d; //Registros de double
	//Constructor
	Memoria();
	//Destructor
	~Memoria();
};

#endif