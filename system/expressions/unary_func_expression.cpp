#include "unary_func_expression.h"

#include <cmath>
#include "string_slice.h"
#include <string.h>
#include <cstdio>

UnaryFunctionWrapper::UnaryFunctionWrapper(const char *stringRep,
                                           UnaryFunction *function,
                                           ExpressionValueType_e opType,
                                           ExpressionValueType_e valueType,
										   bool defaultValue) {
    strncpy(this->stringRep, stringRep, 10);
    this->stringLen = strlen(stringRep);
    this->opType = opType;
    this->function = function;
    this->valueType = valueType;
    this->defaultValue = defaultValue;
}

void UnaryFunctionWrapper::evaluate(Expression *expr,
                                    FilterData_s *filterData,
                                    Expression *op1) {
    this->function(expr, filterData, op1);
}

bool UnaryFunctionWrapper::matchesSlice(StringSlice &slice) {
    return slice == this->stringRep;
}

bool UnaryFunctionWrapper::acceptsArgumentType(ExpressionValueType_e type) {
    return this->opType == type;
}

bool UnaryFunctionWrapper::getDefaultBoolean() {
	return this->defaultValue;
}


#define UNARY_FUNCTION_HEADER [](Expression *expr, \
FilterData_s *filterData, \
Expression *operand)

static auto notFunc = UNARY_FUNCTION_HEADER {
    expr->setBooleanValue(!operand->getBooleanValue());
};

static auto alwaysFunc = UNARY_FUNCTION_HEADER {
    expr->setBooleanValue(expr->getBooleanValue() && operand->getBooleanValue());
};

static auto everFunc = UNARY_FUNCTION_HEADER {
    expr->setBooleanValue(expr->getBooleanValue() || operand->getBooleanValue());
};

static auto sinFunc = UNARY_FUNCTION_HEADER {
    expr->setNumberValue(sin(operand->getNumberValue()));
};

static auto cosFunc = UNARY_FUNCTION_HEADER {
    expr->setNumberValue(cos(operand->getNumberValue()));
};

static auto tanFunc = UNARY_FUNCTION_HEADER {
    expr->setNumberValue(tan(operand->getNumberValue()));
};

UnaryFunctionWrapper unaryFunctionWrappers[NUM_UNARY_FUNCTION] = {
        UnaryFunctionWrapper("not", notFunc, boolean, boolean, false),
        UnaryFunctionWrapper("always", alwaysFunc, boolean, boolean, true),
        UnaryFunctionWrapper("ever", everFunc, boolean, boolean, false),
        UnaryFunctionWrapper("sin", sinFunc, number, number, false),
        UnaryFunctionWrapper("cos", cosFunc, number, number, false),
        UnaryFunctionWrapper("tan", tanFunc, number, number, false),
};

int UnaryFuncExpression::toString(char *buffer, int n, Expression *expressions[]) {
    int selfLength = strlen(unaryFunctionWrappers[this->opcode].stringRep);
    snprintf(buffer, n, "(%s ", unaryFunctionWrappers[this->opcode].stringRep);
    if (selfLength + 2 < n) {
        int operandLen = expressions[this->operandID]->toString(buffer + 2 + selfLength, n - selfLength - 2, expressions);
        if (operandLen + selfLength + 2 < n) {
            buffer[operandLen + selfLength + 2] = ')';
            buffer[operandLen + selfLength + 2 + 1] = '\0';
            return operandLen + selfLength + 3;
        }
        return operandLen + selfLength + 2;
    }
    return selfLength;
}

void UnaryFuncExpression::evaluate(FilterData_s *filterData, Expression *expressions[]) {
    unaryFunctionWrappers[this->opcode].evaluate(this, filterData, expressions[operandID]);
}

void UnaryFuncExpression::serializeInto(SerializedExpression_s *serialized) {
	serialized->triggerNum = this->triggerNum;
	serialized->type = unaryFunc;
	serialized->contents.unary.opcode = this->opcode;
	serialized->contents.unary.operandID = this->operandID;
}

bool UnaryFuncExpression::getDefaultValue() {
	return unaryFunctionWrappers[this->opcode].getDefaultBoolean();
}
