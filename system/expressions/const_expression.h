#ifndef SYSTEM_EXPRESSIONS_CONST_EXPRESSION_H_
#define SYSTEM_EXPRESSIONS_CONST_EXPRESSION_H_

#include <cstdio>

#include "dtoa.h"
#include "expression.h"

class ConstExpression : public Expression {
 public:
  ConstExpression(int triggerNum, float value) {
    this->setNumberValue(value);
    this->setTriggerNum(triggerNum);
  }

  ConstExpression(int triggerNum, double value) {
    this->setNumberValue(value);
    this->setTriggerNum(triggerNum);
  }

  ConstExpression(int triggerNum, int value) {
    this->setNumberValue(value);
    this->setTriggerNum(triggerNum);
  }

  void evaluate(FilterData_s *filterData, Expression *expressions[]) override {
    return;
  }

  int toString(char *buffer, int n, Expression *expressions[]) {
    static char floatBuf[10];
    dtoa(floatBuf, 10, this->getNumberValue(), 2);
    snprintf(buffer, n, "%s", floatBuf);
    return strlen(buffer);
  }

  void serializeInto(SerializedExpression_s *serialized) {
    serialized->triggerNum = this->triggerNum;
    serialized->type = constant;
    serialized->contents.constant.value = this->getNumberValue();
  }
};

#endif  // SYSTEM_EXPRESSIONS_CONST_EXPRESSION_H_
