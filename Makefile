# macro
TARGET = semantic_analyzer
TARGET_SYNTAX = syntax_analyzer
L = lex.l
Y = parse.y
T = type.h
YYLEX = lex.yy.c 
YYPARSE = parse.tab.c
PRINT = print.c
PRINT_SEM = print_sem.c
SEMANTIC = semantic.c
MAIN = main.c
LEX = flex 
YACC = bison 

# dependencies
all : $(TARGET)

$(TARGET): $(SEMANTIC) $(PRINT_SEM) $(PRINT) $(MAIN) $(YYLEX) $(YYPARSE)
	$(CC) $^ -o $@ -g

$(TARGET_SYNTAX): $(PRINT) $(MAIN) $(YYLEX) $(YYPARSE)
	$(CC) $^ -o $@ -g

$(YYLEX): $(L) $(T)
	$(LEX) $(L)

$(YYPARSE): $(Y) $(T)
	$(YACC) $(Y)

clean:
	$(RM) $(YYLEX) $(YYPARSE) $(TARGET) $(TARGET_SYNTAX)

# test macro
echo:
	echo $(COMP)
