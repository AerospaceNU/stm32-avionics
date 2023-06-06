#ifndef COMMON_SYSTEM_EXPRESSIONS_UNARY_FUNC_EXPRESSION_H_
#define COMMON_SYSTEM_EXPRESSIONS_UNARY_FUNC_EXPRESSION_H_

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
using UnaryFunction = void(Expression *expr, FilterData_s *filterData,
                           Expression *operand);

/**
 * A wrapper class for a lambda that makes it easier to use. This wrapper class
 * provides the utility of evaluating a lambda function with some other
 * information about how the function works and what argument it takes.
 */
class UnaryFunctionWrapper {
  UnaryFunction *function;
  ExpressionValueType_e opType;
  uint16_t stringLen;
  bool defaultValue;

 public:
  char stringRep[10];
  ExpressionValueType_e valueType;

  /**
   * Construct a unary function wrapper
   * @param stringRep String name of the function.
   * @param function The lambda function.
   * @param opType Type of operand.
   * @param valueType The type of result this function returns.
   * @param defaultValue Default boolean value to set.
   */
  UnaryFunctionWrapper(const char *stringRep, UnaryFunction *function,
                       ExpressionValueType_e opType,
                       ExpressionValueType_e valueType, bool defaultValue);

  /**
   * Evaluate this wrapper's function.
   * @param expr Expression using this wrapper.
   * @param filterData
   * @param op1 The expression operand.
   */
  void evaluate(Expression *expr, FilterData_s *filterData, Expression *op1);

  /**
   * See if this wrapper's string name matches a slice of a string.
   * @param slice
   * @return bool of match or not.
   */
  bool matchesSlice(const StringSlice &slice);

  /**
   * See if this wrapper matches a type of operand.
   * @param type Potential operand type.
   * @return bool of match or not.
   */
  bool acceptsArgumentType(ExpressionValueType_e type);
  /**
   * Get the default value of this function.
   * @return bool of the default value.
   */
  bool getDefaultBoolean();
};

extern UnaryFunctionWrapper unaryFunctionWrappers[NUM_UNARY_FUNCTION];

/**
 * An expression that performs an operation on one other expression. A unary
 * function expression has two components, an operator that performs some
 * operation, and then the id of another expression to use as the operand of
 * that function.
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
  UnaryFuncExpression(uint16_t triggerNum, UnaryFunction_e _opcode,
                      uint16_t _operandID)
      : opcode{_opcode}, operandID{_operandID} {
    this->setTriggerNum(triggerNum);
  }

  void evaluate(FilterData_s *filterData,
                ExpressionPtrCallback &expressionPtrCallback);

  int toString(char *buffer, int n,
               ExpressionPtrCallback &expressionPtrCallback) const;

  void serializeInto(SerializedExpression_s *serialized) const;
};

#endif  // COMMON_SYSTEM_EXPRESSIONS_UNARY_FUNC_EXPRESSION_H_
