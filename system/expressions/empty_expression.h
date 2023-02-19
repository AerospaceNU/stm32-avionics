//
// Created by sam on 2/14/23.
//

#ifndef CTEST_EMPTY_EXPRESSION_H
#define CTEST_EMPTY_EXPRESSION_H

#include "expression.h"

/**
 * An empty expression.
 */
class EmptyExpression : public Expression {
public:
    EmptyExpression() {
        this->setBooleanValue(false);
        this->triggerNum = -1;
    }

    // The only expression type that is actually empty.
    bool isEmpty() {
        return true;
    }

    // Does nothing
    void evaluate(FilterData_s *filterData, Expression *expressions[]) {
        return;
    }

    int toString(char *buffer, int n, Expression *expressions[]) {
        strncpy(buffer, "empty", n);
        return 5;
    }

    void serializeInto(SerializedExpression_s *serialized) {
    	serialized->triggerNum = this->triggerNum;
    	serialized->type = empty;
    }

};

#endif //CTEST_EMPTY_EXPRESSION_H
