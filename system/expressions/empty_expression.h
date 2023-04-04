#ifndef SYSTEM_EXPRESSIONS_EMPTY_EXPRESSION_H_
#define SYSTEM_EXPRESSIONS_EMPTY_EXPRESSION_H_

#include "expression.h"

/**
 * An empty expression. This serves as a placeholder in the store of expressions
 * for an unused expression and to ensure that the expression will not be
 * incorrectly used (i.e. rather than storing some invalid value of another type
 * of expression that is always assumed to be active, this class of  expression
 * means that there is no expression present).
 */
class EmptyExpression : public Expression {
 public:
  /**
   * Construct an empty expression.
   */
  EmptyExpression();

  // The only expression type that is actually empty.
  bool isEmpty() const;

  // Does nothing
  void evaluate(FilterData_s *filterData,
                ExpressionPtrCallback &expressionPtrCallback);

  int toString(char *buffer, int n,
               ExpressionPtrCallback &expressionPtrCallback) const;

  void serializeInto(SerializedExpression_s *serialized) const;
};

#endif  // SYSTEM_EXPRESSIONS_EMPTY_EXPRESSION_H_
