#ifndef TASKS_EXPRESSIONS_UNARY_FUNC_EXPRESSION_H_
#define TASKS_EXPRESSIONS_UNARY_FUNC_EXPRESSION_H_

#include "expression.h"

/**
 * Enum of possible unary operations.
 */
typedef enum {
    not_o = 0,
    always_o,
    ever_o,
    sin_o,
    cos_o,
    tan_o,
    NUM_UNARY_FUNCTION
} UnaryFunction_e;

/**
 * Definition of a UnaryFunction lambda.
 */
using UnaryFunction = void(Expression *expr, FilterData_s *filterData, Expression *operand);

/**
 * A wrapper class for a lambda that makes it easier to use.
 */
class UnaryFunctionWrapper {
    UnaryFunction *function;
    ExpressionValueType_e opType;
    uint16_t stringLen;
public:
    char stringRep[10];
    ExpressionValueType_e valueType;

    /**
     * Construct a unary function wrapper
     * @param stringRep String name of the function.
     * @param function The lambda function.
     * @param opType Type of operand.
     * @param valueType The type of result this function returns.
     */
    UnaryFunctionWrapper(const char *stringRep,
                         UnaryFunction *function,
                         ExpressionValueType_e opType,
                         ExpressionValueType_e valueType);

    /**
     * Evaluate this wrapper's function.
     * @param expr Expression using this wrapper.
     * @param filterData
     * @param op1 The expression operand.
     */
    void evaluate(Expression *expr,
                  FilterData_s *filterData,
                  Expression *op1);

    /**
     * See if this wrapper's string name matches a slice of a string.
     * @param slice
     * @return bool of match or not.
     */
    bool matchesSlice(StringSlice &slice);

    /**
     * See if this wrapper matches a type of operand.
     * @param type Potential operand type.
     * @return bool of match or not.
     */
    bool acceptsArgumentType(ExpressionValueType_e type);
};

extern UnaryFunctionWrapper unaryFunctionWrappers[NUM_UNARY_FUNCTION];

/**
 * An expression that performs an operation on one other expression.
 */
class UnaryFuncExpression : public Expression {
protected:
    UnaryFunction_e opcode;
    uint16_t operandID;
public:
    /**
     * Construct a UnaryFuncExpression
     * @param triggerNum Trigger number to associate with.
     * @param opcode Opcode for the function.
     * @param operandID Expression ID of the operand.
     */
    UnaryFuncExpression(int triggerNum, UnaryFunction_e opcode, uint16_t operandID) :
            opcode{opcode}, operandID{operandID} {
        this->setTriggerNum(triggerNum);
    }

    void evaluate(FilterData_s *filterData, Expression *expressions[]);

    int toString(char *buffer, int n, Expression *expressions[]);

    void serializeInto(SerializedExpression_s *serialized);
};


#endif  // TASKS_EXPRESSIONS_UNARY_FUNC_EXPRESSION_H_

