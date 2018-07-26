default:
	bison -d Gramaticas.bison -ogramaticas.tab.c
	flex Expresiones.flex
	gcc -oparser.exe lex.yy.c gramaticas.tab.c