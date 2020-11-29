#include "type.h"
void semantic_analysis(A_NODE *);
void set_literal_address(A_NODE *);
int put_literal(A_LITERAL, int);
void sem_program(A_NODE *);
A_TYPE *sem_expression(A_NODE *);
int sem_statement(A_NODE *, int, A_TYPE *, BOOLEAN, BOOLEAN,
		BOOLEAN);
int sem_statement_list(A_NODE *, int, A_TYPE *, BOOLEAN, BOOLEAN,
		BOOLEAN);
void sem_for_expression(A_NODE *);
int sem_A_TYPE(A_TYPE *);
int sem_declaration_list(A_ID *, int);
int sem_declaration(A_ID *, int);
A_LITERAL getTypeAndValueOfExpression(A_NODE *);
void semantic_warning(int, int);
void semantic_error();
int global_address = 12;
int semantic_err = 0;
#define LIT_MAX 100
A_LITERAL literal_table[LIT_MAX];
int literal_no = 0;
int literal_size = 0;
void semantic_analysis(A_NODE *node)
{
	sem_program(node);
	set_literal_address(node);
}
void set_literal_address(A_NODE *node)
{
	int i;
	for (i = 1; i <= literal_no; i++)
		literal_table[i].addr += node->value;
	node->value += literal_size;
}
void sem_program(A_NODE *node)
{
	int i;
	switch (node->name)
	{
		case N_PROGRAM:
			i = sem_declaration_list(node->clink, 12);
			node->value=global_address;
			break;
		default:
			semantic_error(90, node->line);
			break;
	}
}
int put_literal(A_LITERAL lit, int ll)
{
	float ff;
	if (literal_no >= LIT_MAX)
		semantic_error(93, ll);
	else
		literal_no++;
	literal_table[literal_no] = lit;
	literal_table[literal_no].addr = literal_size;
	if (lit.type->kind == T_ENUM)
		literal_size += 4;
	else if (isStringType(lit.type))
		literal_size += strlen(lit.value.s) + 1;
	if (literal_size % 4)
		literal_size = literal_size / 4 * 4 + 4;
	return (literal_no);
}
A_TYPE *sem_expression(A_NODE *node)
{
	A_TYPE *result = NIL, *t, *t1, *t2;
	A_ID *id;
	A_LITERAL lit;
	int i;
	BOOLEAN lvalue = FALSE;
	switch (node->name)
	{
		case N_EXP_IDENT:
			id = node->clink;
			switch (id->kind) {
				case ID_VAR:
				case ID_PARM:
					result = id->type;
					if (!isArrayType(result))
						lvalue = TRUE;
					break;
				case ID_FUNC:
					result = id->type;
					break;
				case ID_ENUM_LITERAL:
					result = int_type;
					break;
				default:
					semantic_error(38, node->line, id->name);
					break;
			}
			break;
		case N_EXP_INT_CONST:
			result = int_type;
			break;
		case N_EXP_FLOAT_CONST:
			lit.type = float_type;
			lit.value.f = atof(node->clink);
			node->clink = put_literal(lit, node->line);
			result = float_type;
			break;
		case N_EXP_CHAR_CONST:
			result = char_type;
			break;
		case N_EXP_STRING_LITERAL:
			lit.type = float_type;
			lit.value.f = atof(node->clink);
			node->clink = put_literal(lit, node->line);
			result = float_type;
			break;
		case N_EXP_ARRAY:
			t1 = sem_expression(node->llink);
			t2 = sem_expression(node->rlink);
			// usual binary conversion
			t = convertUsualBinaryConversion(node);
			t1 = node->llink->type;
			t2 = node->rlink->type;
			if (isPointerOrArrayType(t1)) 
				result = t1->element_type;
			else
				semantic_erorr(32, node->line);
			if (!isIntegralType(t2))
				semantic_error(29, node->line);
			if (!isArrayType(result))
				lvalue = TRUE;
			break;
		case N_EXP_STRUCT:
			t = sem_expression(node->llink);
			id = getStructFieldIdentifier(t, node->rlink);
			if (id) {
				result = id -> type;
				if (node->llink->value && !isArrayType(result))
					lvalue = TRUE;
			}
			else
				semantic_error(37, node->line);
			node->rlink = id;
			break;
		case N_EXP_ARROW:
			t = sem_expression(node->llink);
			id = getPointerFieldIdentifier(t, node->rlink);
			if (id) {
				result = id->type;
				if (!isArrayType(result))
					lvalue = TRUE;
			}
			else
				semantic_error(37, node->line);
			node->rlink = id;
			break;
		case N_EXP_FUNCTION_CALL:
			t = sem_expression(node->llink);
			// usual unary conversion
			node->llink = convertUsualUnaryConversion(node->llink);
			t = node->llink->type;
			if (isPointerType(t) && isFunctionType(t->element_type)) {
				sem_arg_expr_list(node->rlink, t->element_type->field);
				result = t->element_type->element_type;
				else
					semantic_error(21, node->line);
				break;
				case N_EXP_POST_INC:
				case N_EXP_POST_DEC:
				result = sem_expression(node->clink);
				// usual binary conversion between the expression and 1
				if (!isScalarType(result))
					semantic_error(27, node->line);
				// check if modifiable lvalue
				if (!isModifiableLvalue(node->clink))
					semantic_error(60, node->line);
				break;
				case N_EXP_CAST:
				result = node->llink;
				i = sem_A_TYPE(result);
				t = sem_expression(node->rlink);
				// check allowable casting conversion
				if (!isAllowableCastingConversion(result, t))
					semantic_error(58, node->line);
				break;
				case N_EXP_SIZE_TYPE:
				t = node->clink;
				i = sem_A_TYPE(t);
				// check if incomplete array, function, void
				if (isArrayType(y) && t->size == 0 || isFunctionType(t) || isVoidType(t))
					semantic_error(39, node->line);
				else
					node->clink = i;
				result = int_type;
				break;
				case N_EXP_SIZE_EXP:
				t = node->clink;
				// check if incomplete array, function
				if ((node->clink->name != N_EXP_IDENT || ((A_ID*)node->clink->clink)->kind != ID_PARM) &&
						(isArrayType(t) && t->size == 0 || isFunctionType(t)))
					semantic_error(39, node->line);
				else
					node->clink = t->size;
				result = int_type;
				break;
				case N_EXP_PLUS:
				case N_EXP_MINUS:
					t = sem_expression(node->clink);
					if (isArithmeticType(t)) {
						node->clink = convertUsualUnaryConversion(node->clink);
						result = node->clink->type;
					}
					else 
						semantic_error(13, node->line);
					break;
				case N_EXP_NOT:
					t = sem_expressio(node->clink);
					if (isScalarType(t)) {
						node->clink = convertUsualUnaryConversion(node->clink);
						result = int_type;
					}
					else
						semantic_error(27, node->line);
					break;
				case N_EXP_AMP:
					t = sem_expression(node->clink);
					if (node->clink->value == TRUE || isFunctionType(t)) {
						result = setTypeElementType(makeType(T_POINTER), t);
						result->size = 4;
					}
					else
						semantic_error(60, node->line);
					break;
				case N_EXP_STAR:
					t = sem_expression(node->clink);
					node->clink = convertUsualUnaryConversion(node->clink);
					if (isPointerType(t)) {
						result = t->element_type;
						// lvalue if points to an object
						if (isStructOrUnionType(result) || isScalarType(result))
							lvalue = true;
					}
					else
						semantic_error(31, node->line);
					break;
				case N_EXP_PRE_INC:
				case N_EXP_PRE_DEC:
					result = sem_expression(node->clink);
					// unary binary conversion between the expression and 1
					if (!isScalarType(result))
						semantic_error(27, node->line);
					// check if modifiable lvalue
					if (!isModifiableLvalue(node->clink))
						semantic_error(60, node->line);
					break;
				case N_EXP_MUL:
				case N_EXP_DIV:
					t1 = sem_expression(node->llink);
					t2 = sem_expression(node->rlink);
					if (isArithmeticType(t1) && isArithmeticType(t2))
						result = convertUsualBinaryConversion(node);
					else
						semantic_error(28, node->line);
					break;
				case N_EXP_MOD:
					t1 = sem_expression(node->llink);
					t2 = sem_expression(node->rlink);
					if (isIntegralType(t1) && isIntegralType(t2))
						result = convertUsualBinaryConversion(node);
					else
						semantic_error(29, node->line);
					result = int_type;
					break;
				case N_EXP_ADD:
					t1 = sem_expression(node->llink);
					t2 = sem_expression(node->rlink);
					if (isArithmeticType(t1) && isArithmeticType(t2))
						result = convertUsualBinaryConversion(t2);
					else if (isPointerType(t1) && isIntegralType(t2))
						result = t1;
					else if (isIntegralType(t1) && isPointerType(t2))
						result = t2;
					else
						semantic_error(24, node->line);
					break;
				case N_EXP_SUB:
					t1 = sem_expression(node->llink);
					t2 = sem_expression(node->rlink);
					if (isArithmeticType(t1) && isArithmeticType(t2))
						result = convertUsualBinaryConversion(node);
					else if (isPointerType(t1) && isIntegralType(t2))
						result = t1;
					else if (isCompatiblePointerType(t1, t2))
						result = t1;
					else
						semantic_error(24, node->line);
					break;
				case N_EXP_LSS:
				case N_EXP_GTR:
				case N_EXP_LEQ:
				case N_EXP_GEQ:
					t1 = sem_expression(node->llink);
					t2 = sem_expression(node->rlink);
					if (isArithmeticType(t1) && isArithmeticType(t2))
						t = convertUsualBinaryConversion(node);
					else if (!isCompatiblePointerType(t1, t2))
						semantic_error(40, node->line);
					result = int_type;
					break;
				case N_EXP_NEQ:
				case N_EXP_EQL:
					t1 = sem_expression(node->llink);
					t2 = sem_expression(node->rlink);
					if (isArithmeticType(t1) && isArithmeticType(t2))
						t = convertUsualBinaryConversion(node);
					else if (!isCompatiblePointerType(t1, t2) &&
							(!isPointerType(t1) || !isConstantZeroExp(node->rlink)) &&
							(!isPointerType(t2) || !isConstantZeroExp(node->llink)))
						semantic_error(40, node->line);
					result = int_type;
					break;
				case N_EXP_AND:
				case N_EXP_OR:
					t = sem_expression(node->llink);
					if (isScalarType(t))
						node->llink = convertUsualUnaryConversion(node->llink);
					else
						semantic_error(27, node->line);
					t = sem_expression(node->rlink);
					if (isScalarType(t))
						node->rlink = convertUsualUnaryConversion(node->rlink);
					else
						semantic_error(27, node->line);
					result = int_type;
					break;
				case N_EXP_ASSIGN:
					result = sem_expression(node->llink);
					// check if modifiable lvalue
					if (!isModifiableLvalue(node->llink))
						semantic_error(60, node->line);
					t = sem_expression(node->rlink);
					if (isAllowableAssignmentConversion(result, t, node->rlink)) {
						if (isArithmeticType(result) && isArithmeticType(t))
							node->rlink = convertUsualAssignmentConversion(result, node->rlink);
					}
					else
						semantic_error(58, node->line);
					break;
				default:
					semantic_error(90, node->line);
					break;
			}
			node->type = result;
			node->value = lvalue;
			return result;
	}
	int sem_statement(A_NODE *node, int addr, A_TYPE *ret, BOOLEAN sw, BOOLEAN brk, BOOLEAN cnt)
	{
		switch (node->name)
		{
			case N_STMT_LABEL_CASE:
			case N_STMT_LABEL_DEFAULT:
			case N_STMT_COMPOUND:
			case N_STMT_EMPTY:
			case N_STMT_EXPRESSION:
			case N_STMT_IF:
			case N_STMT_IF_ELSE:
			case N_STMT_SWITCH:
			case N_STMT_WHILE:
			case N_STMT_DO:
			case N_STMT_FOR:
			case N_STMT_CONTINUE:
			case N_STMT_BREAK:
			case N_STMT_RETURN:
		}
	}
	void sem_for_expression(A_NODE *node)
	{
		switch (node->name)
		{
			case N_FOR_EXP:
		}
	}
	int sem_statement_list(A_NODE *node, int addr, A_TYPE *ret, BOOLEAN sw,
			BOOLEAN brk, BOOLEAN cnt)
	{
		switch (node->name)
		{
			case N_STMT_LIST:
			case N_STMT_LIST_NIL:
		}
	}
	int sem_A_TYPE(A_TYPE *t)
	{
		switch (t->kind)
		{
			case T_NULL:
			case T_ENUM:
			case T_ARRAY:
			case T_STRUCT:
			case T_UNION:
			case T_FUNC:
			case T_POINTER:
		}
	}
	int sem_declaration_list(A_ID *id, int addr)
	{
		while (id)
		{
			sem_declaration(id, r);
			id = id->link;
		}
	}
	int sem_declaration(A_ID *id, int addr)
	{
		switch (id->kind)
		{
			case ID_VAR:
			case ID_FIELD:
			case ID_FUNC:
			case ID_PARM:
			case ID_TYPE:
		}
	}
	A_LITERAL getTypeAndValueOfExpression(A_NODE *node)
	{
		A_TYPE *t;
		A_ID *id;
		A_LITERAL result, r;
		result.type = NIL;
		switch (node->name)
		{
			case N_EXP_IDENT:
				id = node->clink;
				if (id->kind != ID_ENUM_LITERAL)
					semantic_error(19, node->line, id->name);
				else
				{
					result.type = int_type;
					result.value.i = id->init;
				}
				break;
			case N_EXP_INT_CONST:
				result.type = int_type;
				result.value.i = (int)node->clink;
				break;
			case N_EXP_CHAR_CONST:
				result.type = char_type;
				result.value.c = (char)node->clink;
				break;
			case N_EXP_FLOAT_CONST:
				result.type = float_type;
				result.value.f = atof(node->clink);
				break;
			case N_EXP_STRING_LITERAL:
			case N_EXP_ARRAY:
			case N_EXP_FUNCTION_CALL:
			case N_EXP_STRUCT:
			case N_EXP_ARROW:
			case N_EXP_POST_INC:
			case N_EXP_PRE_INC:
			case N_EXP_POST_DEC:
			case N_EXP_PRE_DEC:
			case N_EXP_AMP:
			case N_EXP_STAR:
			case N_EXP_NOT:
				semantic_error(18, node->line);
				break;
			case N_EXP_MINUS:
				result = getTypeAndValueOfExpression(node->clink);
				if (result.type == int_type)
					result.value.i = -result.value.i;
				else if (result.type == float_type)
					result.value.f = -result.value.f;
				else
					semantic_error(18, node->line);
				break;
			case N_EXP_SIZE_EXP:
				t = sem_expression(node->clink);
				result.type = int_type;
				result.value.i = t->size;
				break;
			case N_EXP_SIZE_TYPE:
				result.type = int_type;
				result.value.i = sem_A_TYPE(node->clink);
				break;
			case N_EXP_CAST:
				result = getTypeAndValueOfExpression(node->rlink);
				result = checkTypeAndConvertLiteral(
						result, (A_TYPE *)node->llink, node->line);
				break;
			case N_EXP_MUL:
				result = getTypeAndValueOfExpression(node->llink);
				r = getTypeAndValueOfExpression(node->rlink);
				if (result.type == int_type && r.type == int_type)
				{
					result.type = int_type;
					result.value.i = result.value.i * r.value.i;
				}
				else if (result.type == int_type && r.type == float_type)
				{
					result.type = float_type;
					result.value.f = result.value.i * r.value.f;
				}
				else if (result.type == float_type && r.type == int_type)
				{
					result.type = float_type;
					result.value.f = result.value.f * r.value.i;
				}
				else if (result.type == float_type && r.type == float_type)
				{
					result.type = float_type;
					result.value.f = result.value.f * r.value.f;
				}
				else
					semantic_error(18, node->line);
				break;
			case N_EXP_DIV:
				result = getTypeAndValueOfExpression(node->llink);
				r = getTypeAndValueOfExpression(node->rlink);
				if (result.type == int_type && r.type == int_type)
				{
					result.type = int_type;
					result.value.i = result.value.i / r.value.i;
				}
				else if (result.type == int_type && r.type == float_type)
				{
					result.type = float_type;
					result.value.f = result.value.i / r.value.f;
				}
				else if (result.type == float_type && r.type == int_type)
				{
					result.type = float_type;
					result.value.f = result.value.f / r.value.i;
				}
				else if (result.type == float_type && r.type == float_type)
				{
					result.type = float_type;
					result.value.f = result.value.f / r.value.f;
				}
				else
					semantic_error(18, node->line);
				break;
			case N_EXP_MOD:
				result = getTypeAndValueOfExpression(node->llink);
				r = getTypeAndValueOfExpression(node->rlink);
				if (result.type == int_type && r.type == int_type)
					result.value.i = result.value.i % r.value.i;
				else
					semantic_error(18, node->line);
				break;
			case N_EXP_ADD:
				result = getTypeAndValueOfExpression(node->llink);
				r = getTypeAndValueOfExpression(node->rlink);
				if (result.type == int_type && r.type == int_type)
				{
					result.type = int_type;
					result.value.i = result.value.i + r.value.i;
				}
				else if (result.type == int_type && r.type == float_type)
				{
					result.type = float_type;
					result.value.f = result.value.i + r.value.f;
				}
				else if (result.type == float_type && r.type == int_type)
				{
					result.type = float_type;
					result.value.f = result.value.f + r.value.i;
				}
				else if (result.type == float_type && r.type == float_type)
				{
					result.type = float_type;
					result.value.f = result.value.f + r.value.f;
				}
				else
					semantic_error(18, node->line);
				break;
			case N_EXP_SUB:
				result = getTypeAndValueOfExpression(node->llink);
				r = getTypeAndValueOfExpression(node->rlink);
				if (result.type == int_type && r.type == int_type)
				{
					result.type = int_type;
					result.value.i = result.value.i - r.value.i;
				}
				else if (result.type == int_type && r.type == float_type)
				{
					result.type = float_type;
					result.value.f = result.value.i - r.value.f;
				}
				else if (result.type == float_type && r.type == int_type)
				{
					result.type = float_type;
					result.value.f = result.value.f - r.value.i;
				}
				else if (result.type == float_type && r.type == float_type)
				{
					result.type = float_type;
					result.value.f = result.value.f - r.value.f;
				}
				else
					semantic_error(18, node->line);
				break;
			case N_EXP_LSS:
			case N_EXP_GTR:
			case N_EXP_LEQ:
			case N_EXP_GEQ:
			case N_EXP_NEQ:
			case N_EXP_EQL:
			case N_EXP_AND:
			case N_EXP_OR:
			case N_EXP_ASSIGN:
				semantic_error(18, node->line);
				break;
			default:
				semantic_error(90, node->line);
				break;
		}
		return (result);
	}
	void semantic_error(int i, int ll, char *s)
	{
		semantic_err++;
		printf("*** semantic error at line %d: ", ll);
		switch (i)
		{
			// errors in expression
			case 13:
				printf("arith type expr required in unary operation\n");
				break;
			case 18:
				printf("illegal constant expression \n");
				break;
			case 19:
				printf("illegal identifier %s in constant expression\n", s);
				break;
			case 21:
				printf("illegal type in function call expression\n");
				break;
			case 24:
				printf("incompatible type in additive expression\n");
				break;
			case 27:
				printf("scalar type expr required in expression\n");
				break;
			case 28:
				printf("arith type expression required in binary operation\n");
				break;
			case 29:
				printf("integral type expression required in expression\n");
				break;
			case 31:
				printf("pointer type expr required in pointer operation\n");
				break;
			case 32:
				printf("array type required in array expression\n");
				break;
			case 34:
				printf("too many arguments in function call\n");
				break;
			case 35:
				printf("too few arguments in function call\n");
				break;
			case 37:
				printf("illegal struct field identifier in struct reference expr\n");
				break;
			case 38:
				printf("illegal kind of identifier %s in expression\n");
				break;
			case 39:
				printf("illegal type size in sizeof operation\n");
				break;
			case 40:
				printf("illegal expression type in relational operation");
				break;
				//case 41:printf("incompatible type in literal\n");
				break;
				// erros in statement
			case 49:
				printf("scalar type expr required in middle of for-expr\n");
				break;
			case 50:
				printf("integral type expression required in statement\n");
				break;
			case 51:
				printf("illegal expression type in case label\n");
				break;
			case 57:
				printf("not permitted type conversion in return expression\n");
				break;
			case 58:
				printf("not permitted type casting in expression\n");
				break;
			case 59:
				printf("not permitted type conversion in argument\n");
				break;
			case 60:
				printf("expression is not an lvalue \n");
				break;
			case 71:
				printf("case label not within a switch statement \n");
				break;
			case 72:
				printf("default label not within a switch statement \n");
				break;
			case 73:
				printf("break statement not within loop or switch stmt\n");
				break;
			case 74:
				printf("continue statement not within a loop \n");
				break;
				// errors in type & declarator
			case 80:
				printf("undefined type\n");
				break;
			case 81:
				printf("integer type expression required in enumerator\n");
				break;
			case 82:
				printf("illegal array size or type\n");
				break;
			case 83:
				printf("illegal element type of array declarator\n");
				break;
			case 84:
				printf("illegal type in struct or union field\n");
				break;
			case 85:
				printf("invalid function return type\n");
				break;
			case 86:
				printf("illegal array size or empty array \n");
				break;
			case 89:
				printf("unknown identifier kind: %s\n", s);
				break;
				// misc errors
			case 90:
				printf("fatal compiler error in parse result\n");
				break;
			case 93:
				printf("too many literals in source program \n");
				break;
			default:
				printf("unknown \n");
				break;
		}
	}
	void semantic_warning(int i, int ll)
	{
		printf("--- warning at line %d:", ll);
		switch (i)
		{
			case 11:
				printf("incompatible types in assignment expression\n");
				break;
			case 12:
				printf("incompatible types in argument or return expr\n");
				break;
			case 14:
				printf("incompatible types in binary expression\n");
				break;
			case 16:
				printf("integer type expression is required\n");
				break;
			default:
				printf("unknown\n");
				break;
		}
	}
