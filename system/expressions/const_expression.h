#ifndef TASKS_EXPRESSIONS_CONST_EXPRESSION_H_
#define TASKS_EXPRESSIONS_CONST_EXPRESSION_H_

#include "expression.h"
#include "dtoa.h"
#include <cstdio>

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


#endif  // TASKS_EXPRESSIONS_CONST_EXPRESSION_H_

