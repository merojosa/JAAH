%{
#include <stdio.h>
#include <string>
#include "ArbolC++.h"
using namespace std;
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" char* yytext;
void yyerror(const char *s);
Arbol arbol;
int cuenta = 0;
void imprimir_tabs(int cantidad_tabs)
{
	for(int contador = 0; contador < cantidad_tabs; ++contador)
	{
		cout << '\t';
	}
}


/**
* EFE: Lista las etiquetas del arbol haciendo un recorrido en pre-orden.
* REQ: Arbol inicializado.
* MOD: N/A.
*/
void listPreORecur(Arbol& arbol, Nodo nodo, int cantidad_tabs)
{
	imprimir_tabs(cantidad_tabs);\
	if(cantidad_tabs == 0){
		cout << *nodo->tokenName << endl;
	} else {
		if(arbol.esHoja(nodo)){
			cout<<"Instruccion ";
		} else {
			if(cantidad_tabs == 1){
				cout << "Metodo ";
			} else {
				cout<<"Bloque ";
			}
		}
		cout << *nodo->tokenName << " ";
	}				// Imprimo instrucción.
	if(nodo->params != 0x0){
		if(nodo->params->size() != 0){
			cout<<"Parametros: ";
			for(list<string*>::iterator it = nodo->params->begin(); it != nodo->params->end(); it++){
				cout<<**it;
			}
		}
	}
	cout<<"\n";
	Nodo hijo = arbol.hijoMasIzq(nodo);

	if( hijo != nodoNulo )							// Si tiene hijo.
	{
		++cantidad_tabs;							// Bajo de nivel, aumento tab.
		listPreORecur(arbol, hijo, cantidad_tabs);	// Repito con el hijo.
		--cantidad_tabs;
		// Al salir del if, terminó de recorrer el nivel.  Por lo que resto un tab.
	}

	Nodo hermano_derecho = arbol.hermanoDer(nodo);
	if( hermano_derecho != nodoNulo )				// Verifico si tiene hermano.
	{
		listPreORecur(arbol, hermano_derecho, cantidad_tabs);
	}
	// Si no tiene hermano, termino (sea nivel o método).
}

// El llamado que se hace en el main.
void listPreO(Arbol& arbol)
{
	cout<<"\n";
	if (!arbol.vacio())
	{
		listPreORecur(arbol, arbol.raiz(), 0);
	}
}

%}
%error-verbose

%code requires{
	#include <list>
	#include <string>
	//#include "ArbolC++.h"
	using namespace std;
}
%union {
	string* hilera;
	int intVal;
	struct Caja* nodo;
	struct Simbolos* simbolo;
	std::list<string*> *params;
}

%token <hilera> HILERA "Hilera"
%token <hilera> FOR "para"
%token <hilera> WHILE "mientras"
%token <hilera> IF "si"
%token <hilera> ELSE "sino"
%token <hilera> COMPARACION "comparador"
%token <hilera> AND_OR "&& o ||"
%token <hilera> BOOL "verdadero o falso"
%token <intVal> NUM "numero"
%token <hilera> INI "ini"
%token <hilera> FIN "fin"
%token <hilera> RET "ret"
%token <hilera> MAS "+"
%token <hilera> MENOS "-"
%token <hilera> POR "*"
%token <hilera> ENTRE "/"
%token <hilera> MOD "%"
%token <hilera> ENTRADA "entrada"
%token <hilera> SALIDA "salida"
%token <hilera> ID "identificador"
%token <hilera> ERROR
%token <hilera> EJECUCION "principal"
%token <hilera> PARD ")"
%token <hilera> CORD "]"
%token <hilera> PARI "("
%token <hilera> CORI "["
%token <hilera> IGUAL "="
%token <hilera> DOSP ":"
%token <hilera> COM ","
%token <hilera> PUNTO "."
%token <hilera> POINTER "->"
%token <hilera> PYC ";"
%token <hilera> LINK "enlazador de archivos"
%token <hilera> JAAH "ruta del archivo"


%type <nodo> declarations
%type <nodo> assign
%type <nodo> rule_IF
%type <params> comparaciones;
%type <nodo> instrucciones;
%type <nodo> tipos
%type <params> operaciones
%type <nodo> rule_for
%type <nodo> rule_While
%type <nodo> rule_ELSE
%type <nodo> ret
%type <nodo> method_call
%type <nodo> method
%type <params> method_arguments
%type <params> concat_IDs_operations
%type <nodo> operando
%type <nodo> arreglo
%type <hilera> OPERACIONES
%type <params> comparacionesbooleanas
%type <params> tamano_arreglo
%type <nodo> inicio
%type <nodo> principal

