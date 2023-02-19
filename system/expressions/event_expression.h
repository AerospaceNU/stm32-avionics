#ifndef EXPRESSIONS_EVENT_EXPRESSION_H_
#define EXPRESSIONS_EVENT_EXPRESSION_H_

#include "expression.h"
#include "event_manager.h"
#include <cstdio>
static const char *eventStrings[] = {
	"launch",
	"burnout",
	"apogee",
	"touchdown"
};

class EventExpression : public Expression {
private:
	Event_e event;
public:
	EventExpression(int triggerNum, Event_e event) {
		this->setTriggerNum(triggerNum);
		this->event = event;
	}

	void evaluate(FilterData_s *filterData, Expression *expressions[]) {
		this->setBooleanValue(eventManager_getEventStatus(this->event));
	}

    int toString(char *buffer, int n, Expression *expressions[]) {
        snprintf(buffer, n, eventStrings[this->event]);
        return strlen(buffer);
    }

    void serializeInto(SerializedExpression_s *serialized) {
    	serialized->triggerNum = this->triggerNum;
    	serialized->type = ExpressionType_e::event;
    	serialized->contents.event.event = this->event;
    }

};

#endif /* EXPRESSIONS_EVENTEXPRESSION_H_ */
