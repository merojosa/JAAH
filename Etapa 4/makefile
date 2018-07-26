default:
	bison -d newWhole.bison -ogramaticas.tab.c
	flex newOld.flex
	gcc -oparser.exe lex.yy.c gramaticas.tab.c