%%
//This is where the fun begins.

super:
	inicio
	{
		string* root = new std::string("Programa:"); arbol.ponerRaiz(cuenta,root); arbol.agregarHijo(arbol.raiz(),$1); listPreO(arbol);cuenta++;}
	;

inicio:
	concat_links principal {$$ = $2;}
	| principal {$$ = $1;}
	;

principal:
	declarations method principal
	{
		$2->HD = $3;
		if($1 != nodoNulo){$1->HD = $2;}
		if($1 != nodoNulo){$$ = $1;} else { $$ = $2;}
		}
	| declarations method
	{
		if($1 != nodoNulo){$1->HD = $2; $$ = $1;}
		else { $$ = $2;}
	}
	;

declarations:
	assign PYC declarations
	{
		$$ = $1;
		if($3 != nodoNulo){
			$1->HD = $3;
		}
	}
	| ID PYC declarations
	{
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		if($3 != nodoNulo){
			$$->HD = $3;
		}
	}
	| {$$ = nodoNulo;}
	;

assign:
	ID IGUAL tipos
	{
		$$ = new Caja(cuenta++, $1,NULL,NULL);
		$$->params = new list<string*>();
		$$->params->push_back($2);
		$$->params->push_back($3->tokenName);
		}
	| ID IGUAL operaciones
	{
		$$ = new Caja(cuenta++, $1,NULL,NULL);
		$3->push_front($2);
		$$->params = $3;
	}
	| arreglo IGUAL tipos
	{
		$$ = $1;
		$$->params->push_back($2);
		$$->params->push_back($3->tokenName);
	}
	| arreglo IGUAL operaciones
	{
		$$ = $1;
		$$->params->push_back($2);
		arbol.mergeLists($$->params,$3);
	}
	| ID IGUAL method_call
	{
		$$ = new Caja(cuenta++, $1,NULL,NULL);
		$$->params = new list<string*>();
		$$->params->push_back($2);
		$$->params->push_back($3->tokenName);
	}
	;

concat_links:
	LINK JAAH concat_links
	| LINK JAAH
	;

operaciones:
	operaciones OPERACIONES operando
	{
		$$->push_back($2);
		$$->push_back($3->tokenName);
	}
	| operando OPERACIONES operando {
		$$ = new list<string*>();
		$$->push_back($1->tokenName);
		$$->push_back($2);
		$$->push_back($3->tokenName);
	}
	;

tipos:
	operando {$$ = $1;}
	| BOOL {$$ = new Caja(cuenta++, $1,NULL,NULL);}
	| HILERA {$$ = new Caja(cuenta++, $1,NULL,NULL);}
	;

operando:
	ID {$$ = new Caja(cuenta++, $1,NULL,NULL);}
	| NUM {cuenta++;string* num = new string(to_string($1));$$ = new Caja(cuenta,num,NULL,NULL);}
	| MENOS NUM {cuenta++;int value = 0-$2; string* sValue = new string(to_string(value)); $$ = new Caja(cuenta, sValue,NULL,NULL);}
	| arreglo {$$ = $1;}
	| POINTER method_call {$$ = $2;}
	;


instrucciones:
	assign PYC instrucciones {
		$$ = $1;
		$$->HD = $3;
	}
	| SALIDA DOSP tipos PYC instrucciones {
		$$ = new Caja(cuenta++, $1,$5,NULL);
		$$->params = new list<string*>();
		$$->params->push_back($3->tokenName);
	}
	| rule_for instrucciones {
		$$ = $1;
		$$->HD = $2;
	}
	| rule_ELSE instrucciones {
		$$ = $1;
		$$->HD->HD = $2;
	}
	| rule_IF instrucciones {$$ = $1;
		$$->HD = $2;
	}
	| rule_While instrucciones {
		$$ = $1;
		$$->HD = $2;
	}
	| ENTRADA DOSP ID PYC instrucciones {
		$$ = new Caja(cuenta++, $1,$5,NULL);
		$$->params = new list<string*>();
		$$->params->push_back($3);
	}
	| ret PYC instrucciones {
		$$ = $1;
		$$->HD = $3;
	}
	| PYC instrucciones {
		$$ = $2;
	}
	| method_call PYC instrucciones {
		$$ = $1;
		$$->HD = $3;
	}
	| {$$ = nodoNulo;}
	;

method:
	INI method_call DOSP
	instrucciones FIN
	{
		$$ = $2;
		if($4 != nodoNulo)
		{$$->HMI = $4;}

	}
	;

