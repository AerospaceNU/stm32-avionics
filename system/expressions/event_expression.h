#ifndef SYSTEM_EXPRESSIONS_EVENT_EXPRESSION_H_
#define SYSTEM_EXPRESSIONS_EVENT_EXPRESSION_H_

#include <cstdio>

#include "event_manager.h"
#include "expression.h"
extern const char *eventStrings[];

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

#endif  // SYSTEM_EXPRESSIONS_EVENT_EXPRESSION_H_
