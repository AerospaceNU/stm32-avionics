#include "binary_func_expression.h"

#include <cmath>
#include <string.h>
#include "hardware_manager.h"
#include <cstdio>

BinaryFunctionWrapper::BinaryFunctionWrapper(const char *stringRep,
                                             BinaryFunction *function,
                                             ExpressionValueType_e op1Type,
                                             ExpressionValueType_e op2Type,
                                             ExpressionValueType_e valueType) {
    strncpy(this->stringRep, stringRep, 10);
    this->stringLen = strlen(stringRep);
    this->op1Type = op1Type;
    this->op2Type = op2Type;
    this->function = function;
    this->valueType = valueType;
}

void BinaryFunctionWrapper::evaluate(Expression *expr,
                                     FilterData_s *filterData,
                                     Expression *op1,
                                     Expression *op2) {
    this->function(expr, filterData, op1, op2);
}

bool BinaryFunctionWrapper::matchesSlice(StringSlice &slice) {
    return slice == this->stringRep;
}

bool BinaryFunctionWrapper::acceptsArgument1Type(ExpressionValueType_e type) {
    return this->op1Type == type;
}

bool BinaryFunctionWrapper::acceptsArgument2Type(ExpressionValueType_e type) {
    return this->op2Type == type;
}


#define BINARY_FUNCTION_HEADER [](Expression *expr, \
FilterData_s *filterData, \
Expression *op1, \
Expression *op2)

static auto andFunc = BINARY_FUNCTION_HEADER {
    expr->setBooleanValue(op1->getBooleanValue() && op2->getBooleanValue());
};

static auto orFunc = BINARY_FUNCTION_HEADER {
    expr->setBooleanValue(op1->getBooleanValue() || op2->getBooleanValue());
};

static auto leqFunc = BINARY_FUNCTION_HEADER {
    expr->setBooleanValue(op1->getNumberValue() <= op2->getNumberValue());
};

static auto geqFunc = BINARY_FUNCTION_HEADER {
    expr->setBooleanValue(op1->getNumberValue() >= op2->getNumberValue());
};

static auto ltFunc = BINARY_FUNCTION_HEADER {
    expr->setBooleanValue(op1->getNumberValue() < op2->getNumberValue());
};

static auto gtFunc = BINARY_FUNCTION_HEADER {
    expr->setBooleanValue(op1->getNumberValue() > op2->getNumberValue());
};

static auto eqFunc = BINARY_FUNCTION_HEADER {
    expr->setBooleanValue(abs(op1->getNumberValue() - op2->getNumberValue()) < 0.001);
};

static auto afterFunc = BINARY_FUNCTION_HEADER {
    float conditionalTime = op1->getNumberValue();
    int firstTrue = op2->firstTrue;
    expr->setBooleanValue(firstTrue != -1 && (int)((int)hm_millis() - (int)firstTrue) > conditionalTime);
};

static auto forFunc = BINARY_FUNCTION_HEADER {
    int trueSince = op1->trueSince;
    float conditionalTime = op2->getNumberValue();
    expr->setBooleanValue(trueSince != -1 && op1->getBooleanValue() && (int)((int)hm_millis() - (int)trueSince) > conditionalTime);
};

static auto addFunc = BINARY_FUNCTION_HEADER {
    expr->setNumberValue(op1->getNumberValue() + op2->getNumberValue());
};

static auto subFunc = BINARY_FUNCTION_HEADER {
    expr->setNumberValue(op1->getNumberValue() - op2->getNumberValue());
};

static auto mulFunc = BINARY_FUNCTION_HEADER {
    expr->setNumberValue(op1->getNumberValue() * op2->getNumberValue());
};

static auto divFunc = BINARY_FUNCTION_HEADER {
    expr->setNumberValue(op1->getNumberValue() / op2->getNumberValue());
};

BinaryFunctionWrapper binaryFunctionWrappers[NUM_BINARY_FUNCTION] = {
        BinaryFunctionWrapper("and", andFunc, boolean, boolean, boolean),
        BinaryFunctionWrapper("or", orFunc, boolean, boolean, boolean),
        BinaryFunctionWrapper("<=", leqFunc, number, number, boolean),
        BinaryFunctionWrapper(">=", geqFunc, number, number, boolean),
        BinaryFunctionWrapper("<", ltFunc, number, number, boolean),
        BinaryFunctionWrapper(">", gtFunc, number, number, boolean),
        BinaryFunctionWrapper("==", eqFunc, number, number, boolean),
        BinaryFunctionWrapper("after", afterFunc, number, boolean, boolean),
        BinaryFunctionWrapper("for", forFunc, boolean, number, boolean),
        BinaryFunctionWrapper("+", addFunc, number, number, number),
        BinaryFunctionWrapper("-", subFunc, number, number, number),
        BinaryFunctionWrapper("*", mulFunc, number, number, number),
        BinaryFunctionWrapper("/", divFunc, number, number, number)
};

int BinaryFuncExpression::toString(char *buffer, int n, Expression *expressions[]) {
    int selfLength = strlen(binaryFunctionWrappers[this->opcode].stringRep);
    if (n == 0) {
        return 0;
    }
    buffer[0] = '(';
    int operand1Len = expressions[this->operand1ID]->toString(buffer + 1, n - 1, expressions);
    int remaining = n - operand1Len - 1;
    if (remaining > selfLength) {
        snprintf(buffer + operand1Len + 1, remaining, " %s ", binaryFunctionWrappers[this->opcode].stringRep);
        remaining -= selfLength + 2;
    }
    int operand2Len = 0;
    if (remaining > 0) {
        operand2Len = expressions[this->operand2ID]->toString(buffer + 1 + operand1Len + selfLength + 2,
                                                                 remaining, expressions);
        remaining -= operand2Len;
    }
    if (remaining > 0) {
        buffer[1 + operand1Len + 2 + selfLength + operand2Len] = ')';
        buffer[1 + operand1Len + 2 + selfLength + operand2Len + 1] = '\0';

    }
    return 1 + operand1Len + 2 + selfLength + operand2Len + 1;
}

void BinaryFuncExpression::evaluate(FilterData_s *filterData, Expression *expressions[]) {
    binaryFunctionWrappers[this->opcode].evaluate(this, filterData, expressions[operand1ID], expressions[operand2ID]);
}

void BinaryFuncExpression::serializeInto(SerializedExpression_s *serialized) {
	serialized->triggerNum = this->triggerNum;
	serialized->type = binaryFunc;
	serialized->contents.binary.opcode = this->opcode;
	serialized->contents.binary.operand1ID = this->operand1ID;
	serialized->contents.binary.operand2ID = this->operand2ID;
}
