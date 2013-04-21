

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
		c = Cubo(); 
		availNum = 0;
		type = ctype = vis = dim = -1;
		name = "";
		gi = gd = gb = gs = li = ld = lb = ls = ti = td = tb = ts = ci = cd = cb = cs = 0;
		className = "";
		err = false;
		cts.insert(CTS::value_type("1", Constantes(INT, bci + ci)));
		ci++;
		Class();
		Main();
}

void Parser::Class() {
		string ext = "";
		className = "";
		dirProc.clear();
		Expect(_tCla);
		Expect(_idO);
		className = conv(t->val); 
		if (la->kind == _tExt) {
			Get();
			Expect(_idO);
			ext = conv(t->val);
			if(dirGral.find(ext) != dirGral.end()){
			for(CTABLE::iterator i = dirGral[ext].attributes.begin() ; 
			i != dirGral[ext].attributes.end(); 
			i++){
			if(i->second.att_vis != PRIVATE){
			dirProc.insert(CTABLE::value_type(i->first, i->second));
			}
			}
			}else{
			cout << "PARENT CLASS NOT DECLARED!\n";
			err = true;
			}
		}
		Expect(40 /* "{" */);
		Bloque();
		Expect(41 /* "}" */);
		if(dirGral.find(className) == dirGral.end()){
		dirGral.insert(TABLE::value_type(className, ClassStruct(ext, dirProc)));
		}else{
		cout << "CLASS PREVIOUSLY DECLARED!\n";
		}
		if (la->kind == _tCla) {
			Class();
		}
}

