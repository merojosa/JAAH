default:
	bison -d Gramaticas.bison -ogramaticas.tab.c
	flex Regex.flex
	gcc -oparser.exe lista_parametros.h ArbolC.h tabla_simbolos.h lex.yy.c gramaticas.tab.c

