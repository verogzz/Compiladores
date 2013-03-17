

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"




void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }
		if (la->kind == _comentarios) {
		}

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::Solstice() {
		Class();
		Main();
}

void Parser::Class() {
		Expect(_tCla);
		Expect(_idO);
		clase = conv(t->val); 
		if (la->kind == _tExt) {
			Get();
			Expect(_idO);
		}
		Expect(40 /* "{" */);
		Bloque();
		Expect(41 /* "}" */);
		if (la->kind == _tCla) {
			Class();
		}
}

void Parser::Main() {
		Expect(_tMai);
		name = conv(t->val);
		if(dir.find(name) == dir.end()){
		dir.insert(TABLA::value_type(name, Attribute(vis, MAIN, 0, "")));
		vis = type = -1;
		} else {
		cout << "CANNOT DECLARE MORE THAN ONE MAIN." << '\n';
		err = TRUE;
		}
		
		Expect(38 /* "(" */);
		Expect(39 /* ")" */);
		Expect(40 /* "{" */);
		while (StartOf(1)) {
			Decl();
		}
		while (StartOf(2)) {
			Estatuto();
		}
		Expect(41 /* "}" */);
}

void Parser::Arr() {
		Expect(34 /* "[" */);
		if (la->kind == _idV) {
			Get();
		} else if (la->kind == _cteI) {
			Get();
		} else SynErr(56);
		Expect(35 /* "]" */);
}

void Parser::Asig() {
		if (la->kind == _tInc || la->kind == _tDec || la->kind == 37 /* "=" */) {
			if (la->kind == _tInc) {
				Get();
			} else if (la->kind == _tDec) {
				Get();
			} else {
				Asig2();
			}
		} else if (la->kind == 34 /* "[" */) {
			Arr();
			Asig2();
		} else SynErr(57);
		Expect(36 /* ";" */);
}

void Parser::Asig2() {
		Expect(37 /* "=" */);
		Expresion();
}

void Parser::Expresion() {
		Exp();
		if (StartOf(3)) {
			switch (la->kind) {
			case _tMay: {
				Get();
				break;
			}
			case _tMen: {
				Get();
				break;
			}
			case _tDif: {
				Get();
				break;
			}
			case 48 /* "<" */: {
				Get();
				break;
			}
			case 49 /* ">" */: {
				Get();
				break;
			}
			case _tEqu: {
				Get();
				break;
			}
			}
			Exp();
		}
}

void Parser::Atributo() {
		Vis();
		Tipo();
		Expect(_idV);
		name = conv(t->val);
		if(dir.find(name) == dir.end()){
		dir.insert(TABLA::value_type(name, Attribute(vis, type, 0, clase)));
		name = "";
		vis = type = -1;
		} else {
		cout << "PREVIOUSLY DECLARED ATTRIBUTE: " << name << '\n';
		err = TRUE;
		}
		
		Expect(36 /* ";" */);
}

void Parser::Vis() {
		if (la->kind == _tPub) {
			Get();
			vis = PUBLIC; 
		} else if (la->kind == _tPri) {
			Get();
			vis = PRIVATE; 
		} else if (la->kind == _tPro) {
			Get();
			vis = PROTECT; 
		} else SynErr(58);
}

void Parser::Tipo() {
		if (la->kind == _tInt) {
			Get();
			type = INT; 
		} else if (la->kind == _tDou) {
			Get();
			type = DOUBLE; 
		} else if (la->kind == _tStr) {
			Get();
			type = STRING; 
		} else if (la->kind == _tBoo) {
			Get();
			type = BOOLEAN; 
		} else SynErr(59);
}

void Parser::Bloque() {
		Atributo();
		while (la->kind == _tPub || la->kind == _tPri || la->kind == _tPro) {
			Atributo();
		}
		Constructor();
		while (la->kind == _tPub || la->kind == _tPri || la->kind == _tPro) {
			Vis();
			if (StartOf(4)) {
				MetodoR();
			} else if (la->kind == _tVoi) {
				MetodoV();
			} else SynErr(60);
		}
}

