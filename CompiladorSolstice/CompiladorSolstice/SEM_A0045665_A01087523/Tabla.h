#include <stdio.h>
#include <map>
#include <string>
#include <wchar.h>

using namespace std;

class Vis {
public:
	enum {
		PRIVATE = 0,
		PROCTECT = 1,
		PUBLIC = 2
	};
};

class Type{
public:
	enum {
		INT = 0,
		DOUBLE = 1,
		STRING = 2,
		BOOLEAN = 3,
		OBJECT = 4
	};
};

class Variable {
public: 
	int var_vis;
	int var_type;
	int var_dim;
	
	Variable(int v, int t, int d){
		var_vis = v;
		var_type = t;
		var_dim = d;
	};
};

typedef std::map<string , Variable> VMAP;

class Metodo {
public:
	int m_vis;
	int m_type;
	VMAP vars;
	Metodo(){
		m_vis = 0;
		m_type = 0;
	};
	Metodo(int v, int t){
		m_vis = v;
		m_type = t;
	};
};

typedef std::map<string, Metodo> SMAP;

class Tabla {
public:
	SMAP publico;
	SMAP privado;
	SMAP protegido;
	string conv(wchar_t* x){
		std::wstring ws(x);
		string s(ws.begin(), ws.end());
		s.assign(ws.begin(), ws.end());
		return s;
	};
};
