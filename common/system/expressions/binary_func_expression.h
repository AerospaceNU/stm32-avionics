#ifndef COMMON_SYSTEM_EXPRESSIONS_BINARY_FUNC_EXPRESSION_H_
#define COMMON_SYSTEM_EXPRESSIONS_BINARY_FUNC_EXPRESSION_H_

#include "expression.h"
#include "string_slice.h"

// Binary functions
typedef enum {
  and_o = 0,
  or_o,
  leq_o,
  geq_o,
  lt_o,
  gt_o,
  eq_o,
  after_o,
  for_o,
  add_o,
  sub_o,
  mul_o,
  div_o,
  NUM_BINARY_FUNCTION
} BinaryFunction_e;

/**
 * Lambda for a binary function.
 */
using BinaryFunction = void(Expression *expr, FilterData_s *filterData,
                            Expression *op1, Expression *op2);

/**
 * A wrapper class for a binary function lambda that makes it easier to use.
 * This wrapper class provides the utility of evaluating a lambda function with
 * some other information about how the function works and what arguments it
 * takes.
 */
class BinaryFunctionWrapper {
 private:
  BinaryFunction *function;
  ExpressionValueType_e op1Type;
  ExpressionValueType_e op2Type;
  uint16_t stringLen;

 public:
  char stringRep[10];
  ExpressionValueType_e valueType;

  BinaryFunctionWrapper(const char *stringRep, BinaryFunction *function,
                        ExpressionValueType_e op1Type,
                        ExpressionValueType_e op2Type,
                        ExpressionValueType_e valueType);

  void evaluate(Expression *expr, FilterData_s *filterData, Expression *op1,
                Expression *op2);

  bool matchesSlice(const StringSlice &slice);

  bool acceptsArgument1Type(ExpressionValueType_e type);

  bool acceptsArgument2Type(ExpressionValueType_e type);
};

extern BinaryFunctionWrapper binaryFunctionWrappers[NUM_BINARY_FUNCTION];

/**
 * A binary function expression that has an operation function and two
 * expressions to use as operands. Evaluating a binary function expression
 * passes the values of the two operands to the function and stores that result
 * as this expression's value.
 */
class BinaryFuncExpression : public Expression {
 private:
  BinaryFunction_e opcode;
  uint16_t operand1ID;
  uint16_t operand2ID;

 public:
  /**
   * Construct a BinaryFuncExpression.
   * @param triggerNum Trigger num that this expression is used for.
   * @param opcode One of the enum values for a binary function operation.
   * @param operand1ID The ID of the expression used as the first operand.
   * @param operand2ID The ID of the expression used as the second operand.
   */
  BinaryFuncExpression(uint16_t triggerNum, BinaryFunction_e opcode,
                       uint16_t _operand1ID, uint16_t _operand2ID)
      : opcode{opcode}, operand1ID{_operand1ID}, operand2ID{_operand2ID} {
    this->setTriggerNum(triggerNum);
  }

  void evaluate(FilterData_s *filterData,
                ExpressionPtrCallback &expressionPtrCallback);

  int toString(char *buffer, int n,
               ExpressionPtrCallback &expressionPtrCallback) const;

  void serializeInto(SerializedExpression_s *serialized) const;
};

#endif  // COMMON_SYSTEM_EXPRESSIONS_BINARY_FUNC_EXPRESSION_H_
