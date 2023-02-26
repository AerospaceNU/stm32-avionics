#include "event_expression.h"

const char *eventStrings[] = {"launch", "burnout", "apogee", "touchdown"};

EventExpression::EventExpression(int triggerNum, Event_e event) {
  this->setTriggerNum(triggerNum);
  this->event = event;
}

void EventExpression::evaluate(FilterData_s *filterData,
                               Expression *expressions[]) {
  this->setBooleanValue(eventManager_getEventStatus(this->event));
}

int EventExpression::toString(char *buffer, int n,
                              Expression *expressions[]) const {
  snprintf(buffer, n, eventStrings[this->event]);
  return strlen(buffer);
}

void EventExpression::serializeInto(SerializedExpression_s *serialized) const {
  serialized->triggerNum = this->triggerNum;
  serialized->type = ExpressionType_e::event;
  serialized->contents.event = this->event;
}