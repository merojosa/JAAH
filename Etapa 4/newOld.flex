%{
#include <stdio.h>
#include <stdlib.h>
#include "gramaticas.tab.h"
char* yyvalue;
%}

%option noyywrap
%option yylineno 

%%
(\s)+|(\t)+|(\n)+|(\R)+ {
		/*Whitespaces ignored*/;
	}
(\/\*)((.|\s|\n)*)?(\*\/) {
 		/*Block comments ignored*/;
 	}
(\/\/)((.|\n\r)*)? {
 		/*In-line comments ignored*/;
 	}
\"((.|\n\r)*)?\" {
		yylval.string = yytext;
 		return HILERA;
 	}
principal	{
		return EJECUCION;
	}
para	{
 		return FOR;
 	}
mientras	{
 		return WHILE;
 	}
\+incluir	{
		return LINK;
	}
si	{
 		return IF;
 	}
sino	{
 		return ELSE;
 	}
==|<=|>=|>|<	{
		yylval.string = yytext;
 		return COMPARACION;
 	}
(\|\|)|&&	{
 		return AND_OR;
 	}
verdadero|falso	{
 		return BOOL;
 	}
ini {
 		return INI;
 	}
fin {
 		return FIN;
 	}
ret {
 		return RET;
 	}
entrada {
 		return ENTRADA;
 	}
salida {
 		return SALIDA;
 	}
\( {
 		return PARI;
 	}
\) {
 		return PARD;
 	}
\[ {
 		return CORI;
 	}
\] {
 		return CORD;
 	}
\= {
 		return IGUAL;
 	}
\;	{
		return PYC;
	}
\: {
		return DOSP;
 	}
,	{
		return COM;
	}
\.	{
		return PUNTO;
	}
\-\>	{
		return POINTER;	
	}
\<[a-zA-Z0-9_ñ]+\.jaah\>\n	{
		return JAAH;
	}
[a-zñ][a-zA-Z0-9_ñ]* {
		yylval.string = yytext;
 		return ID;
 	}
-?[0-9]+ {
		yylval.intVal = atoi(yytext);
 		return NUM;
 	}
[\+\-\*\/\%] {
		yylval.string = yytext;
 		return OPERACIONES;
 	}
[A-Z_Ñ0-9][a-zA-Z0-9_ñ]*|[0-9]+(\.|\,)[0-9]+	{
		printError("Error lexico", 'a');
 		return ERROR;
 	}
%%