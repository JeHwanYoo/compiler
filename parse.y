%{
union YYTYPE {
	int i;
	char c;
	char *s;
}
union YYTYPE yylval;
int line_no;
%}
%token AUTO_SYM BREAK_SYM CASE_SYM CONTINUE_SYM DEFAULT_SYM DO_SYM ELSE_SYM FOR_SYM IF_SYM RETURN_SYM SIZEOF_SYM STATIC_SYM STRUCT_SYM SWITCH_SYM TYPEDEF_SYM UNION_SYM ENUM_SYM WHITE_SYM CONST_SYM
%token PLUSPLUSH MINUSMINUS ARROW GTR LEQ GEQ EQL NEQ AMPAMP BARBAR DOTDOTDOT LP RP LB RB LR RR COLON PERIOD COMMA EXCL STAR SLASH PERCENT AMP SEMICOLON PLUS MINUS ASSIGN
%token INTEGER_CONSTANT FLOAT_CONSTANT CHARACTER_CONSTANT STRING_CONSTANT STRING_LITERAL
%token TYPE_IDENTIFER IDENTIFIER
%%

program
	: translation_unit

translation_unit
	: external_declaration
	| translation_unit external_declaration

external_declaration
	: function_definition
	| declaration

function_definition
	: declaration_specifiers declarator compound_statement
	| declarator compound_statement

declaration
	: declaration_specifiers SEMICOLON
	| declaration_specifiers init_declarator_list SEMICOLON

declaration_specifiers
	: type_specifier
	| storage_class_specifier
	| type_specifier declaration_specifiers
	| storage_class_specifier declaration_specifiers

storage_class_specifier
	: AUTO_SYM
	| STATIC_SYM
	| TYPEDEF_SYM

init_declarator_list
	: init_declarator
	| init_declarator_list COMMA init_declarator

init_declarator
	: declarator
	| declarator ASSIGN initializer

type_specifier
	: struct_specifier
	| enum_specifier
	| TYPE_IDENTIFER

struct_specifier
	: struct_or_union IDENTIFIER LR struct_declaration_list RR
	| struct_or_union LR struct_declaration_list RR
	| struct_or_union IDENTIFIER

struct_or_union
	: STRUCT_SYM
	| UNION_SYM

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration

struct_declaration
	: type_specifier struct_declarator_list SEMICOLON

struct_declarator_list
	: struct_declarator
	| struct_declarator COMMA struct_declarator

struct_declarator
	: declarator

enum_specifier
	: ENUM_SYM IDENTIFIER LR enumerator_list RR
	| ENUM_SYM LR enumerator_list RR
	| ENUM_SYM IDENTIFIER

enumerator_list
	: enumerator
	| enumerator_list COMMA enumerator

enumerator
	: IDENTIFIER
	| IDENTIFIER ASSIGN constant_expression

declarator
	: pointer direct_declarator
	| direct_declarator

pointer
	: STAR
	| STAR pointer

direct_declarator
	: IDENTIFIER
	| LP declarator RP
	| direct_declarator LB constant_expression_opt RB
	| direct_declarator LP parameter_type_list_opt RP

constant_expression_opt
	: /* empty */
	| constant_expression

parameter_type_list_opt
	: /* empty */
	| parameter_type_list

parameter_type_list
	: parameter_list

parameter_list
	: parameter_declaration
	| parameter_list COMMA parameter_declaration

parameter_declaration
	: declaration_specifiers declaration
	| declaration_specifiers abstract_declarator
	| declaration_specifiers

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator

direct_abstract_declarator
	: LP abstract_declarator RP
	| LB constant_expression_opt RB
	| LP parameter_type_list_opt RP
	| direct_abstract_declarator LB constant_expression_opt RB
	| direct_abstract_declarator LP parameter_type_list_opt RP

%%

int main(void)
{
	yyparse();
	return 0;
}
