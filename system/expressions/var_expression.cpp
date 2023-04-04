#include "var_expression.h"

#include <string.h>

VarExpression::VarExpression(int triggerNum, FilterData_e dataMember) {
  this->dataMember = dataMember;
  this->setTriggerNum(triggerNum);
}

void VarExpression::evaluate(FilterData_s *filterData,
                             ExpressionPtrCallback &expressionPtrCallback) {
  this->setNumberValue(getVariableValue(filterData, this->dataMember));
}

int VarExpression::toString(
    char *buffer, int n, ExpressionPtrCallback &expressionPtrCallback) const {
  fillVariableName(buffer, n, this->dataMember);
  return strnlen(buffer, 17);
}

void VarExpression::serializeInto(SerializedExpression_s *serialized) const {
  serialized->triggerNum = this->triggerNum;
  serialized->type = variable;
  serialized->contents.variable = this->dataMember;
}
