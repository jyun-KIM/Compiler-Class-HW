all: compiler

compiler: parser.tab.c lex.yy.c ast.c
	gcc -o my_interpreter parser.tab.c lex.yy.c ast.c -I.

parser.tab.c: parser.y
	bison -d parser.y

lex.yy.c: scanner.l
	flex scanner.l

clean:
	rm -f my_interpreter parser.tab.c parser.tab.h lex.yy.c