void Parser::Constructor() {
		Expect(_idC);
		Expect(38 /* "(" */);
		if (StartOf(4)) {
			Param();
		}
		Expect(39 /* ")" */);
		Expect(40 /* "{" */);
		while (la->kind == _idV) {
			Init();
		}
		Expect(41 /* "}" */);
}

void Parser::MetodoR() {
		Tipo();
		Expect(_idM);
		name = conv(t->val);
		if(dir.find(name) == dir.end()){
		dir.insert(TABLA::value_type(name, Attribute(vis, type, 1, clase)));
		vis = type = -1;
		} else {
		cout << "PREVIOUSLY DECLARED METHOD: " << name << '\n';
		err = TRUE;
		}
		
		Expect(38 /* "(" */);
		if (StartOf(4)) {
			Param();
		}
		Expect(39 /* ")" */);
		Expect(40 /* "{" */);
		while (StartOf(1)) {
			Decl();
		}
		while (StartOf(2)) {
			Estatuto();
		}
		Return();
		Expect(41 /* "}" */);
		name = "";
}

void Parser::MetodoV() {
		Expect(_tVoi);
		Expect(_idM);
		name = conv(t->val);
		if(dir.find(name) == dir.end()){
		dir.insert(TABLA::value_type(name, Attribute(vis, VOID, 1, clase)));
		vis = type = -1;
		} else {
		cout << "PREVIOUSLY DECLARED METHOD: " << name << '\n';
		err = TRUE;
		}
		
		Expect(38 /* "(" */);
		if (StartOf(4)) {
			Param();
		}
		Expect(39 /* ")" */);
		Expect(40 /* "{" */);
		while (StartOf(1)) {
			Decl();
		}
		while (StartOf(2)) {
			Estatuto();
		}
		Expect(41 /* "}" */);
		name = "";
}

void Parser::Ciclo() {
		Expect(_tWhi);
		Expect(38 /* "(" */);
		Expresion();
		Expect(39 /* ")" */);
		Expect(40 /* "{" */);
		while (StartOf(2)) {
			Estatuto();
		}
		Expect(41 /* "}" */);
}

void Parser::Estatuto() {
		switch (la->kind) {
		case _idV: {
			Get();
			if (StartOf(5)) {
				Asig();
			} else if (la->kind == 51 /* "." */) {
				Reg();
			} else SynErr(61);
			break;
		}
		case _idM: {
			Metodo();
			Expect(36 /* ";" */);
			break;
		}
		case _tWhi: {
			Ciclo();
			break;
		}
		case _tEsc: {
			Esc();
			break;
		}
		case _tLec: {
			Lec();
			break;
		}
		case _tCon: {
			ConG();
			break;
		}
		default: SynErr(62); break;
		}
}

void Parser::Con() {
		Expect(_tCon);
		Expect(38 /* "(" */);
		Expresion();
		Expect(39 /* ")" */);
		Expect(40 /* "{" */);
		Estatuto();
		while (StartOf(2)) {
			Estatuto();
		}
		Expect(41 /* "}" */);
		while (la->kind == _tEif) {
			Get();
			Expect(38 /* "(" */);
			Expresion();
			Expect(39 /* ")" */);
			Expect(40 /* "{" */);
			Estatuto();
			while (StartOf(2)) {
				Estatuto();
			}
			Expect(41 /* "}" */);
		}
}

void Parser::Con2() {
		Expect(_tEls);
		Expect(40 /* "{" */);
		Estatuto();
		while (StartOf(2)) {
			Estatuto();
		}
		Expect(41 /* "}" */);
}

void Parser::ConG() {
		Con();
		if (la->kind == _tEls) {
			Con2();
		}
}

