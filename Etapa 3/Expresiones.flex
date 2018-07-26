%{
#include <stdio.h>
#include <stdlib.h>
#include "gramaticas.tab.h"
	/*
		Para ejecutar el programa:
		-Requiere flex y gcc instalados.
		-Abrir desde consola el directorio donde esté ubicado éste archivo.
		-Escribir en consola flex Expresiones.flex
		-Compilar el archivo generado(lex.yy.c) con el siguiente comando:
			gcc lex.yy.c -o["Nombre que quiera darle al ejecutable"].exe
			Sin los corchetes y comillas.
		-Escribir el consola ["Nombre del ejecutable"] < ["Nombre del archivo a analizar"]
			Sin corchetes y comillas.
		-Si se abre el ejecutable sin archivo a anilzar, espera datos en la entrada estándar para analizarlos.
		-Para parar el programa: ctrl + c
	*/
%}
%option noyywrap
%%
(\n)+	{
	return LF;
}
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
si	{
 		return IF;
 	}
sino	{
 		return ELSE;
 	}
==|<=|>=|>|<	{
 		return COMPARACION;
 	}
(\|\|)|&&	{
 		return AND_OR;
 	}
verdadero|falso	{
 		return BOOL;
 	}
[0-9]+ {
 		return NUM;
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
\: {
 		return DOSP;
 	}
,	{
		return COM;
	}
[a-zñ][a-zA-Z0-9_ñ]* {
 		return ID;
 	}
[\+\-\*\/\%] {
 		return OPERACIONES;
 	}
[A-Z_Ñ0-9][a-zA-Z0-9_ñ]*|[0-9]+(\.|\,)[0-9]+	{
 		return ERROR;
 	}
%%