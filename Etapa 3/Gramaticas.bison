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
%}

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token  HILERA//ya
%token  FOR//ya
%token  WHILE
%token  IF
%token  ELSE
%token  COMPARACION //ya
%token  AND_OR //ya
%token  BOOL //ya
%token  NUM //ya
%token  INI //ya 
%token  FIN //ya
%token  RET
%token  OPERACIONES //ya
%token  ENTRADA 
%token  SALIDA //ya
%token  ID //ya
%token  ERROR //ya
%token  EJECUCION
%token  PARD //ya
%token  CORD 
%token  PARI //ya
%token  CORI //ya 
%token  IGUAL //ya
%token  DOSP //ya
%token  COM //ya
%token LF //ya

%%
principal:
	instrucciones
	;
method:
	cambios_linea ini def_method cambios_linea
	instrucciones cambios_linea fin id  cambios_linea{printf("<%s> - <INI> \n %s \n %s\n %s\n %s \n <%s> - <FIN> \n <%s> - <ID> \n", $2, $3, $4, $5, $6, $7, $8);}
	;

def_method:
	id dosp concat_ids {printf("<%s> - <ID> \n <%s> - <:> \n", $1, $2);}
	| id dosp {printf("<%s> - <ID> \n  <%s> - <:> \n", $1, $2);}
	;
	
concat_ids:
	id com concat_ids {printf("<%s> - <ID> \n <%s> - <,> \n", $1, $2);}
	| id {printf("<%s> - <ID> \n", $1);}
	;
	
rule_for:
	for dosp assign com comparaciones com operaciones pari cambios_linea instrucciones pard 
	{printf("<%s> - <PARA> \n <%s> - <:> \n <%s> - <,> \n <%s> - <,> \n <%s> - <\(> \n <%s> - <\)>\n", $1, $2, $4, $6, $8, $11);}
	;
	
assign:
	id igual id {printf("<%s> - <ID> \n <%s> - <=> \n <%s> - <ID> \n", $1, $2 ,$3);}
	| id igual num {printf("<%s> - <ID> \n <%s> - <=> \n <%s> - <NUM> \n", $1, $2 ,$3);}
	| id igual bool {printf("<%s> - <ID> \n <%s> - <=> \n <%s> - <BOOL> \n", $1, $2 ,$3);}
	| id igual operaciones {printf("<%s> - <ID> \n <%s> - <=> \n %s \n", $1, $2 ,$3);}
	; 
	
comparaciones: 
	comparacionesbooleanas |
	comparaciones and_or comparacionesbooleanas {printf("<%s> - <AND_OR> \n",$2);}
	;
	 
comparacionesbooleanas: 
	tipos comparacion tipos {printf("<%s> - <COMPARACION> \n",$2);}
	| tipos comparacion operaciones {printf("<%s> - <ID o NUM> \n <%s> - <Comparacion> \n <%s> - <ID o NUM> \n",$2);}
	;
	
tipos:
	id {printf("<%s> - <ID>\n",$1);}| num {printf("<%i> - <NUM> \n",$1);}| bool {printf("<%s> - <BOOL> \n",$1);}
	;
	
operaciones:
	operando operar operando {printf("<%s> - <OPERADOR> \n", $2);}
	| operaciones operar operando {printf("<%s> - <OPERADOR>\n", $2);}
	;
	
operando:
	id {printf("<%s> - <ID> \n",$1);}
	| num {printf("<%s> - <NUM> \n",$1);}
	
	;
	
instrucciones:
	assign cambios_linea instrucciones
	| salida dosp id cambios_linea instrucciones {printf("<%s> - <SALIDA> \n <%s> - <:> \n <%s> - <ID> \n", $1, $2 ,$3);}
	| salida dosp num cambios_linea instrucciones {printf("<%s> - <SALIDA> \n <%s> - <:> \n <%i> - <NUM> \n", $1, $2 ,$3);}
	| salida dosp hilera cambios_linea instrucciones {printf("<%s> - <SALIDA> \n <%s> - <:> \n <%s> - <HILERA> \n", $1, $2 ,$3);}
	| cambios_linea 
	|
	;
	
cambios_linea:
	LF cambios_linea {printf("<%s> - <ID> \n",$1);}
	| LF {printf("<%s> - <ID> \n",$1);}
	;
	
id:
	ID {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
ini:
	INI {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
num:
	NUM {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
com:
	COM {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
dosp:
	DOSP {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
igual:
	IGUAL {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
comparacion:
	COMPARACION {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
for:
	FOR {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
operar:
	OPERACIONES {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
fin:
	FIN INI {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
bool:
	BOOL {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
hilera:	
	HILERA {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
and_or:
	AND_OR {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
pari:
	PARI {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
pard:
	PARD {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
salida:
	SALIDA {$$ = strdup(yytext);} | ERROR {$$ = "Error Lexico";}
	;
%%

int main(int argc, char** argv) {
	yyin = fopen(argv[1],"r");
	
		yyparse();
	
}

void yyerror(const char *s){
	printf("Error de Sintaxis!  Message: %s\n",s);
	// might as well halt now:
	exit(-1);
}