#ifndef COMMON_SYSTEM_EXPRESSIONS_EVENT_EXPRESSION_H_
#define COMMON_SYSTEM_EXPRESSIONS_EVENT_EXPRESSION_H_

#include <cstdio>

#include "event_manager.h"
#include "expression.h"
extern const char *eventStrings[];

/**
 * An event expression is an expression that evaluates directly to the status of
 * a given event, as enumerated in the event manager. Essentially serves as an
 * interface between the normal system code event manager and the events as used
 * in the expression configuration. An event expression will always be either
 * true or false, mirroring the state of the event it uses.
 */
class EventExpression : public Expression {
 private:
  Event event;

 public:
  /**
   * Construct an EventExpression.
   * @param triggerNum Trigger num that this expression is used for.
   * @param event The event to reference as this expression's value.
   */
  EventExpression(uint16_t triggerNum, Event event);

  void evaluate(FilterData_s *filterData,
                ExpressionPtrCallback &expressionPtrCallback);

  int toString(char *buffer, int n,
               ExpressionPtrCallback &expressionPtrCallback) const;

  void serializeInto(SerializedExpression_s *serialized) const;
};

#endif  // COMMON_SYSTEM_EXPRESSIONS_EVENT_EXPRESSION_H_
