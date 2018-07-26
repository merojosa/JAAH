%{
#include <iostream>
#include "gramaticas.tab.h"
using namespace std;
#define YY_DECL extern "C" int yylex()
#define guardar std::string(yytext, yylen)
extern void printError(std::string, char);

%}
%option yylineno
%option noyywrap

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
		yylval.hilera = new std::string(yytext,yyleng);
 		return HILERA;
 	}
principal	{
		yylval.hilera = new std::string(yytext,yyleng);
		return EJECUCION;
	}
para	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return FOR;
 	}
mientras	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return WHILE;
 	}
\+incluir	{
		return LINK;
	}
si	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return IF;
 	}
sino	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return ELSE;
 	}
==|<=|>=|>|<	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return COMPARACION;
 	}
(\|\|)|&&	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return AND_OR;
 	}
verdadero|falso	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return BOOL;
 	}
ini {
		yylval.hilera = new std::string(yytext,yyleng);
 		return INI;
 	}
fin {
		yylval.hilera = new std::string(yytext,yyleng);
 		return FIN;
 	}
ret {
		yylval.hilera = new std::string(yytext,yyleng);
 		return RET;
 	}
entrada {
		yylval.hilera = new std::string(yytext,yyleng);
 		return ENTRADA;
 	}
salida {
		yylval.hilera = new std::string(yytext,yyleng);
 		return SALIDA;
 	}
\( {
		yylval.hilera = new std::string(yytext,yyleng);
 		return PARI;
 	}
\) {
		yylval.hilera = new std::string(yytext,yyleng);
 		return PARD;
 	}
\[ {
		yylval.hilera = new std::string(yytext,yyleng);
 		return CORI;
 	}
\] {
		yylval.hilera = new std::string(yytext,yyleng);
 		return CORD;
 	}
\= {
		yylval.hilera = new std::string(yytext,yyleng);
 		return IGUAL;
 	}
\;	{
		yylval.hilera = new std::string(yytext,yyleng);
		return PYC;
	}
\: {
		yylval.hilera = new std::string(yytext,yyleng);
		return DOSP;
 	}
,	{
		yylval.hilera = new std::string(yytext,yyleng);
		return COM;
	}
\.	{
		yylval.hilera = new std::string(yytext,yyleng);
		return PUNTO;
	}
\-\>	{
		yylval.hilera = new std::string(yytext,yyleng);
		return POINTER;
	}
\<[a-zA-Z0-9_ñ]+\.jaah\>\n	{
		return JAAH;
	}
[a-zñ][a-zA-Z0-9_ñ]*	{
		yylval.hilera = new std::string(yytext,yyleng);
 		return ID;
 	}
[0-9]+ {
		yylval.intVal = atoi(yytext);
 		return NUM;
 	}
[\+] {
		yylval.hilera = new std::string(yytext,yyleng);
 		return MAS;
 	}
[\-] {
		yylval.hilera = new std::string(yytext,yyleng);
 		return MENOS;
 	}
[\*] {
		yylval.hilera = new std::string(yytext,yyleng);
 		return POR;
 	}
[\/] {
		yylval.hilera = new std::string(yytext,yyleng);
 		return ENTRE;
 	}
[\%] {
		yylval.hilera = new std::string(yytext,yyleng);
 		return MOD;
 	}
[A-Z_Ñ0-9][a-zA-Z0-9_ñ]*|[0-9]+(\.|\,)[0-9]+	{
		printError("Error lexico", 'a');
 		return ERROR;
 	}
%%
