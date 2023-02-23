#ifndef SYSTEM_EXPRESSIONS_VAR_EXPRESSION_H_
#define SYSTEM_EXPRESSIONS_VAR_EXPRESSION_H_

#include "expression.h"

/**
 * An expression that evaluates to the value of some member of FilterData.
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
  VarExpression(int triggerNum, FilterData_e dataMember) {
    this->dataMember = dataMember;
    this->setTriggerNum(triggerNum);
  }

  void evaluate(FilterData_s *filterData, Expression *expressions[]) {
    this->setNumberValue(getVariableValue(filterData, this->dataMember));
  }

  int toString(char *buffer, int n, Expression *expressions[]) {
    fillVariableName(buffer, n, this->dataMember);
    return strlen(buffer);
  }

  void serializeInto(SerializedExpression_s *serialized) {
    serialized->triggerNum = this->triggerNum;
    serialized->type = variable;
    serialized->contents.variable.variable = this->dataMember;
  }
};

#endif  // SYSTEM_EXPRESSIONS_VAR_EXPRESSION_H_