void Parser::Param() {
		string nameL; 
		Tipo();
		Expect(_idV);
		nameL = conv(t->val);
		if((dir.find(nameL) == dir.end()) && (dir[name].vars.find(nameL) == dir[name].vars.end())){
		dir[name].vars.insert(VMAP::value_type(nameL, Variable(type, 0)));
		} else {
		cout << "PREVIOUSLY DECLARED VARIABLE: " << nameL << '\n';
		err = TRUE;
		}
		
		if (la->kind == 42 /* "," */) {
			Get();
			Param();
		}
}

void Parser::Init() {
		Expect(_idV);
		Expect(37 /* "=" */);
		CTES();
		Expect(36 /* ";" */);
}

void Parser::CTE() {
		if (la->kind == _cteS) {
			Get();
		} else if (la->kind == _cteI) {
			Get();
		} else if (la->kind == _cteD) {
			Get();
		} else if (la->kind == _cteB) {
			Get();
		} else SynErr(63);
}

void Parser::CTES() {
		if (la->kind == _idV) {
			Get();
		} else if (StartOf(6)) {
			CTE();
		} else SynErr(64);
}

void Parser::Decl() {
		string nameL;
		if (la->kind == _idO) {
			New();
		} else if (StartOf(4)) {
			Tipo();
			Expect(_idV);
			nameL = conv(t->val);
			if(( dir.find(nameL) == dir.end()) && 
			(dir[name].vars.find(nameL) == dir[name].vars.end())){
			dir[name].vars.insert(VMAP::value_type(nameL, Variable(type, 0)));
			} else {
			cout << "PREVIOUSLY DECLARED VARIABLE: " << nameL << '\n';
			err = TRUE;
			}
			
			if (la->kind == 37 /* "=" */) {
				Get();
				CTES();
			} else if (la->kind == 34 /* "[" */) {
				Arr();
			} else SynErr(65);
			while (la->kind == 42 /* "," */) {
				Get();
				Expect(_idV);
				nameL = conv(t->val);
				if( (dir.find(nameL) == dir.end()) && 
				(dir[name].vars.find(nameL) == dir[name].vars.end())){
				dir[name].vars.insert(VMAP::value_type(nameL, Variable(type, 0)));
				} else {
				cout << "PREVIOUSLY DECLARED VARIABLE: " << nameL << '\n';
				err = TRUE;
				}
				
				if (la->kind == 37 /* "=" */) {
					Get();
					CTES();
				} else if (la->kind == 34 /* "[" */) {
					Arr();
					nameL = conv(t->val);
					if((dir.find(nameL) == dir.end()) && (dir[name].vars.find(nameL) == dir[name].vars.end())){
					dir[name].vars.insert(VMAP::value_type(nameL, Variable(type, 1)));
					} else {
					cout << "PREVIOUSLY DECLARED VARIABLE: " << nameL << '\n';
					err = TRUE;
					}
					
				} else SynErr(66);
			}
		} else SynErr(67);
		Expect(36 /* ";" */);
}

void Parser::New() {
		string nameL; 
		Expect(_idO);
		Expect(_idV);
		nameL = conv(t->val);
		if((dir.find(nameL) == dir.end()) && (dir[name].vars.find(nameL) == dir[name].vars.end())){
		dir[name].vars.insert(VMAP::value_type(nameL, Variable(4, 0)));
		} else {
		cout << "PREVIOUSLY DECLARED VARIABLE: " << nameL << '\n';
		err = TRUE;
		}
		
		Expect(37 /* "=" */);
		Expect(_tNew);
		Expect(_idC);
		Expect(38 /* "(" */);
		if (la->kind == _idV) {
			Lista();
		}
		Expect(39 /* ")" */);
}

void Parser::Esc() {
		Expect(_tEsc);
		Expect(38 /* "(" */);
		Expresion();
		while (la->kind == 43 /* "^" */) {
			Get();
			Expresion();
		}
		Expect(39 /* ")" */);
		Expect(36 /* ";" */);
}

