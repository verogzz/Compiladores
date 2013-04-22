#include<iostream>
#include "Cubo.h"

Cubo::Cubo(void) {
	/* Cubo[OP1][OP2][OPR] = TIPO
	
	OP#
	INT		 0
	DOUBLE	 1
	STRING	 2
	BOOL	 3
	ERROR	-1

	OPERADOR
	+		0
	-		1
	*		2
	/		3
	%		4
	&		5
	|		6
	<		7
	>		8
	==		9
	<>		10
	>=		11
	<=		12 
	^		13
	=		14*/

	// INT
	cubo[0][0][0] = 0; // int + int = int
	cubo[0][1][0] = -1; // int + double = error
	cubo[0][2][0] = -1; // int + string = error
	cubo[0][3][0] = -1; // int + bool = error
	
	cubo[0][0][1] = 0; // int - int = int
	cubo[0][1][1] = -1; // int - double = error
	cubo[0][2][1] = -1; // int - string = error
	cubo[0][3][1] = -1; // int - bool = error
	
	cubo[0][0][2] = 0; // int * int = int
	cubo[0][1][2] = -1; // int * double = error
	cubo[0][2][2] = -1; // int * string = error
	cubo[0][3][2] = -1; // int * bool = error
	
	cubo[0][0][3] = 0; // int / int = int
	cubo[0][1][3] = -1; // int / double = error
	cubo[0][2][3] = -1; // int / string = error
	cubo[0][3][3] = -1; // int / bool = error

	cubo[0][0][4] = 0; // int % int = int
	cubo[0][1][4] = -1; // int % double = error
	cubo[0][2][4] = -1; // int % string = error
	cubo[0][3][4] = -1; // int % bool = error

	cubo[0][0][5] = -1; // int & int = error
	cubo[0][1][5] = -1; // int & double = error
	cubo[0][2][5] = -1; // int & string = error
	cubo[0][3][5] = -1; // int & bool = error
	
	cubo[0][0][6] = -1; // int | int = error
	cubo[0][1][6] = -1; // int | double = error
	cubo[0][2][6] = -1; // int | string = error
	cubo[0][3][6] = -1; // int | bool = error
	
	cubo[0][0][7] = 3; // int < int = bool
	cubo[0][1][7] = -1; // int < double = error
	cubo[0][2][7] = -1; // int < string = error
	cubo[0][3][7] = -1; // int < bool = error
	
	cubo[0][0][8] = 3; // int > int = bool
	cubo[0][1][8] = -1; // int > double = error
	cubo[0][2][8] = -1; // int > string = error
	cubo[0][3][8] = -1; // int > bool = error
	
	cubo[0][0][9] = 3; // int == int = bool
	cubo[0][1][9] = -1; // int == double = error
	cubo[0][2][9] = -1; // int == string = error
	cubo[0][3][9] = -1; // int == bool = error
	
	cubo[0][0][10] = 3; // int <> int = bool
	cubo[0][1][10] = -1; // int <> double = error
	cubo[0][2][10] = -1; // int <> string = error
	cubo[0][3][10] = -1; // int <> bool = error
	
	cubo[0][0][11] = 3; // int >= int = bool
	cubo[0][1][11] = -1; // int >= double = error
	cubo[0][2][11] = -1; // int >= string = error
	cubo[0][3][11] = -1; // int >= bool = error
	
	cubo[0][0][12] = 3; // int <= int = bool
	cubo[0][1][12] = -1; // int <= double = error
	cubo[0][2][12] = -1; // int <= string = error
	cubo[0][3][12] = -1; // int <= bool = error

	cubo[0][0][13] = 2; // int ^ int = string
	cubo[0][1][13] = 2; // int ^ double = string
	cubo[0][2][13] = 2; // int ^ string = string
	cubo[0][3][13] = 2; // int ^ bool = string

	cubo[0][0][14] = 1; // int = int
	cubo[0][1][14] = -1; // int = double
	cubo[0][2][14] = -1; // int = string
	cubo[0][3][14] = -1; // int = bool
	
	// DOUBLE
	cubo[1][0][0] = -1; // double + int = error
	cubo[1][1][0] = 1; // double + double = double
	cubo[1][2][0] = -1; // double + string = error
	cubo[1][3][0] = -1; // double + bool = error
	
	cubo[1][0][1] = -1; // double - int = error
	cubo[1][1][1] = 1; // double - double = double
	cubo[1][2][1] = -1; // double - string = error
	cubo[1][3][1] = -1; // double - bool = error

	cubo[1][0][2] = -1; // double * int = error
	cubo[1][1][2] = 1; // double * double = double
	cubo[1][2][2] = -1; // double * string = error
	cubo[1][3][2] = -1; // double * bool = error

	cubo[1][0][3] = -1; // double / int = error
	cubo[1][1][3] = 1; // double / double = double
	cubo[1][2][3] = -1; // double / string = error
	cubo[1][3][3] = -1; // double / bool = error
	
	cubo[1][0][4] = -1; // double % int = error
	cubo[1][1][4] = 0; // double % double = int
	cubo[1][2][4] = -1; // double % string = error
	cubo[1][3][4] = -1; // double % bool = error
	
	cubo[1][0][5] = -1; // double & int = error
	cubo[1][1][5] = -1; // double & double = error
	cubo[1][2][5] = -1; // double & string = error
	cubo[1][3][5] = -1; // double & bool = error
	
	cubo[1][0][6] = -1; // double | int = error
	cubo[1][1][6] = -1; // double | double = error
	cubo[1][2][6] = -1; // double | string = error
	cubo[1][3][6] = -1; // double | bool = error
	
	cubo[1][0][7] = -1; // double < int = error
	cubo[1][1][7] = 3; // double < double = bool
	cubo[1][2][7] = -1; // double < string = string
	cubo[1][3][7] = -1; // double < bool = string
	
	cubo[1][0][8] = -1; // double > int = error
	cubo[1][1][8] = 3; // double > double = bool
	cubo[1][2][8] = -1; // double > string = error
	cubo[1][3][8] = -1; // double > bool = error
	
	cubo[1][0][9] = -1; // double == int = error
	cubo[1][1][9] = 3; // double == double = bool
	cubo[1][2][9] = -1; // double == string = error
	cubo[1][3][9] = -1; // double == bool = error
	
	cubo[1][0][10] = -1; // double <> int = error
	cubo[1][1][10] = 3; // double <> double = double
	cubo[1][2][10] = -1; // double <> string = error
	cubo[1][3][10] = -1; // double <> bool = error
	
	cubo[1][0][11] = -1; // double >= int = error
	cubo[1][1][11] = 3; // double >= double = bool
	cubo[1][2][11] = -1; // double >= string = error
	cubo[1][3][11] = -1; // double >= bool = error
	
	cubo[1][0][12] = -1; // double <= int = error
	cubo[1][1][12] = 3; // double <= double = bool
	cubo[1][2][12] = -1; // double <= string = error
	cubo[1][3][12] = -1; // double <= bool = error

	cubo[1][0][13] = 2; // double ^ int = string
	cubo[1][1][13] = 2; // double ^ double = string
	cubo[1][2][13] = 2; // double ^ string = string
	cubo[1][3][13] = 2; // double ^ bool = string

	cubo[1][0][14] = -1; // double = int
	cubo[1][1][14] = 1; // double = double
	cubo[1][2][14] = -1; // double = string
	cubo[1][3][14] = -1; // double = bool
	
	// STRING
	cubo[2][0][0] = -1; // string + int = error
	cubo[2][1][0] = -1; // string + double = error
	cubo[2][2][0] = -1; // string + string = error
	cubo[2][3][0] = -1; // string + bool = error

	cubo[2][0][1] = -1; // string - int = error
	cubo[2][1][1] = -1; // string - double = error
	cubo[2][2][1] = -1; // string - string = error
	cubo[2][3][1] = -1; // string - bool = error

	cubo[2][0][2] = -1; // string * int = error
	cubo[2][1][2] = -1; // string * double = error
	cubo[2][2][2] = -1; // string * string = error
	cubo[2][3][2] = -1; // string * bool = error

	cubo[2][0][3] = -1; // string / int = error
	cubo[2][1][3] = -1; // string / double = error
	cubo[2][2][3] = -1; // string / string = error
	cubo[2][3][3] = -1; // string / bool = error
	
	cubo[2][0][4] = -1; // string % int = error
	cubo[2][1][4] = -1; // string % double = error
	cubo[2][2][4] = -1; // string % string = error
	cubo[2][3][4] = -1; // string % bool = error

	cubo[2][0][5] = -1; // string & int = error
	cubo[2][1][5] = -1; // string & double = error
	cubo[2][2][5] = -1; // string & string = error
	cubo[2][3][5] = -1; // string & bool = error

	cubo[2][0][6] = -1; // string | int = error
	cubo[2][1][6] = -1; // string | double = error
	cubo[2][2][6] = -1; // string | string = error
	cubo[2][3][6] = -1; // string | bool = error

	cubo[2][0][7] = -1; // string < int = error
	cubo[2][1][7] = -1; // string < double = error
	cubo[2][2][7] = -1; // string < string = error
	cubo[2][3][7] = -1; // string < bool = error

	cubo[2][0][8] = -1; // string > int = error
	cubo[2][1][8] = -1; // string > double = error
	cubo[2][2][8] = -1; // string > string = error
	cubo[2][3][8] = -1; // string > bool = error

	cubo[2][0][9] = -1; // string == int = error
	cubo[2][1][9] = -1; // string == double = error
	cubo[2][2][9] = 3; // string == string = bool
	cubo[2][3][9] = -1; // string == bool = error

	cubo[2][0][10] = -1; // string <> int = error
	cubo[2][1][10] = -1; // string <> double = error
	cubo[2][2][10] = 3; // string <> string = bool
	cubo[2][3][10] = -1; // string <> bool = error

	cubo[2][0][11] = -1; // string >= int = error
	cubo[2][1][11] = -1; // string >= double = error
	cubo[2][2][11] = -1; // string >= string = error
	cubo[2][3][11] = -1; // string >= bool = error
	
	cubo[2][0][12] = -1; // string <= int = error
	cubo[2][1][12] = -1; // string <= double = error
	cubo[2][2][12] = -1; // string <= string = error
	cubo[2][3][12] = -1; // string <= bool = error
	
	cubo[2][0][13] = 2; // string ^ int = string
	cubo[2][1][13] = 2; // string ^ double = string
	cubo[2][2][13] = 2; // string ^ string = string
	cubo[2][3][13] = 2; // string ^ bool = string

	cubo[2][0][14] = -1; // string = int
	cubo[2][1][14] = -1; // string = double
	cubo[2][2][14] = 2; // string = string
	cubo[2][3][14] = -1; // string = bool

	// BOOL
	cubo[3][0][0] = -1; // bool + int = error
	cubo[3][1][0] = -1; // bool + double = error
	cubo[3][2][0] = -1; // bool + string = error
	cubo[3][3][0] = -1; // bool + bool = error

	cubo[3][0][1] = -1; // bool - int = error
	cubo[3][1][1] = -1; // bool - double = error
	cubo[3][2][1] = -1; // bool - string = error
	cubo[3][3][1] = -1; // bool - bool = error

	cubo[3][0][2] = -1; // bool * int = error
	cubo[3][1][2] = -1; // bool * double = error
	cubo[3][2][2] = -1; // bool * string = error
	cubo[3][3][2] = -1; // bool * bool = error
	
	cubo[3][0][3] = -1; // bool / int = error
	cubo[3][1][3] = -1; // bool / double = error
	cubo[3][2][3] = -1; // bool / string = error
	cubo[3][3][3] = -1; // bool / bool = error

	cubo[3][0][4] = -1; // bool % int = error
	cubo[3][1][4] = -1; // bool % double = error
	cubo[3][2][4] = -1; // bool % string = error
	cubo[3][3][4] = -1; // bool % bool = error

	cubo[3][0][5] = -1; // bool & int = error
	cubo[3][1][5] = -1; // bool & double = error
	cubo[3][2][5] = -1; // bool & string = error
	cubo[3][3][5] = 3;  // bool & bool = bool
	
	cubo[3][0][6] = -1; // bool | int = error
	cubo[3][1][6] = -1; // bool | double = error
	cubo[3][2][6] = -1; // bool | string = error
	cubo[3][3][6] = 3;  // bool | bool = bool

	cubo[3][0][7] = -1; // bool < int = error
	cubo[3][1][7] = -1; // bool < double = error
	cubo[3][2][7] = -1; // bool < string = error
	cubo[3][3][7] = -1; // bool < bool = error

	cubo[3][0][8] = -1; // bool > int = error
	cubo[3][1][8] = -1; // bool > double = error
	cubo[3][2][8] = -1; // bool > string = error
	cubo[3][3][8] = -1; // bool > bool = error
	
	cubo[3][0][9] = -1; // bool == int = error
	cubo[3][1][9] = -1; // bool == double = error
	cubo[3][2][9] = -1; // bool == string = error
	cubo[3][3][9] = 3; // bool == bool = bool

	cubo[3][0][10] = -1; // bool <> int = error
	cubo[3][1][10] = -1; // bool <> double = error
	cubo[3][2][10] = -1; // bool <> string = error
	cubo[3][3][10] = 3; // bool <> bool = bool

	cubo[3][0][11] = -1; // bool >= int = error
	cubo[3][1][11] = -1; // bool >= double = error
	cubo[3][2][11] = -1; // bool >= string = error
	cubo[3][3][11] = -1; // bool >= bool = error

	cubo[3][0][12] = -1; // bool <= int = error
	cubo[3][1][12] = -1; // bool <= double = error
	cubo[3][2][12] = -1; // bool <= string = error
	cubo[3][3][12] = -1; // bool <= bool = error
	
	cubo[3][0][13] = 2; // bool ^ int = string
	cubo[3][1][13] = 2; // bool ^ double = string
	cubo[3][2][13] = 2; // bool ^ string = string
	cubo[3][3][13] = 2; // bool ^ bool = string

	cubo[3][0][14] = -1; // bool = int
	cubo[3][1][14] = -1; // bool = double
	cubo[3][2][14] = -1; // bool = string
	cubo[3][3][14] = 3; // bool = bool

	Cubo::cubo[1][1][1];
}