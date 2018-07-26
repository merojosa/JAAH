%{
#include <stdio.h>
#include <stdlib.h>
#include "gramaticas.tab.h"
char* yyvalue;
extern void printError(char*,char);
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
		yylval.string = yytext;
		return EJECUCION;
	}
para	{
		yylval.string = yytext;
 		return FOR;
 	}
mientras	{
		yylval.string = yytext;
 		return WHILE;
 	}
\+incluir	{
		return LINK;
	}
si	{
		yylval.string = yytext;
 		return IF;
 	}
sino	{
		yylval.string = yytext;
 		return ELSE;
 	}
==|<=|>=|>|<	{
		yylval.string = yytext;
 		return COMPARACION;
 	}
(\|\|)|&&	{
		yylval.string = yytext;
 		return AND_OR;
 	}
verdadero|falso	{
		yylval.string = yytext;
 		return BOOL;
 	}
ini {
		yylval.string = yytext;
 		return INI;
 	}
fin {
		yylval.string = yytext;
 		return FIN;
 	}
ret {
		yylval.string = yytext;
 		return RET;
 	}
entrada {
		yylval.string = yytext;
 		return ENTRADA;
 	}
salida {
		yylval.string = yytext;
 		return SALIDA;
 	}
\( {
		yylval.string = yytext;
 		return PARI;
 	}
\) {
		yylval.string = yytext;
 		return PARD;
 	}
\[ {
		yylval.string = yytext;
 		return CORI;
 	}
\] {
		yylval.string = yytext;
 		return CORD;
 	}
\= {
		yylval.string = yytext;
 		return IGUAL;
 	}
\;	{
		yylval.string = yytext;
		return PYC;
	}
\: {
		yylval.string = yytext;
		return DOSP;
 	}
,	{
		yylval.string = yytext;
		return COM;
	}
\.	{
		yylval.string = yytext;
		return PUNTO;
	}
\-\>	{
		yylval.string = yytext;
		return POINTER;
	}
\<[a-zA-Z0-9_ñ]+\.jaah\>\n	{
		return JAAH;
	}
[a-zñ][a-zA-Z0-9_ñ]* {
		yylval.string = yytext;
 		return ID;
 	}
[0-9]+ {
		yylval.intVal = atoi(yytext);
 		return NUM;
 	}
[\+] {
		yylval.string = yytext;
 		return MAS;
 	}
[\-] {
		yylval.string = yytext;
 		return MENOS;
 	}
[\*] {
		yylval.string = yytext;
 		return POR;
 	}
[\/] {
		yylval.string = yytext;
 		return ENTRE;
 	}
[\%] {
		yylval.string = yytext;
 		return MOD;
 	}
[A-Z_Ñ0-9][a-zA-Z0-9_ñ]*|[0-9]+(\.|\,)[0-9]+	{
		printError("Error lexico", 'a');
 		return ERROR;
 	}
%%
