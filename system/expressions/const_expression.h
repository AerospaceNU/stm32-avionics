#ifndef SYSTEM_EXPRESSIONS_CONST_EXPRESSION_H_
#define SYSTEM_EXPRESSIONS_CONST_EXPRESSION_H_

#include <cstdio>

#include "dtoa.h"
#include "expression.h"

/**
 * A ConstExpression evaluates to some constant numerical value.
 * It can be initialized with a float, double, or int, but internally
 * is always represented as a float.
 */
class ConstExpression : public Expression {
 public:
  /**
   * Construct a ConstExpression.
   * @param triggerNum Trigger num that this expression is used for.
   * @param value Float value of this expression.
   */
  ConstExpression(int triggerNum, float value);

  /**
   * Construct a ConstExpression.
   * @param triggerNum Trigger num that this expression is used for.
   * @param value Double value of this expression (will be converted to float).
   */
  ConstExpression(int triggerNum, double value);

  /**
   * Construct a ConstExpression.
   * @param triggerNum Trigger num that this expression is used for.
   * @param value Int value of this expression (will be converted to float).
   */
  ConstExpression(int triggerNum, int value);

  void evaluate(FilterData_s *filterData, Expression *expressions[]);

  int toString(char *buffer, int n, Expression *expressions[]) const;

  void serializeInto(SerializedExpression_s *serialized) const;
};

#endif  // SYSTEM_EXPRESSIONS_CONST_EXPRESSION_H_
