digit										[0-9]
letter									[a-zA-z_]
delim										[\t]
line										[\n]
ws											{delim}+
%{
#include "y.tab.h"
union YYTYPE; // { int i; char c; char *s; }
extern union YYTYPE yylval;
extern int line_no;
char *makeString(char *s);
int checkIdentifier(char *s);
%}

%%

{ws}										{}
{line}									{ line_no++; }
auto										{ return (AUTO_SYM); }
break										{ return (BREAK_SYM); }
case										{ return (CASE_SYM); }
continue								{ return (CONTINUE_SYM); }
default									{ return (DEFAULT_SYM); }
do											{ return (DO_SYM); }
else										{ return (ELSE_SYM); }
for											{ return (FOR_SYM); }
if											{ return (IF_SYM); }
return									{ return (RETURN_SYM); }
sizeof									{ return (SIZEOF_SYM); }
static									{ return (STATIC_SYM); }
struct									{ return (STRUCT_SYM); }
switch									{ return (SWITCH_SYM); }
typedef									{ return (SWITCH_SYM); }
union										{ return (UNION_SYM); }
while										{ return (WHILE_SYM); }
const										{ return (CONST_SYM); }

"\+\+"									{ return (PLUSPLUS); }
"\-\-"									{ return (MINUSMINUS); }
"\->"										{ return (ARROW); }
"<"											{ return (LSS); }
">"											{ return (GTR); }
"<="										{ return (LEQ); }
">="										{ return (GEQ); }
"=="										{ return (EQL); }
"!="										{ return (NEQ); }
"&&"										{ return (AMPAMP); }
"||"										{ return (BARBAR); }
"\.\.\."								{ return (DOTDOTDOT); }
"\("										{ return (LP); }
"\)"										{ return (RP); }
"\["										{ return (LB); }
"\]"										{ return (RB); }
"\{"										{ return (LR); }
"\}"										{ return (RR); }
"\:"										{ return (COLON); }
"\."										{ return (PERIOD); }
"\,"										{ return (COMMA); }
"\!"										{ return (EXCL); }
"\*"										{ return (STAR); }
"\/"										{ return (SLASH); }
"\%"										{ return (PERCENT); }
"\&"										{ return (AMP); }
"\;"										{ return (SEMICOLON); }
"\+"										{ return (PLUS); }
"\-"										{ return (MINUS); }
"\="										{ return (ASSIGN); }

{digit}+													{ yylval.i=atoi(yytext); return (INTEGER_CONSTANT); }
{digit}+\.{digit}+								{ yylval.s=makeString(yytext); return (FLOAT_CONSTANT); }
{letter}({letter}|{digit})*				{ return checkIdentifier(yytext); }
\"(\\["\n]|[^"\n])*\"							{ yylval.s=makeString(yytext); return (STRING_LITERAL); }
\'(\\['a-zA-z]|[^'\n])\'					{ yylval.c=*(yytext+1); return (CHARACTER_CONSTANT); }
\/\*([^*\/]|[^*]\/|\*[^\/])*\*\/  { }
\/\/[^\n]*												{ }

%%

char *makeString(char *s) {
	char *t;
	t = malloc(strlen(s)+1);
	strcpy(t, s);
	return t;
}

int checkIdentifer(char *s) {
	yylval.s = makeString(s);
	if (strcmp(s, "int") == 0) {
		return TYPE_IDENTIFIER;
	}
	else if (strcmp(s, "char") == 0) {
		return TYPE_IDENTIFIER;
	}
	else if (strcmp(s, "void") == 0) {
		return TYPE_IDENTIFIER;
	}
	else if (strcmp(s, "float") == 0) {
		return TYPE_IDENTIFIER;
	}
	else if (strcmp(s, "double") == 0) {
		return TYPE_IDENTIFIER;
	}
	else {
		return IDENTIFIER;
	}
}