void Parser::Main() {
		Expect(_tMai);
		name = conv(t->val);
		if(dirProc.find(name) == dirProc.end()){
		dirProc.insert(CTABLE::value_type(name, Attribute(3, MAIN, 0)));
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
		if(dirGral.find("main") == dirGral.end()){
		dirGral.insert(TABLE::value_type("main", ClassStruct("", dirProc)));
		}
}

void Parser::Arr() {
		Expect(34 /* "[" */);
		Expect(_cteI);
		dim = atoi(&conv(t->val)[0]);
		if(dim < 2){
		cout << "ARRAY SIZE MUST BE GREATER THAN ONE!\n";
		}
		Expect(35 /* "]" */);
}

void Parser::Arr2() {
		Expect(34 /* "[" */);
		if(o1.var_dim < 2){
		cout << "VARIABLE IS NOT AN ARRAY!\n";	
		err = true;
		}
		ExpOY();
		if(!operandos.empty()){
		o2 = operandos.top();
		operandos.pop();
		if(o2.var_type == INT){
		gen.push_back(Cuadruplo(VER, o2.dir, -1, o1.var_dim));
		int mem = bti + ti;
		if(mem > lti){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		ti++;
		gen.push_back(Cuadruplo(SUM, o2.dir, o1.dir, mem));
		o1.var_dim = 0;
		}else{
		cout << "EXPRESSION MUST BE INT TYPE!\n";	
		err = true;
		}
		}else{
		cout << "ERROR!\n";	
		err = true;
		}
		Expect(35 /* "]" */);
}

void Parser::ExpOY() {
		int mem = 0;
		Expresion();
		while (la->kind == 46 /* "&" */ || la->kind == 47 /* "|" */) {
			if(!oper.empty() && (oper.top() == AND || 
			oper.top() == OR)){
			CVariable o2 = operandos.top();
			operandos.pop();
			CVariable o1 = operandos.top();
			operandos.pop();
			int op = oper.top();
			oper.pop();
			if(c.cubo[o1.var_type][o2.var_type][op] != -1&& 
			o1.var_dim < 2 &&
			o1.var_dim == o2.var_dim){
			mem = btb + tb;
			if(mem > ltb){
			cout << "OUT OF MEMORY!\n";
			err = true;
			}
			tb++;
			gen.push_back(Cuadruplo(op, o1.dir, o2.dir, mem));
			operandos.push(CVariable("memoria",c.cubo[o1.var_type][o2.var_type][op], 0, mem));
			availNum++;
			} else {
			cout << "TYPE MISMATCH! :" << o1.name << '\t' << o2.name << '\n';
			err = TRUE;
			}
			}
			if (la->kind == 46 /* "&" */) {
				Get();
				oper.push(AND); 
			} else {
				Get();
				oper.push(OR); 
			}
			Expresion();
		}
		if(!oper.empty() && (oper.top() == AND || 
		oper.top() == OR)){
		CVariable o2 = operandos.top();
		operandos.pop();
		CVariable o1 = operandos.top();
		operandos.pop();
		int op = oper.top();
		oper.pop();
		if(c.cubo[o1.var_type][o2.var_type][op] != -1 && 
		o1.var_dim < 2 &&
		o1.var_dim == o2.var_dim){
		mem = btb + tb;
		if(mem > ltb){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		tb++;
		gen.push_back(Cuadruplo(op, o1.dir, o2.dir, mem));
		operandos.push(CVariable("memoria" ,c.cubo[o1.var_type][o2.var_type][op], 0, mem));
		availNum++;
		} else {
		cout << "TYPE MISMATCH! :" << o1.name << '\t' << o2.name << '\n';
		err = TRUE;
		}
		}
}

void Parser::Asig() {
		if (la->kind == _tInc || la->kind == _tDec || la->kind == 37 /* "=" */) {
			if (la->kind == _tInc) {
				Get();
				if(o1.var_type == INT && o1.var_dim < 2){
				gen.push_back(Cuadruplo(SUM, o1.dir, cts["1"].dir , o1.dir));
				} else {
				cout << "TYPE MISMATCH! :" << o1.name << '\n';
				err = TRUE;
				}
			} else if (la->kind == _tDec) {
				Get();
				if(o1.var_type == INT && o1.var_dim < 2){
				gen.push_back(Cuadruplo(SUB, o1.dir, cts["1"].dir , o1.dir));
				} else {
				cout << "TYPE MISMATCH! :" << o1.name << '\n';
				err = TRUE;
				}
			} else {
				Asig2();
			}
		} else if (la->kind == 34 /* "[" */) {
			Arr2();
			Asig2();
		} else SynErr(56);
		Expect(36 /* ";" */);
}

void Parser::Asig2() {
		Expect(37 /* "=" */);
		ExpOY();
		if(c.cubo[operandos.top().var_type][o1.var_type][ASI] != -1 && 
		o1.var_dim == operandos.top().var_dim){
		gen.push_back(Cuadruplo(ASI, operandos.top().dir, -1 , o1.dir));
		operandos.pop();
		} else {
		cout << "TYPE MISMATCH! :" << o1.name << '\t' << operandos.top().name << '\n';
		operandos.pop();
		err = TRUE;
		}
}

void Parser::Atributo() {
		name = "";
		vis = type = -1;
		Vis();
		Tipo();
		Expect(_idV);
		name = conv(t->val);
		if(dirProc.find(name) == dirProc.end()){
		int mem = 0;
		switch (type){
		case INT:	mem = bgi + gi;
		if(mem > lgi){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		gi++;
		break;
		case DOUBLE:	mem = bgd + gd;
		if(mem > lgd){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		gd++;
		break;
		case STRING:	mem = bgs + gs;
		if(mem > lgs){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		gs++;
		break;
		case BOOLEAN:	mem = bgb + gb;
		if(mem > lgb){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		gb++;
		break;
		}
		dirProc.insert(CTABLE::value_type(name, Attribute(vis, type, 0, mem)));
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
		} else SynErr(57);
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
		} else SynErr(58);
}

void Parser::Bloque() {
		Atributo();
		while (la->kind == _tPub || la->kind == _tPri || la->kind == _tPro) {
			Atributo();
		}
		Constructor();
		while (la->kind == _tPub || la->kind == _tPri || la->kind == _tPro) {
			Vis();
			if (StartOf(3)) {
				MetodoR();
			} else if (la->kind == _tVoi) {
				MetodoV();
			} else SynErr(59);
		}
}

void Parser::Constructor() {
		name = "";
		vis = type = -1;
		Expect(_idC);
		name = conv(t->val);
		if(dirProc.find(name) == dirProc.end()){
		dirProc.insert(CTABLE::value_type(name, Attribute(PUBLIC, OBJECT, 1)));
		} else {
		cout << "CANNOT DECLARE MORE THAN ONE CONSTRUCTOR." << '\n';
		err = TRUE;
		}
		Expect(38 /* "(" */);
		if (StartOf(3)) {
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
		vis = type = -1;
		li = ld = lb = ls = ti = td = tb = ts = 0;
		Tipo();
		Expect(_idM);
		name = conv(t->val);
		if(dirProc.find(name) == dirProc.end()){
		dirProc.insert(CTABLE::value_type(name, Attribute(vis, type, 1, gen.size())));
		} else {
		cout << "PREVIOUSLY DECLARED METHOD: " << name << '\n';
		err = TRUE;
		}
		
		Expect(38 /* "(" */);
		if (StartOf(3)) {
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
		gen.push_back(Cuadruplo(RET, -1, -1, -1));
}

void Parser::MetodoV() {
		vis = type = -1;
		li = ld = lb = ls = ti = td = tb = ts = 0;
		Expect(_tVoi);
		Expect(_idM);
		name = conv(t->val);
		if(dirProc.find(name) == dirProc.end()){
		dirProc.insert(CTABLE::value_type(name, Attribute(vis, VOID, 1, gen.size())));
		} else {
		cout << "PREVIOUSLY DECLARED METHOD: " << name << '\n';
		err = TRUE;
		}
		
		Expect(38 /* "(" */);
		if (StartOf(3)) {
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
		gen.push_back(Cuadruplo(RET, -1, -1, -1));
}

void Parser::Ciclo() {
		Expect(_tWhi);
		saltos.push(gen.size());
		Expect(38 /* "(" */);
		ExpOY();
		Expect(39 /* ")" */);
		CVariable boolRes = operandos.top();
		operandos.pop();
		if(boolRes.var_type != BOOLEAN){
		cout << "EXPRESSION MUST BE BOOL TYPE!\n";
		}else{
		gen.push_back(Cuadruplo(GTF, boolRes.dir, -1 , -1));
		saltos.push(gen.size() - 1);
		}
		Expect(40 /* "{" */);
		while (StartOf(2)) {
			Estatuto();
		}
		Expect(41 /* "}" */);
		int falso = saltos.top();
		saltos.pop();
		int ret = saltos.top();
		saltos.pop();
		gen.push_back(Cuadruplo(GTO, ret, -1 , -1));
		gen[falso].res = gen.size();
		
}

void Parser::Estatuto() {
		string temp = "";
		o1.~CVariable();
		o2.~CVariable();
		switch (la->kind) {
		case _idV: {
			Get();
			temp = conv(t->val); 
			if (dirProc[name].vars.find(temp) != dirProc[name].vars.end()){
			o1.name = temp;
			o1.var_dim = dirProc[name].vars[temp].var_dim;
			o1.var_type = dirProc[name].vars[temp].var_type;
			o1.dir = dirProc[name].vars[temp].dir;
			}else if(dirProc.find(temp) != dirProc.end()){
			o1.name = temp;
			o1.var_dim = 0;
			o1.var_type = dirProc[temp].att_type;
			o1.dir = dirProc[temp].dir;
			}else {
			cout << "UNDECLARED VARIABLE: " << temp << '\n';
			err = TRUE;
			}
			if (StartOf(4)) {
				Asig();
			} else if (la->kind == 51 /* "." */) {
				Llamada();
			} else SynErr(60);
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
		default: SynErr(61); break;
		}
}

void Parser::Con() {
		Expect(_tCon);
		ifs.push(1);
		Expect(38 /* "(" */);
		ExpOY();
		Expect(39 /* ")" */);
		CVariable boolRes = operandos.top();
		operandos.pop();
		if(boolRes.var_type != BOOLEAN){
		cout << "EXPRESSION MUST BE BOOL TYPE!\n";
		}else{
		gen.push_back(Cuadruplo(GTF, boolRes.dir, -1 , -1));
		saltos.push(gen.size() - 1);
		}
		Expect(40 /* "{" */);
		Estatuto();
		while (StartOf(2)) {
			Estatuto();
		}
		Expect(41 /* "}" */);
		while (la->kind == _tEif) {
			Get();
			int ps = saltos.top();
			saltos.pop();
			gen.push_back(Cuadruplo(GTO, -1, -1, -1));
			saltos.push(gen.size() - 1);
			gen[ps].res = gen.size();
			Expect(38 /* "(" */);
			ExpOY();
			Expect(39 /* ")" */);
			ifs.top()++;
			CVariable boolRes = operandos.top();
			operandos.pop();
			if(boolRes.var_type != BOOLEAN){
			cout << "EXPRESSION MUST BE BOOL TYPE!\n";
			}else{
			gen.push_back(Cuadruplo(GTF, boolRes.dir, -1, -1));
			saltos.push(gen.size() - 1);
			}
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
		if(gen[saltos.top()].operador != GTO){
		int ps = saltos.top();
		saltos.pop();
		gen.push_back(Cuadruplo(GTO, -1, -1, -1));
		gen[ps].res = gen.size();
		saltos.push(gen.size() - 1);
		}
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
		int gts = ifs.top();
		ifs.pop();
		while(gts > 0){
		int ps = saltos.top();
		saltos.pop();
		if(gen[ps].operador == GTO)
		gen[ps].op1 = gen.size();
		else if (gen[ps].operador == GTF)
		gen[ps].res = gen.size();
		gts--;
		}
}

void Parser::Param() {
		string nameL; 
		Tipo();
		Expect(_idV);
		nameL = conv(t->val);
		if((dirProc.find(nameL) == dirProc.end()) && (dirProc[name].vars.find(nameL) == dirProc[name].vars.end())){
		int mem = 0;
		switch (type){
		case INT:	mem = bli + li;
		if(mem > lli){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		li++;
		break;
		case DOUBLE:	mem = bld + ld;
		if(mem > lld){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		ld++;
		break;
		case STRING:	mem = bls + ls;
		if(mem > lls){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		ls++;
		break;
		case BOOLEAN:	mem = blb + lb;
		if(mem > llb){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		lb++;
		break;
		}
		dirProc[name].vars.insert(VMAP::value_type(nameL, Variable(type, 0, mem)));
		dirProc[name].params.push_back(type);
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
		string temp = conv(t->val); 
		if(dirProc.find(temp) != dirProc.end()){
		o1.name = temp;
		o1.var_dim = 0;
		o1.var_type = dirProc[temp].att_type;
		o1.dir = dirProc[temp].dir;
		}else{
		cout << "UNDECLARED VARIABLE: " << temp << '\n';
		err = TRUE;
		}
		Expect(37 /* "=" */);
		CTES();
		if(c.cubo[o2.var_type][o1.var_type][ASI] != -1 && 
		o1.var_dim == o2.var_dim){
		gen.push_back(Cuadruplo(ASI,o2.dir, -1, o1.dir));
		} else {
		cout << "TYPE MISMATCH! :" << o1.name << '\t' << o2.name << '\n';
		err = TRUE;
		}
		Expect(36 /* ";" */);
}

void Parser::CTE() {
		ctype = -1;
		if (la->kind == _cteS) {
			Get();
			ctype = 2; 
		} else if (la->kind == _cteI) {
			Get();
			ctype = 0; 
		} else if (la->kind == _cteD) {
			Get();
			ctype = 1; 
		} else if (la->kind == _cteB) {
			Get();
			ctype = 3; 
		} else SynErr(62);
		string c = conv(t->val);
		if(cts.find(c) == cts.end()){
		int mem = 0;
		switch (ctype){
		case INT:	mem = bci + ci;
				if(mem > lci){
					cout << "OUT OF MEMORY!\n";
					err = true;
				}
				ci++;
				break;
		case DOUBLE:	mem = bcd + cd;
				if(mem > lcd){
					cout << "OUT OF MEMORY!\n";
					err = true;
				}
				cd++;
				break;
		case STRING:	mem = bcs + cs;
				if(mem > lcs){
					cout << "OUT OF MEMORY!\n";
					err = true;
				}
				cs++;
				break;
		case BOOLEAN:	mem = bcb + cb;
				if(mem > lcb){
					cout << "OUT OF MEMORY!\n";
					err = true;
				}
				cb++;
				break;
		}
		cts.insert(CTS::value_type(c, Constantes(ctype, mem)));
		}
}

void Parser::CTES() {
		string temp = ""; 
		if (la->kind == _idV) {
			Get();
			temp = conv(t->val);
			if(dirProc[name].vars.find(temp) != dirProc[name].vars.end()){
			o2.name = temp;
			o2.var_dim = dirProc[name].vars[temp].var_dim;
			o2.var_type = dirProc[name].vars[temp].var_type;
			o2.dir = dirProc[name].vars[temp].dir;
			} else if(dirProc.find(temp) != dirProc.end()){
			o2.name = temp;
			o2.var_dim = 0;
			o2.var_type = dirProc[name].att_type;
			o2.dir = dirProc[name].dir;
			} else {
			cout << "UNDECLARED VARIABLE." << '\n';
			err = TRUE;
			}
		} else if (StartOf(5)) {
			CTE();
			temp = conv(t->val);
			o2.name = temp;
			o2.var_dim = 0;
			o2.var_type = ctype;
			o2.dir = cts[temp].dir;
			
		} else SynErr(63);
}

void Parser::Decl() {
		string nameL; dim = 0;
		if (la->kind == _idO) {
			New();
		} else if (StartOf(3)) {
			Tipo();
			Expect(_idV);
			nameL = conv(t->val); int mem = 0;
			if (la->kind == 37 /* "=" */) {
				Get();
				CTES();
			} else if (la->kind == 34 /* "[" */) {
				Arr();
			} else SynErr(64);
			if(dirProc[name].vars.find(nameL) == dirProc[name].vars.end()){
			switch (type){
			case INT:	mem = bli + li;
			if(mem > lli){
			cout << "OUT OF MEMORY!\n";
			err = true;
			}
			li++;
			break;
			case DOUBLE:	mem = bld + ld;
			if(mem > lld){
			cout << "OUT OF MEMORY!\n";
			err = true;
			}
			ld++;
			break;
			case STRING:	mem = bls + ls;
			if(mem > lls){
			cout << "OUT OF MEMORY!\n";
			err = true;
			}
			ls++;
			break;
			case BOOLEAN:	mem = blb + lb;
			if(mem > llb){
			cout << "OUT OF MEMORY!\n";
			err = true;
			}
			lb++;
			break;
			}
			dirProc[name].vars.insert(VMAP::value_type(nameL, Variable(type, dim, mem)));
			} else {
			cout << "PREVIOUSLY DECLARED VARIABLE: " << nameL << '\n';
			err = TRUE;
			}
			if(dim == 0){
			if(c.cubo[o2.var_type][type][ASI] != -1){
			gen.push_back(Cuadruplo(ASI, o2.dir, -1, mem));
			} else {
			cout << "TYPE MISMATCH! :" << nameL << '\t' << o2.name << '\n';
			err = TRUE;
			}
			}
			while (la->kind == 42 /* "," */) {
				Get();
				Expect(_idV);
				nameL = conv(t->val); dim = 0; mem = 0;
				if (la->kind == 37 /* "=" */) {
					Get();
					CTES();
				} else if (la->kind == 34 /* "[" */) {
					Arr();
				} else SynErr(65);
				if(dirProc[name].vars.find(nameL) == dirProc[name].vars.end()){
				switch (type){
				case INT:	mem = bli + li;
				if(mem > lli){
				cout << "OUT OF MEMORY!\n";
				err = true;
				}
				li++;
				break;
				case DOUBLE:	mem = bld + ld;
				if(mem > lld){
				cout << "OUT OF MEMORY!\n";
				err = true;
				}
				ld++;
				break;
				case STRING:	mem = bls + ls;
				if(mem > lls){
				cout << "OUT OF MEMORY!\n";
				err = true;
				}
				ls++;
				break;
				case BOOLEAN:	mem = blb + lb;
				if(mem > llb){
				cout << "OUT OF MEMORY!\n";
				err = true;
				}
				lb++;
				break;
				}
				dirProc[name].vars.insert(VMAP::value_type(nameL, Variable(type, dim, mem)));
				} else {
				cout << "PREVIOUSLY DECLARED VARIABLE: " << nameL << '\n';
				err = TRUE;
				}
				if(dim == 0){
				if(c.cubo[o2.var_type][type][ASI] != -1){
				gen.push_back(Cuadruplo(ASI, o2.dir, -1, mem));
				} else {
				cout << "TYPE MISMATCH! :" << nameL << '\t' << o2.name << '\n';
				err = TRUE;
				}
				}
			}
		} else SynErr(66);
		Expect(36 /* ";" */);
}

void Parser::New() {
		string nameL; 
		Expect(_idO);
		Expect(_idV);
		nameL = conv(t->val);
		if((dirProc.find(nameL) == dirProc.end()) && (dirProc[name].vars.find(nameL) == dirProc[name].vars.end())){
		dirProc[name].vars.insert(VMAP::value_type(nameL, Variable(4, 0, lcb)));
		} else {
		cout << "PREVIOUSLY DECLARED VARIABLE: " << nameL << '\n';
		err = TRUE;
		}
		
		Expect(37 /* "=" */);
		Expect(_tNew);
		Expect(_idC);
		Expect(38 /* "(" */);
		if (StartOf(6)) {
			Lista();
		}
		Expect(39 /* ")" */);
}

void Parser::Esc() {
		Expect(_tEsc);
		Expect(38 /* "(" */);
		ExpOY();
		while (la->kind == 43 /* "^" */) {
			if(!oper.empty() && oper.top() == APP){
			CVariable o2 = operandos.top();
			operandos.pop();
			CVariable o1 = operandos.top();
			operandos.pop();
			int op = oper.top();
			oper.pop();
			if(c.cubo[o1.var_type][o2.var_type][op] != -1 && o1.var_dim == o2.var_dim){
			int mem = bts + ts;
			if(mem > lts){
			cout << "OUT OF MEMORY!\n";
			err = true;
			}
			ts++;
			gen.push_back(Cuadruplo(op, o1.dir, o2.dir, mem));
			operandos.push(CVariable("memoria" ,c.cubo[o1.var_type][o2.var_type][op], 0, mem));
			availNum++;
			} else {
			cout << "TYPE MISMATCH! :" << o1.name << '\t' << o2.name << '\n';
			err = TRUE;
			}
			}
			Get();
			oper.push(APP);
			ExpOY();
		}
		if(!oper.empty() && oper.top() == APP){
		CVariable o2 = operandos.top();
		operandos.pop();
		CVariable o1 = operandos.top();
		operandos.pop();
		int op = oper.top();
		oper.pop();
		if(c.cubo[o1.var_type][o2.var_type][op] != -1 && o1.var_dim == o2.var_dim){
		int mem = bts + ts;
		if(mem > lts){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		ts++;
		gen.push_back(Cuadruplo(op, o1.dir, o2.dir, mem));
		operandos.push(CVariable("memoria" ,c.cubo[o1.var_type][o2.var_type][op], 0, mem));
		availNum++;
		} else {
		cout << "TYPE MISMATCH! :" << o1.name << '\t' << o2.name << '\n';
		err = TRUE;
		}
		}
		Expect(39 /* ")" */);
		CVariable strRes = operandos.top();
		operandos.pop();
		if(strRes.var_type != STRING && strRes.var_dim < 2){
		cout << "EXPRESSION MUST BE STRING TYPE!\n";
		}else{
		gen.push_back(Cuadruplo(WRI, strRes.dir, -1, -1));
		}
		Expect(36 /* ";" */);
}

void Parser::Llamada() {
		Expect(51 /* "." */);
		if (la->kind == _idM) {
			Metodo();
		} else if (la->kind == _idV) {
			Get();
		} else SynErr(67);
}

void Parser::Metodo() {
		Expect(_idM);
		string name = conv(t->val);
		if(dirProc.find(name) == dirProc.end()){
		cout << "UNDECLARED METHOD: " << name << '\n';
		err = TRUE;
		}
		gen.push_back(Cuadruplo(ERA, -5, -1, -1));
		Expect(38 /* "(" */);
		if (StartOf(6)) {
			Lista();
		}
		if(t_params.size() == dirProc[name].params.size()){
		for(int i = 0; i < dirProc[name].params.size(); i++){
		if (t_params.at(i).var_type != dirProc[name].params.at(i)){
		cout << "PARAMETER TYPES MISMATCH.\n";
		i = dirProc[name].params.size();
		}
		gen.push_back(Cuadruplo(PAR, t_params.at(i).dir, -1 , i+1));
		}
		}else{
		cout << "THERE IS NO METHOD WITH THAT AMOUNT OF PARAMETERS\n";
		}
		t_params.clear();
		Expect(39 /* ")" */);
		gen.push_back(Cuadruplo(GSU, dirProc[name].dir, -1, -1));
}

void Parser::Lec() {
		Expect(_tLec);
		Expect(38 /* "(" */);
		Expect(_idV);
		string temp = conv(t->val); 
		if (dirProc[name].vars.find(temp) != dirProc[name].vars.end()){
		o1.name = temp;
		o1.var_dim = 0;
		o1.var_type = dirProc[name].vars.find(temp)->second.var_type;
		}else if(dirProc.find(temp) != dirProc.end()){
		o1.name = temp;
		o1.var_dim = 0;
		o1.var_type = dirProc.find(temp)->second.att_type;
		}else {
		cout << "UNDECLARED VARIABLE: " << temp << '\n';
		err = TRUE;
		}
		Expect(42 /* "," */);
		Tipo();
		if(o1.var_type == type){
		gen.push_back(Cuadruplo(REA, o1.dir, -1, -1));
		} else {
		cout << "TYPE MISMATCH! :" << o1.name << '\n';
		err = TRUE;
		}
		Expect(39 /* ")" */);
		Expect(36 /* ";" */);
}

void Parser::Exp() {
		Termino();
		while (la->kind == 44 /* "+" */ || la->kind == 45 /* "-" */) {
			if(!oper.empty() && (oper.top() == SUM || 
			oper.top() == SUB)){
			CVariable o2 = operandos.top();
			operandos.pop();
			CVariable o1 = operandos.top();
			operandos.pop();
			int op = oper.top();
			oper.pop();
			if(c.cubo[o1.var_type][o2.var_type][op] != -1 && 
			o1.var_dim < 2 &&
			o1.var_dim == o2.var_dim){
			int mem = 0;
			switch (c.cubo[o1.var_type][o2.var_type][op]){
			case INT:	mem = bti + ti;
			if(mem > lti){
			cout << "OUT OF MEMORY!\n";
			err = true;
			}
			ti++;
			break;
			case DOUBLE:	mem = btd + td;
			if(mem > ltd){
			cout << "OUT OF MEMORY!\n";
			err = true;
			}
			td++;
			break;
			}
			gen.push_back(Cuadruplo(op, o1.dir, o2.dir, mem));
			operandos.push(CVariable("memoria" ,c.cubo[o1.var_type][o2.var_type][op], 0, mem));
			} else {
			cout << "TYPE MISMATCH! :" << o1.name << '\t' << o2.name << '\n';
			err = TRUE;
			}
			}
			if (la->kind == 44 /* "+" */) {
				Get();
				oper.push(SUM); 
			} else {
				Get();
				oper.push(SUB); 
			}
			Termino();
		}
		if(!oper.empty() && (oper.top() == SUM || 
		oper.top() == SUB)){
		CVariable o2 = operandos.top();
		operandos.pop();
		CVariable o1 = operandos.top();
		operandos.pop();
		int op = oper.top();
		oper.pop();
		if(c.cubo[o1.var_type][o2.var_type][op] != -1 && 
		o1.var_dim < 2 &&
		o1.var_dim == o2.var_dim){
		int mem = 0;
		switch (c.cubo[o1.var_type][o2.var_type][op]){
		case INT:	mem = bti + ti;
				if(mem > lti){
					cout << "OUT OF MEMORY!\n";
					err = true;
				}
				ti++;
				break;
		case DOUBLE:	mem = btd + td;
				if(mem > ltd){
					cout << "OUT OF MEMORY!\n";
					err = true;
				}
				td++;
				break;
		}
		gen.push_back(Cuadruplo(op, o1.dir, o2.dir, mem));
		operandos.push(CVariable("memoria" ,c.cubo[o1.var_type][o2.var_type][op], 0, mem));
		availNum++;
		} else {
		cout << "TYPE MISMATCH! :" << o1.name << '\t' << o2.name << '\n';
		err = TRUE;
		}
		}
}

void Parser::Termino() {
		Factor();
		while (la->kind == 52 /* "*" */ || la->kind == 53 /* "/" */ || la->kind == 54 /* "%" */) {
			if(!oper.empty() && (oper.top() == MUL || 
			oper.top() == DIV || 
			oper.top() == MOD)){
			CVariable o2 = operandos.top();
			operandos.pop();
			CVariable o1 = operandos.top();
			operandos.pop();
			int op = oper.top();
			oper.pop();
			if(c.cubo[o1.var_type][o2.var_type][op] != -1 && 
			o1.var_dim < 2 &&
			o1.var_dim == o2.var_dim){
			int mem = 0;
			switch (c.cubo[o1.var_type][o2.var_type][op]){
			case INT:	mem = bti + ti;
			if(mem > lti){
			cout << "OUT OF MEMORY!\n";
			err = true;
			}
			ti++;
			break;
			case DOUBLE:	mem = btd + td;
			if(mem > ltd){
			cout << "OUT OF MEMORY!\n";
			err = true;
			}
			td++;
			break;
			}
			gen.push_back(Cuadruplo(op, o1.dir, o2.dir, mem));
			operandos.push(CVariable("memoria" ,c.cubo[o1.var_type][o2.var_type][op], 0, mem));
			availNum++;
			} else {
			cout << "TYPE MISMATCH! :" << o1.name << '\t' << o2.name << '\n';
			err = TRUE;
			}
			}
			
			if (la->kind == 52 /* "*" */) {
				Get();
				oper.push(MUL); 
			} else if (la->kind == 53 /* "/" */) {
				Get();
				oper.push(DIV); 
			} else {
				Get();
				oper.push(MOD); 
			}
			Factor();
		}
		if(!oper.empty() && (oper.top() == MUL || 
		oper.top() == DIV || 
		oper.top() == MOD)){
		CVariable o2 = operandos.top();
		operandos.pop();
		CVariable o1 = operandos.top();
		operandos.pop();
		int op = oper.top();
		oper.pop();
		if(c.cubo[o1.var_type][o2.var_type][op] != -1 && 
		o1.var_dim < 2 &&
		o1.var_dim == o2.var_dim){
		int mem = 0;
		switch (c.cubo[o1.var_type][o2.var_type][op]){
		case INT:	mem = bti + ti;
			if(mem > lti){
				cout << "OUT OF MEMORY!\n";
				err = true;
			}
			ti++;
			break;
		case DOUBLE:	mem = btd + td;
			if(mem > ltd){
				cout << "OUT OF MEMORY!\n";
				err = true;
			}
			td++;
			break;
		}
		gen.push_back(Cuadruplo(op, o1.dir, o2.dir, mem));
		operandos.push(CVariable("memoria" ,c.cubo[o1.var_type][o2.var_type][op], 0, mem));
		availNum++;
		} else {
		cout << "TYPE MISMATCH! :" << o1.name << '\t' << o2.name << '\n';
		err = TRUE;
		}
		}
}

void Parser::Expresion() {
		int mem = 0;
		Exp();
		if (StartOf(7)) {
			if(!oper.empty() && (oper.top() == GEQ || 
			oper.top() == LEQ || oper.top() == DIF ||
			oper.top() == LT || oper.top() == GT ||
			oper.top() == EQ)){
			CVariable o2 = operandos.top();
			operandos.pop();
			CVariable o1 = operandos.top();
			operandos.pop();
			int op = oper.top();
			oper.pop();
			if(c.cubo[o1.var_type][o2.var_type][op] != -1 && 
			o1.var_dim < 2 &&
			o1.var_dim == o2.var_dim){
			mem = btb + tb;
			if(mem > ltb){
			cout << "OUT OF MEMORY!\n";
			err = true;
			}
			tb++;
			gen.push_back(Cuadruplo(op, o1.dir, o2.dir, mem));
			operandos.push(CVariable("memoria" ,c.cubo[o1.var_type][o2.var_type][op], 0, mem));
			availNum++;
			} else {
			cout << "TYPE MISMATCH! :" << o1.name << '\t' << o2.name << '\n';
			err = TRUE;
			}
			}
			switch (la->kind) {
			case _tMay: {
				Get();
				oper.push(GEQ); 
				break;
			}
			case _tMen: {
				Get();
				oper.push(LEQ); 
				break;
			}
			case _tDif: {
				Get();
				oper.push(DIF); 
				break;
			}
			case 48 /* "<" */: {
				Get();
				oper.push(LT); 
				break;
			}
			case 49 /* ">" */: {
				Get();
				oper.push(GT); 
				break;
			}
			case _tEqu: {
				Get();
				oper.push(EQ); 
				break;
			}
			}
			Exp();
		}
		if(!oper.empty() && (oper.top() == GEQ || 
		oper.top() == LEQ || oper.top() == DIF ||
		oper.top() == LT || oper.top() == GT ||
		oper.top() == EQ)){
		CVariable o2 = operandos.top();
		operandos.pop();
		CVariable o1 = operandos.top();
		operandos.pop();
		int op = oper.top();
		oper.pop();
		if(c.cubo[o1.var_type][o2.var_type][op] != -1 && 
		o1.var_dim < 2 &&
		o1.var_dim == o2.var_dim){
		mem = btb + tb;
		if(mem > ltb){
		cout << "OUT OF MEMORY!\n";
		err = true;
		}
		tb++;
		gen.push_back(Cuadruplo(op, o1.dir, o2.dir, mem));
		operandos.push(CVariable("memoria" ,c.cubo[o1.var_type][o2.var_type][op], 0, mem));
		availNum++;
		} else {
		cout << "TYPE MISMATCH! :" << o1.name << '\t' << o2.name << '\n';
		err = TRUE;
		}
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
			oper.push(99);
			ExpOY();
			Expect(39 /* ")" */);
			oper.pop();
		} else if (la->kind == _idV) {
			Get();
			string temp = conv(t->val);
			if(dirProc[name].vars.find(temp) != dirProc[name].vars.end()){
			operandos.push(CVariable(temp, dirProc[name].vars[temp].var_type, dirProc[name].vars[temp].var_dim, dirProc[name].vars[temp].dir));
			} else if(dirProc.find(temp) != dirProc.end()){
			operandos.push(CVariable(temp, dirProc[temp].att_type, dirProc[temp].att_mtd, dirProc[temp].dir));
			} else {
			cout << "UNDECLARED VARIABLE: " << temp << '\n';
			err = true;
			}
			if (la->kind == 34 /* "[" */ || la->kind == 51 /* "." */) {
				if (la->kind == 51 /* "." */) {
					Llamada();
				} else {
					Arr2();
				}
			}
		} else if (StartOf(5)) {
			CTE();
			string temp = conv(t->val);
			operandos.push(CVariable(temp, ctype, 0, cts[temp].dir));
			
		} else if (la->kind == _idM) {
			Metodo();
		} else SynErr(68);
}

void Parser::Lista() {
		CTES();
		t_params.push_back(o2);
		if (la->kind == 42 /* "," */) {
			Get();
			Lista();
		}
}

void Parser::Return() {
		Expect(_tRet);
		ExpOY();
		if(operandos.size() != 0){
		if(dirProc[name].att_type == operandos.top().var_type && 
		operandos.top().var_dim < 2){
		gen.push_back(Cuadruplo(MR, operandos.top().dir, -1, -1));
		operandos.pop();
		}else{
		cout << "RETURN VALUE IS NOT THE EXPECTED TYPE!\n";
		err = true;
		}
		}else{
		err = true;
		}
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

	static bool set[8][57] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,x,x,T, x,x,x,x, x,x,x,x, x,x,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, T,x,x,T, x,x,T,T, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, T,T,x,x, x,x,T,x, x,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,x,x,x, x,T,T,T, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,T,x,x, x,T,T,T, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, T,T,x,x, x,x,x,x, x}
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
			case 56: s = coco_string_create(L"invalid Asig"); break;
			case 57: s = coco_string_create(L"invalid Vis"); break;
			case 58: s = coco_string_create(L"invalid Tipo"); break;
			case 59: s = coco_string_create(L"invalid Bloque"); break;
			case 60: s = coco_string_create(L"invalid Estatuto"); break;
			case 61: s = coco_string_create(L"invalid Estatuto"); break;
			case 62: s = coco_string_create(L"invalid CTE"); break;
			case 63: s = coco_string_create(L"invalid CTES"); break;
			case 64: s = coco_string_create(L"invalid Decl"); break;
			case 65: s = coco_string_create(L"invalid Decl"); break;
			case 66: s = coco_string_create(L"invalid Decl"); break;
			case 67: s = coco_string_create(L"invalid Llamada"); break;
			case 68: s = coco_string_create(L"invalid Factor"); break;

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


