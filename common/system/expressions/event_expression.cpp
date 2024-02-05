#include "event_expression.h"

const char* eventStrings[] = {"launch", "burnout", "apogee", "touchdown",
                              "unclean_restart"};

EventExpression::EventExpression(uint16_t triggerNum, Event_e event) {
  this->setTriggerNum(triggerNum);
  this->event = event;
}

void EventExpression::evaluate(FilterData_s* filterData,
                               ExpressionPtrCallback& expressionPtrCallback) {
  this->setBooleanValue(eventManager_getEventStatus(this->event));
}

int EventExpression::toString(
    char* buffer, int n, ExpressionPtrCallback& expressionPtrCallback) const {
  snprintf(buffer, n, eventStrings[this->event]);
  return strlen(buffer);
}

void EventExpression::serializeInto(SerializedExpression_s* serialized) const {
  serialized->triggerNum = this->triggerNum;
  serialized->type = ExpressionType_e::event;
  serialized->contents.event = this->event;
}
