default:
	bison -d gramaticas.cpp -ogramaticas.tab.c
	flex expresiones.l
	g++ -g -oparser.exe ArbolC++.h lex.yy.c gramaticas.tab.c -std=c++11
