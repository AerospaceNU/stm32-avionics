#ifndef SYSTEM_EXPRESSIONS_EVENT_EXPRESSION_H_
#define SYSTEM_EXPRESSIONS_EVENT_EXPRESSION_H_

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
  Event_e event;

 public:
  /**
   * Construct an EventExpression.
   * @param triggerNum Trigger num that this expression is used for.
   * @param event The event to reference as this expression's value.
   */
  EventExpression(int triggerNum, Event_e event);

  void evaluate(FilterData_s *filterData, Expression *expressions[]);

  int toString(char *buffer, int n, Expression *expressions[]) const;

  void serializeInto(SerializedExpression_s *serialized) const;
};

#endif  // SYSTEM_EXPRESSIONS_EVENT_EXPRESSION_H_
