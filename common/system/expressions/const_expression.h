#ifndef COMMON_SYSTEM_EXPRESSIONS_CONST_EXPRESSION_H_
#define COMMON_SYSTEM_EXPRESSIONS_CONST_EXPRESSION_H_

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
  ConstExpression(uint16_t triggerNum, float value);

  /**
   * Construct a ConstExpression.
   * @param triggerNum Trigger num that this expression is used for.
   * @param value Int value of this expression (will be converted to float).
   */
  ConstExpression(uint16_t triggerNum, int value);

  void evaluate(FilterData_s* filterData,
                ExpressionPtrCallback& expressionPtrCallback);

  int toString(char* buffer, int n,
               ExpressionPtrCallback& expressionPtrCallback) const;

  void serializeInto(SerializedExpression_s* serialized) const;
};

#endif  // COMMON_SYSTEM_EXPRESSIONS_CONST_EXPRESSION_H_
