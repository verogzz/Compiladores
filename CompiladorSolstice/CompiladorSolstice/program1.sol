class Prog {
public int _prueba;
private int _xyz;
$Prog(){

}

public int #factI(int _x){
int _r = 1;
double _r = 0;
@(_x > 0){
_r = _r * _x;
_x--;	/* Equivale a _x = _x - 1; */
}
return _r;
}
	
/*Factorial recursivo*/
public int #factR(int _x){
int _y = 0;
?(_x <> 1){
_y = _x + 1;
_x = _x * #factR(_y);
}
return _x;
}

protect void #main(){
}
}
	
main(){
int _num1 = 0, _num2 = 0, _p = 2;
Prog1 _p = new $Prog1();
<<("Dame un numero!!!!");
>>(_num1);

?(_num1 == 0){
<<("ERROR!!!!");
}:?(_num1 > 10){
_num2 = _p.#factI(_num1);
}:{
_num2 = _p.#factR(_num1);
}
<<("Resultado: " ^ _num2);		
}
