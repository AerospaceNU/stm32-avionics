#ifndef COMMON_SYSTEM_EXPRESSIONS_VAR_EXPRESSION_H_
#define COMMON_SYSTEM_EXPRESSIONS_VAR_EXPRESSION_H_

#include "expression.h"

/**
 * An expression that evaluates to the value of some member of FilterData.
 * A VarExpression is bound to one of the members of FilterData and always
 * evaluates to the current value of that variable.
 */
class VarExpression : public Expression {
 private:
  FilterData_e dataMember;

 public:
  /**
   * Construct a VariableExpression.
   * @param triggerNum The trigger that this expression is associated with.
   * @param dataMember The filterdata member that this expression will evaluate
   * to.
   */
  VarExpression(uint16_t triggerNum, FilterData_e dataMember);

  void evaluate(FilterData_s* filterData,
                ExpressionPtrCallback& expressionPtrCallback);

  int toString(char* buffer, int n,
               ExpressionPtrCallback& expressionPtrCallback) const;

  void serializeInto(SerializedExpression_s* serialized) const;
};

#endif  // COMMON_SYSTEM_EXPRESSIONS_VAR_EXPRESSION_H_
