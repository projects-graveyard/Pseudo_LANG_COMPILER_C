#include <string.h>
#include <stdio.h>
#include "../include/operators.h"
#include "../include/utils.h"

#define OPERATORS_DEFINED 5

struct
{
	char signature[64];
	OPERATOR *operatorPtr;
} operators_array[OPERATORS_DEFINED];

OPERATOR ADD_NUMBER_NUMBER_OP;
OPERATOR ADD_VOID_NUMBER_OP;

OPERATOR MULTIPLY_NUMBER_NUMBER_OP;

OPERATOR SUBTRACT_NUMBER_NUMBER_OP;
OPERATOR SUBTRACT_VOID_NUMBER_OP;

TYPE ADD_Number_Number(CACHE_PTR a, TYPE b, ASMOP* memory, int* ptr, int* success);
TYPE ADD_Void_Number(CACHE_PTR a, TYPE b, ASMOP* memory, int* ptr, int* success);

TYPE MULTIPLY_Number_Number(CACHE_PTR a, TYPE b, ASMOP* memory, int* ptr, int* success);

TYPE SUBTRACT_Number_Number(CACHE_PTR a, TYPE b, ASMOP* memory, int* ptr, int* success);
TYPE SUBTRACT_Void_Number(CACHE_PTR a, TYPE b, ASMOP* memory, int* ptr, int* success);

void operators_init(){
	strcpy(operators_array[0].signature, "add<Number,Number>");
    ADD_NUMBER_NUMBER_OP.call = &ADD_Number_Number;
	operators_array[0].operatorPtr = &ADD_NUMBER_NUMBER_OP;

	strcpy(operators_array[1].signature, "add<Void,Number>");
    ADD_VOID_NUMBER_OP.call = &ADD_Void_Number;
	operators_array[1].operatorPtr = &ADD_VOID_NUMBER_OP;

	strcpy(operators_array[2].signature, "multiply<Number,Number>");
    MULTIPLY_NUMBER_NUMBER_OP.call = &MULTIPLY_Number_Number;
	operators_array[2].operatorPtr = &MULTIPLY_NUMBER_NUMBER_OP;

	strcpy(operators_array[3].signature, "subtract<Number,Number>");
    SUBTRACT_NUMBER_NUMBER_OP.call = &SUBTRACT_Number_Number;
	operators_array[3].operatorPtr = &SUBTRACT_NUMBER_NUMBER_OP;

	strcpy(operators_array[4].signature, "subtract<Void,Number>");
    SUBTRACT_VOID_NUMBER_OP.call = &SUBTRACT_Void_Number;
	operators_array[4].operatorPtr = &SUBTRACT_VOID_NUMBER_OP;
}

OPERATOR *getOperator(const char *signature)
{
	int left = 0, right = OPERATORS_DEFINED - 1;
	while (left <= right)
	{
		int mid = (left + right) / 2;
		int cmp_result = strcmp(signature, operators_array[mid].signature);
		if (cmp_result < 0)
			right = mid - 1;
		else if (cmp_result > 0)
			left = mid + 1;
		else
			return operators_array[mid].operatorPtr;
	}
	return NULL;
}


TYPE ADD_Number_Number(CACHE_PTR a, TYPE b, ASMOP* memory, int* ptr, int* success){
    strcpy(memory[*ptr].operation, "addq");
    strcpy(memory[*ptr].operand1, "CACHE_MEM+");
    num_to_str(a->cache_index, memory[*ptr].operand1+10);
    strcpy(memory[*ptr].operand2, "%rax");
    memory[*ptr].operand3[0] = '\0';
    (*ptr)++;
    TYPE t;
    t.typeid = NUMBER_TYPE;
    t.saved = NULL;
    return t;
}

TYPE ADD_Void_Number(CACHE_PTR a, TYPE b, ASMOP* memory, int* ptr, int* success){
    // Shouldn't do anything
    TYPE t;
    t.typeid = NUMBER_TYPE;
    t.saved = NULL;
    return t;
}

TYPE SUBTRACT_Number_Number(CACHE_PTR a, TYPE b, ASMOP* memory, int* ptr, int* success){
    strcpy(memory[*ptr].operation, "subq");
    strcpy(memory[*ptr].operand1, "%rax");
    strcpy(memory[*ptr].operand2, "CACHE_MEM+");
    num_to_str(a->cache_index, memory[*ptr].operand2+10);
    memory[*ptr].operand3[0] = '\0';
    (*ptr)++;

    strcpy(memory[*ptr].operation, "movq");
    strcpy(memory[*ptr].operand1, "CACHE_MEM+");
    num_to_str(a->cache_index, memory[*ptr].operand1+10);
    strcpy(memory[*ptr].operand2, "%rax");
    memory[*ptr].operand3[0] = '\0';
    (*ptr)++;
    
    TYPE t;
    t.typeid = NUMBER_TYPE;
    t.saved = NULL;
    return t;
}

TYPE SUBTRACT_Void_Number(CACHE_PTR a, TYPE b, ASMOP* memory, int* ptr, int* success){
    strcpy(memory[*ptr].operation, "negq");
    strcpy(memory[*ptr].operand1, "%rax");
    memory[*ptr].operand2[0] = '\0';
    (*ptr)++;

    TYPE t;
    t.typeid = NUMBER_TYPE;
    t.saved = NULL;
    return t;
}

TYPE MULTIPLY_Number_Number(CACHE_PTR a, TYPE b, ASMOP* memory, int* ptr, int* success){
    strcpy(memory[*ptr].operation, "movq");
    strcpy(memory[*ptr].operand1, "CACHE_MEM+");
    num_to_str(a->cache_index, memory[*ptr].operand1 + 10);
    strcpy(memory[*ptr].operand2, "%r10");
    memory[*ptr].operand3[0] = '\0';
    (*ptr)++;

    strcpy(memory[*ptr].operation, "mul");
    strcpy(memory[*ptr].operand1, "%r10");
    memory[*ptr].operand2[0] = '\0';
    (*ptr)++;

    TYPE t;
    t.typeid = NUMBER_TYPE;
    t.saved = NULL;
    return t;
}