class Prog {
	public int prueba;
	$Prog(){
	
	}
	
	public int #factI(int _x){
		int _r = 1;
		@(_x > 0){
			_r = _r * _x;
			_x--;	/* Equivale a _x = _x - 1; */
		}
		return _r;
	}
	
	/*Factorial recursivo*/
	public int #factR(int _x){
		?(_x == 1){
			return _x;
		}:{
			return _x * #factR(_x - 1);
		}
	}
}	
	main(){
		int _num1 = 0, _num2 = 0;
		Prog1 p = new $Prog1();
		<<("Dame un numero!!!!");
		>>(_num1);
		
		?(_num1 == 0){
			<<("ERROR!!!!");
		}:?(_num1 > 10){
			_num2 = p.#factI(_num1);
		}:{
			_num2 = p.#factR(_num1);
		}
		<<("Resultado: " ^ _num2);		
	}
