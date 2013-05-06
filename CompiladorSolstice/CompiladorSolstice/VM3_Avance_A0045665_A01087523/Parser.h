

#if !defined(COCO_PARSER_H__)
#define COCO_PARSER_H__

#include "Tabla.h"
#include "Cubo.h"
#include "Memoria.h"
#include <iostream>
#include <sstream>


#include "Scanner.h"



class Errors {
public:
	int count;			// number of errors detected

	Errors();
	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_idV=1,
		_idM=2,
		_idO=3,
		_idC=4,
		_cteI=5,
		_cteD=6,
		_cteS=7,
		_cteB=8,
		_tCla=9,
		_tPub=10,
		_tPri=11,
		_tPro=12,
		_tRet=13,
		_tStr=14,
		_tInt=15,
		_tDou=16,
		_tBoo=17,
		_tNew=18,
		_tVoi=19,
		_tCon=20,
		_tEls=21,
		_tEif=22,
		_tWhi=23,
		_tExt=24,
		_tMai=25,
		_tLec=26,
		_tEsc=27,
		_tInc=28,
		_tDec=29,
		_tMay=30,
		_tMen=31,
		_tDif=32,
		_tEqu=33,
		_comentarios=56
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

CTABLE dirProc;
TABLE dirGral;
CVariable paramx;
Cubo c;
stack<int> oper;
stack<CVariable> operandos;
stack<int> saltos;
stack<int> ifs;
CTS cts;
vector<CVariable> t_params;
vector<Cuadruplo> gen;

int type;
int ctype;
int vis;
int dim;
int availNum;
int gi;
int gd;
int gb;
int gs;
int li;
int ld;
int lb;
int ls;
int ti;
int td;
int tb;
int ts;
int tp;
int mo;
int ci;
int cd;
int cb;
int cs;
string name;
string className;
bool err;

string conv(wchar_t* x){
	std::wstring ws(x);
	string s(ws.begin(), ws.end());
	s.assign(ws.begin(), ws.end());
	return s;
};



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void Solstice();
	void Class();
	void Main();
	void Arr();
	void Arr2();
	void ExpOY();
	void Asig();
	void Asig2();
	void Atributo();
	void Vis();
	void Tipo();
	void Bloque();
	void Constructor();
	void MetodoR();
	void MetodoV();
	void Ciclo();
	void Estatuto();
	void Con();
	void Con2();
	void ConG();
	void Param();
	void Init();
	void CTE();
	void CTES();
	void Decl();
	void New();
	void Esc();
	void Llamada();
	void Metodo();
	void Lec();
	void Exp();
	void Termino();
	void Expresion();
	void Factor();
	void Lista();
	void Return();

	void Parse();

}; // end Parser



#endif