void Parser::Reg() {
		Expect(51 /* "." */);
		if (la->kind == _idM) {
			Metodo();
		} else if (la->kind == _idV) {
			Get();
		} else SynErr(68);
}

void Parser::Metodo() {
		Expect(_idM);
		Expect(38 /* "(" */);
		if (la->kind == _idV) {
			Lista();
		}
		Expect(39 /* ")" */);
}

void Parser::Lec() {
		Expect(_tLec);
		Expect(38 /* "(" */);
		Expect(_idV);
		Expect(39 /* ")" */);
		Expect(36 /* ";" */);
}

void Parser::Exp() {
		Termino();
		if (la->kind == 44 /* "+" */ || la->kind == 45 /* "-" */) {
			if (la->kind == 44 /* "+" */) {
				Get();
			} else {
				Get();
			}
			Termino();
		}
}

void Parser::Termino() {
		Factor();
		if (la->kind == 52 /* "*" */ || la->kind == 53 /* "/" */ || la->kind == 54 /* "%" */) {
			if (la->kind == 52 /* "*" */) {
				Get();
			} else if (la->kind == 53 /* "/" */) {
				Get();
			} else {
				Get();
			}
			Factor();
		}
}

void Parser::ExpOY() {
		Expresion();
		if (la->kind == 46 /* "&" */ || la->kind == 47 /* "|" */) {
			if (la->kind == 46 /* "&" */) {
				Get();
			} else {
				Get();
			}
			Expresion();
		}
}

void Parser::Factor() {
		if (la->kind == 45 /* "-" */ || la->kind == 50 /* "~" */) {
			if (la->kind == 50 /* "~" */) {
				Get();
			} else {
				Get();
			}
		}
		if (la->kind == 38 /* "(" */) {
			Get();
			ExpOY();
			Expect(39 /* ")" */);
		} else if (la->kind == _idV) {
			Get();
			if (la->kind == 34 /* "[" */ || la->kind == 51 /* "." */) {
				if (la->kind == 51 /* "." */) {
					Reg();
				} else {
					Arr();
				}
			}
		} else if (StartOf(6)) {
			CTE();
		} else if (la->kind == _idM) {
			Metodo();
		} else SynErr(69);
}

void Parser::Lista() {
		Expect(_idV);
		if (la->kind == 42 /* "," */) {
			Get();
			Lista();
		}
}

