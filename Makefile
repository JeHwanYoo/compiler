# macro
TARGET = code_generation
TARGET_SEMANTIC = semantic_analyzer
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
YACC = bison -d
CODE = code.c

# dependencies
all : $(TARGET)

$(TARGET): $(CODE) $(SEMANTIC) $(PRINT_SEM) $(PRINT) $(MAIN) $(YYLEX) $(YYPARSE)
	$(CC) $^ -o $@ -g

$(TARGET_SEMANTIC): $(SEMANTIC) $(PRINT_SEM) $(PRINT) $(MAIN) $(YYLEX) $(YYPARSE)
	$(CC) $^ -o $@ -g

$(TARGET_SYNTAX): $(PRINT) $(MAIN) $(YYLEX) $(YYPARSE)
	$(CC) $^ -o $@ -g

$(YYLEX): $(L) $(T)
	$(LEX) $(L)

$(YYPARSE): $(Y) $(T)
	$(YACC) $(Y)

clean:
	$(RM) $(YYLEX) $(YYPARSE) $(TARGET) $(TARGET_SEMANTIC) $(TARGET_SYNTAX) parse.tab.h 

# test macro
echo:
	echo $(COMP)
