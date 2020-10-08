# macro
TARGET = syntax_analyzer
L = lex.l
Y = parse.y
YYLEX = lex.yy.c 
YYPARSE = y.tab.c
LEX = flex
YACC = bison -d

# dependencies
all : $(TARGET)

$(TARGET): $(YYLEX) $(YYPARSE)
	$(CC) $^ -o $@

$(YYLEX): $(L)
	$(LEX) $(L)

$(YYPARSE): $(Y)
	$(YACC) $(Y)

clean:
	$(RM) $(SOURCES) $(TARGET)

# test macro
echo:
	echo $(COMP)