void Parser::Return() {
		Expect(_tRet);
		Expresion();
		Expect(36 /* ";" */);
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	Solstice();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 55;

	//ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors();
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[7][57] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,x,x,T, x,x,x,x, x,x,x,x, x,x,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, T,x,x,T, x,x,T,T, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, T,T,x,x, x,x,x,x, x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, T,T,x,x, x,x,T,x, x,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,x,x,x, x,T,T,T, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"idV expected"); break;
			case 2: s = coco_string_create(L"idM expected"); break;
			case 3: s = coco_string_create(L"idO expected"); break;
			case 4: s = coco_string_create(L"idC expected"); break;
			case 5: s = coco_string_create(L"cteI expected"); break;
			case 6: s = coco_string_create(L"cteD expected"); break;
			case 7: s = coco_string_create(L"cteS expected"); break;
			case 8: s = coco_string_create(L"cteB expected"); break;
			case 9: s = coco_string_create(L"tCla expected"); break;
			case 10: s = coco_string_create(L"tPub expected"); break;
			case 11: s = coco_string_create(L"tPri expected"); break;
			case 12: s = coco_string_create(L"tPro expected"); break;
			case 13: s = coco_string_create(L"tRet expected"); break;
			case 14: s = coco_string_create(L"tStr expected"); break;
			case 15: s = coco_string_create(L"tInt expected"); break;
			case 16: s = coco_string_create(L"tDou expected"); break;
			case 17: s = coco_string_create(L"tBoo expected"); break;
			case 18: s = coco_string_create(L"tNew expected"); break;
			case 19: s = coco_string_create(L"tVoi expected"); break;
			case 20: s = coco_string_create(L"tCon expected"); break;
			case 21: s = coco_string_create(L"tEls expected"); break;
			case 22: s = coco_string_create(L"tEif expected"); break;
			case 23: s = coco_string_create(L"tWhi expected"); break;
			case 24: s = coco_string_create(L"tExt expected"); break;
			case 25: s = coco_string_create(L"tMai expected"); break;
			case 26: s = coco_string_create(L"tLec expected"); break;
			case 27: s = coco_string_create(L"tEsc expected"); break;
			case 28: s = coco_string_create(L"tInc expected"); break;
			case 29: s = coco_string_create(L"tDec expected"); break;
			case 30: s = coco_string_create(L"tMay expected"); break;
			case 31: s = coco_string_create(L"tMen expected"); break;
			case 32: s = coco_string_create(L"tDif expected"); break;
			case 33: s = coco_string_create(L"tEqu expected"); break;
			case 34: s = coco_string_create(L"\"[\" expected"); break;
			case 35: s = coco_string_create(L"\"]\" expected"); break;
			case 36: s = coco_string_create(L"\";\" expected"); break;
			case 37: s = coco_string_create(L"\"=\" expected"); break;
			case 38: s = coco_string_create(L"\"(\" expected"); break;
			case 39: s = coco_string_create(L"\")\" expected"); break;
			case 40: s = coco_string_create(L"\"{\" expected"); break;
			case 41: s = coco_string_create(L"\"}\" expected"); break;
			case 42: s = coco_string_create(L"\",\" expected"); break;
			case 43: s = coco_string_create(L"\"^\" expected"); break;
			case 44: s = coco_string_create(L"\"+\" expected"); break;
			case 45: s = coco_string_create(L"\"-\" expected"); break;
			case 46: s = coco_string_create(L"\"&\" expected"); break;
			case 47: s = coco_string_create(L"\"|\" expected"); break;
			case 48: s = coco_string_create(L"\"<\" expected"); break;
			case 49: s = coco_string_create(L"\">\" expected"); break;
			case 50: s = coco_string_create(L"\"~\" expected"); break;
			case 51: s = coco_string_create(L"\".\" expected"); break;
			case 52: s = coco_string_create(L"\"*\" expected"); break;
			case 53: s = coco_string_create(L"\"/\" expected"); break;
			case 54: s = coco_string_create(L"\"%\" expected"); break;
			case 55: s = coco_string_create(L"??? expected"); break;
			case 56: s = coco_string_create(L"invalid Arr"); break;
			case 57: s = coco_string_create(L"invalid Asig"); break;
			case 58: s = coco_string_create(L"invalid Vis"); break;
			case 59: s = coco_string_create(L"invalid Tipo"); break;
			case 60: s = coco_string_create(L"invalid Bloque"); break;
			case 61: s = coco_string_create(L"invalid Estatuto"); break;
			case 62: s = coco_string_create(L"invalid Estatuto"); break;
			case 63: s = coco_string_create(L"invalid CTE"); break;
			case 64: s = coco_string_create(L"invalid CTES"); break;
			case 65: s = coco_string_create(L"invalid Decl"); break;
			case 66: s = coco_string_create(L"invalid Decl"); break;
			case 67: s = coco_string_create(L"invalid Decl"); break;
			case 68: s = coco_string_create(L"invalid Reg"); break;
			case 69: s = coco_string_create(L"invalid Factor"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	wprintf(L"-- line %d col %d: %ls\n", line, col, s);
}

void Errors::Warning(const wchar_t *s) {
	wprintf(L"%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	wprintf(L"%ls", s); 
	exit(1);
}