method_call:
	ID method_arguments
	{
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->params = $2;
	}
	;

	method_arguments:
		PARI concat_IDs_operations PARD {$$ = $2;}
		| PARI PARD {$$ = NULL;}
		;

	concat_IDs_operations:
		tipos COM concat_IDs_operations
		{
			$$ = new list<string*>();
			$$->push_front($1->tokenName);
			arbol.mergeLists($$,$3);
			delete $3;
		}
		| operaciones COM concat_IDs_operations
		{
			$$ = new list<string*>();
			$3->push_front($2);
			arbol.mergeLists($3,$1);
			arbol.mergeLists($$,$3);
		}
		| tipos
		{
			$$ = new list<string*>();
			$$->push_back($1->tokenName);
		}
		| operaciones
		{
			$$ = new list<string*>();
			arbol.mergeLists($$,$1);
		}
		;

comparaciones://falta esta cosita :D
	comparacionesbooleanas AND_OR comparaciones {
		$1->push_back($2);
		arbol.mergeLists($1,$3);
		$$ = $1;
	}
	| comparacionesbooleanas {$$ = $1;}
	;

comparacionesbooleanas:
	tipos COMPARACION tipos
	{
		$$ = new list<string*>();
		$$->push_back($1->tokenName);
		$$->push_back($2);
		$$->push_back($3->tokenName);
	}
	| tipos COMPARACION operaciones
	{
		$3->push_front($2);
		$3->push_front($1->tokenName);
		$$ = $3;
	}
	| operaciones COMPARACION tipos
	{
		$1->push_back($2);
		$1->push_back($3->tokenName);
		$$ = $1;
	}
	| operaciones COMPARACION operaciones
	{
		$1->push_back($2);
		arbol.mergeLists($1, $3);
		$$ = $1;
	}
	;

ret:
	RET tipos {
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->params = new list<string*>();
		$$->params->push_back($2->tokenName);
	}
	| RET operaciones {
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->params = $2;
	}
	| RET comparaciones {
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->params = $2;
	}
	;

rule_IF:
	IF DOSP comparaciones PARI instrucciones PARD
	{
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->params = $3; $$->HMI = $5;
	}
	;

	rule_for:
		FOR DOSP assign COM comparaciones COM operaciones PARI instrucciones PARD
		{
			$$ = new Caja(cuenta++,$1,NULL,NULL);
			/*for(Nodo tmp = $9; tmp != nodoNulo; tmp = tmp->HD){
				tmp->padre = $$;
			}*/
			$$->params = new list<string*>();
		/*	arbol.mergeLists($$->params,$3->params);
			arbol.mergeLists($$->params, $5);
			arbol.mergeLists($$->params,$7);*/
			$$->HD = $9;
		}
		;

rule_While:
	WHILE DOSP comparaciones PARI instrucciones PARD {
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->params = $3;
		arbol.agregarHijo($$,$5);
	}
	;

rule_ELSE:
	rule_IF ELSE PARI instrucciones PARD
	{
		$$ = new Caja(cuenta++,$2,NULL,NULL);
		$$->HMI = $4;
		$1->HD = $$;
		$$ = $1;
	}
	;

arreglo:
	ID CORI tamano_arreglo CORD
	{
		$$ = new Caja(cuenta++, $1,NULL,NULL);
		$$->params = $3;
		$$->params->push_front($2);
		$$->params->push_back($4);
	}
	;

tamano_arreglo:
	operando {
		$$ = new list<string*>();
		$$->push_back($1->tokenName);
	}
	| operaciones {$$ = $1;}
	;

OPERACIONES:
	MAS {$$ = $1;}
	| MENOS {$$ = $1;}
	| POR {$$ = $1;}
	| ENTRE {$$ = $1;}
	| MOD {$$ = $1;}
	;

//This is where we end our suffering.
%%
int main(int argc, char** argv) {
	if(argc > 1){
		yyin = fopen(argv[0],"r");
	} else {
		yyin = stdin;
	}
	yyparse();
	return 0;
}

void printError(string errormsg, char type){
	extern int yylineno;
	//printf("%s en la linea: %d\n", errormsg.c_str(), yylineno);
	cout<< errormsg<<" en la linea: "<<yylineno<<"\n";
	if(type == 'a'){
		printf("El error es: %s\n",yytext);
		exit(-1);
	}
}

void yyerror(const char *s) {
	extern int yylineno;
	//printf("%s",s);
	printf("\n%s   , en la linea %d\n",s,yylineno);
	exit(-1);
}
