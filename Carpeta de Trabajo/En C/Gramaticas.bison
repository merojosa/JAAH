 %{
	#include <stdio.h>
	#include <stdlib.h>
	#include <ctype.h>
	#include "tabla_simbolos.h"
	#include "ArbolC.h"
	#include "lista_parametros.h"
	// stuff from flex that bison needs to know about:
	#define YYDEBUG 1
	extern int yylex(void);
	extern int yyparse();
	//extern char* strdup(char*);
	extern char* yytext;
	extern FILE *yyin;
	extern Nodo_Arbol* constructor_nodo(char*,Nodo_Arbol* HD, Nodo_Arbol* padre, Nodo_Arbol* HI);
	extern Nodo_Arbol* raiz(Arbol* arbol);
	extern char* nodeToken(Nodo_Arbol* nodo);
	extern Nodo_Arbol* padre(Nodo_Arbol* padre);
	extern Nodo_Arbol* hijoMasIzq(Nodo_Arbol* nodo);
	extern Nodo_Arbol* hermanoDer(Nodo_Arbol* nodo);
	extern void agregarHijo(Arbol* arbol, Nodo_Arbol* nodo, Nodo_Arbol*,Etiqueta etiq);
	extern Arbol* constructor_arbol();

	extern bool vacio(Arbol* arbol);
	extern bool esHoja(Nodo_Arbol* nodo);

	extern void borrarHoja(Arbol* arbol, Nodo_Arbol* nodo);
	extern void modificarEtiqueta(Nodo_Arbol* nodo, Etiqueta etiq);
	extern void destructor_arbol(Arbol* arbol);
	extern void vaciar_arbol(Arbol* arbol, Nodo_Arbol* nodo);
	extern void ponerRaiz(Arbol* arbol, Etiqueta etiq);

	extern Etiqueta etiqueta(Nodo_Arbol* nodo);

int numNodos(Arbol* arbol);
int numHijos(Nodo_Arbol* nodo);
	Arbol* arbol;
	Lista* lista; //Tabla de símbolos.
	void yyerror(const char *s);
	void printError(char* errormsg, char type);
%}
%define parse.lac full
%error-verbose
%union
{
	int intVal;
	char* string;
	void* valorTabla;
	void* node;
	void* params;
}
// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <string> HILERA "Hilera"
%token <string> FOR "para"
%token <string> WHILE "mientras"
%token <string> IF "si"
%token <string> ELSE "sino"
%token <string> COMPARACION "comparador"
%token <string> AND_OR "&& o ||"
%token <string> BOOL "verdadero o falso"
%token <intVal> NUM "numero"
%token <string> INI "ini"
%token <string> FIN "fin"
%token <string> RET "ret"
%token <string> MAS "+"
%token <string> MENOS "-"
%token <string> POR "*"
%token <string> ENTRE "/"
%token <string> MOD "%"
%token <string> ENTRADA "entrada"
%token <string> SALIDA "salida"
%token <string> ID "identificador"
%token <string> ERROR
%token <string> EJECUCION "principal"
%token <string> PARD ")"
%token <string> CORD "]"
%token <string> PARI "("
%token <string> CORI "["
%token <string> IGUAL "="
%token <string> DOSP ":"
%token <string> COM ","
%token <string> PUNTO "."
%token <string> POINTER "->"
%token <string> PYC ";"
%token <string> LINK "enlazador de archivos"
%token <string> JAAH "ruta del archivo"

%type <string> OPERACIONES
%type <node> assign arreglo
%%
//This is where the fun begins.

super:
	inicio 
	{
	arbol = constructor_arbol(); 
	lista = constructor_lista();
	ponerRaiz(arbol,-1);
	}
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
	assign PYC declarations {Nodo_Arbol* nodo;}
	| ID PYC declarations
	|
	;

assign:
	ID IGUAL tipos 
	{
	Nodo_Arbol* node = constructor_nodo($1,NULL,NULL,NULL);
	Nodo_Lista* parametros = constructor_nodo_lista($2,NULL);
	}
	| ID IGUAL operaciones
	| arreglo IGUAL tipos
	{
	Nodo_Arbol* node = constructor_nodo($1,NULL,NULL,NULL);
	Nodo_Lista* parametros = constructor_nodo_lista($2,NULL);
	}
	| arreglo IGUAL operaciones
	| ID IGUAL method_call
	{
	Nodo_Arbol* node = constructor_nodo($1,NULL,NULL,NULL);
	Nodo_Lista* parametros = constructor_nodo_lista($2,NULL);
	}
	;

concat_links:
	LINK JAAH concat_links
	| LINK JAAH
	;

operaciones:
	operando OPERACIONES operando
	| operaciones OPERACIONES operando
	;

tipos:
	operando
	| BOOL
	| HILERA
	;

operando:
	ID
	| NUM
	| MENOS NUM
	| arreglo
	| ID PUNTO ID
	| POINTER method_call
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

OPERACIONES:
	MAS
	| MENOS
	| POR
	| ENTRE
	| MOD
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
