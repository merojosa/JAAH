%{	
	#include <stdio.h>
	#include <stdlib.h>
	#include <ctype.h>
	// stuff from flex that bison needs to know about:
	#define YYDEBUG 1
	extern int yylex(void);
	extern int yyparse();
	extern char* strdup(char*);
	extern char* yytext;
	extern FILE *yyin;
	void yyerror(const char *s);
	void printError(char* errormsg, char type);
%}
%define parse.lac full
%error-verbose
%union
{
	int intVal;
	char* string;
}
// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <string> HILERA "Hilera"
%token  FOR "para"
%token  WHILE "mientras"
%token  IF "si"
%token  ELSE "sino"
%token <string> COMPARACION "comparador"
%token  AND_OR "&& o ||"
%token  BOOL "verdadero o falso"
%token <intVal> NUM "numero"
%token  INI "ini"
%token  FIN "fin"
%token  RET "ret"
%token <string> OPERACIONES "simbolos de operaciones matematicas"
%token  ENTRADA "entrada"
%token  SALIDA "salida"
%token <string> ID "identificador"
%token  ERROR
%token  EJECUCION "principal"
%token  PARD ")"
%token  CORD "]"
%token  PARI "("
%token  CORI "["
%token  IGUAL "="
%token  DOSP ":"
%token  COM ","
%token  PUNTO "."
%token  POINTER "->"
%token  PYC ";"
%token  LINK "enlazador de archivos"
%token  JAAH "ruta del archivo"
%%
//This is where the fun begins.

super:
	inicio
	;

inicio:
	concat_links principal
	| principal
	;

principal:
	declarations method principal 
	| declarations method
	;
	
declarations:
	assign PYC declarations
	| ID PYC declarations
	|
	;
	
assign:
	ID IGUAL tipos
	| ID IGUAL operaciones
	| arreglo IGUAL tipos
	| arreglo IGUAL operaciones
	ID IGUAL method_call;
	;
	
concat_links:
	LINK JAAH concat_links
	| LINK JAAH
	;

operaciones:
	operando OPERACIONES operando {if($2[0] == '/' && $2[0] == '/'){printError("Error logico, division por cero",'b');};}
	| operaciones OPERACIONES operando {if($2[0] == '/' && $2[0] == '/'){printError("Error logico, division por cero",'b');};}
	;
	
tipos:
	operando
	| BOOL
	| HILERA
	;
	
operando:
	ID 
	| NUM
	| arreglo
	| ID PUNTO ID
	| POINTER method_call
	| ID NUM {if($2 > -1){yyerror("No se puede poner Identificador Entero");};}
	;

	
instrucciones:
	assign PYC instrucciones
	| SALIDA DOSP tipos PYC instrucciones 
	| rule_for instrucciones
	| rule_ELSE instrucciones
	| rule_IF instrucciones
	| rule_While instrucciones
	| ENTRADA DOSP ID PYC instrucciones
	| ret PYC instrucciones
	| PYC instrucciones
	| method_call PYC instrucciones
	|
	;

method:
	INI method_call DOSP
	instrucciones FIN ID 
	/* $2 no agarra el valor del token que debería, agarra un monton de otras cosas.
	Toncs esto no funciona aún.
	{
		if($2 != $7){
			printf($7);
			printError("El identificador de inicio y fin deben ser el mismo ", 'b');}
		;
	}
	*/
	;

method_call:
	ID method_arguments
	;

method_arguments:
	PARI concat_IDs PARD 
	| PARI concat_operations PARD 
	| PARI PARD
	;
	
concat_IDs:
	tipos COM concat_IDs 
	| tipos
	;
	
concat_operations:
	operaciones COM concat_operations
	| operaciones
	;
	
rule_for:
	FOR DOSP assign COM comparaciones COM operaciones PARI instrucciones PARD
	;
	
comparaciones: 
	comparacionesbooleanas |
	comparaciones AND_OR comparacionesbooleanas
	;
	 
comparacionesbooleanas: 
	tipos COMPARACION tipos
	| tipos COMPARACION operaciones
	| operaciones COMPARACION tipos
	| operaciones COMPARACION operaciones
	;
	
ret:
	RET tipos | RET operaciones | RET comparaciones
	;
	
rule_IF:
	IF DOSP comparaciones PARI instrucciones PARD
	;
	
rule_While:
	WHILE DOSP comparaciones PARI instrucciones PARD
	;
	
rule_ELSE:
	rule_IF ELSE PARI instrucciones PARD
	;
	
arreglo:
	ID CORI tamano_arreglo CORD 
	;

tamano_arreglo:
	operando
	| operaciones
	;

//This is where we end our suffering.
%%

int main(int argc, char** argv) {
	yyin = fopen(argv[1],"r");
	
		yyparse();
	
}

void printError(char* errormsg, char type){
	extern int yylineno;
	printf("%s en la linea: %d\n", errormsg, yylineno);
	if(type == 'a'){
		printf("El error es: %s\n",yytext);
		exit(-1);
	}
}

void yyerror(const char *s){
	extern int yylineno;
	//printf("%s",s);
	printf("\n%s   , en la linea %d\n",s,yylineno);
	exit(-1);
}