#include "const_expression.h"

ConstExpression::ConstExpression(int triggerNum, float value) {
  this->setNumberValue(value);
  this->setTriggerNum(triggerNum);
}

ConstExpression::ConstExpression(int triggerNum, double value) {
  this->setNumberValue(value);
  this->setTriggerNum(triggerNum);
}

ConstExpression::ConstExpression(int triggerNum, int value) {
  this->setNumberValue(value);
  this->setTriggerNum(triggerNum);
}

void ConstExpression::evaluate(FilterData_s *filterData,
                               ExpressionPtrCallback &expressionPtrCallback) {
  return;
}

int ConstExpression::toString(
    char *buffer, int n, ExpressionPtrCallback &expressionPtrCallback) const {
  static char floatBuf[10];
  dtoa(floatBuf, 10, this->getNumberValue(), 2);
  snprintf(buffer, n, "%s", floatBuf);
  return strlen(buffer);
}

void ConstExpression::serializeInto(SerializedExpression_s *serialized) const {
  serialized->triggerNum = this->triggerNum;
  serialized->type = constant;
  serialized->contents.constant = this->getNumberValue();
}
