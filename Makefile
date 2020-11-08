# macro
TARGET = syntax_analyzer
L = lex.l
Y = parse.y
T = type.h
YYLEX = lex.yy.c 
YYPARSE = parse.tab.c
PRINT = print.c
MAIN = main.c
LEX = flex -d
YACC = bison -d

# dependencies
all : $(TARGET)

$(TARGET): $(PRINT) $(MAIN) $(YYLEX) $(YYPARSE)
	$(CC) $^ -o $@ -g

$(YYLEX): $(L) $(T)
	$(LEX) $(L)

$(YYPARSE): $(Y) $(T)
	$(YACC) $(Y)

clean:
	$(RM) $(YYLEX) $(YYPARSE) $(TARGET)

# test macro
echo:
	echo $(COMP)
