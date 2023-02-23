#ifndef SYSTEM_EXPRESSIONS_BINARY_FUNC_EXPRESSION_H_
#define SYSTEM_EXPRESSIONS_BINARY_FUNC_EXPRESSION_H_

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
 * Wrapper for a binary function
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
 * A binary function expression.
 */
class BinaryFuncExpression : public Expression {
 private:
  BinaryFunction_e opcode;
  uint16_t operand1ID;
  uint16_t operand2ID;

 public:
  BinaryFuncExpression(int triggerNum, BinaryFunction_e opcode,
                       uint16_t operand1ID, uint16_t operand2ID)
      : opcode{opcode}, operand1ID{operand1ID}, operand2ID{operand2ID} {
    this->setTriggerNum(triggerNum);
  }

  void evaluate(FilterData_s *filterData, Expression *expressions[]);

  int toString(char *buffer, int n, Expression *expressions[]);

  void serializeInto(SerializedExpression_s *serialized);
};

#endif  // SYSTEM_EXPRESSIONS_BINARY_FUNC_EXPRESSION_H_
