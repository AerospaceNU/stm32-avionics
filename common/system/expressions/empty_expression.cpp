#include "empty_expression.h"

EmptyExpression::EmptyExpression() {
  this->setBooleanValue(false);
  this->triggerNum = -1;
}

// The only expression type that is actually empty.
bool EmptyExpression::isEmpty() const { return true; }

// Does nothing
void EmptyExpression::evaluate(FilterData_s *filterData,
                               ExpressionPtrCallback &expressionPtrCallback) {
  return;
}

int EmptyExpression::toString(
    char *buffer, int n, ExpressionPtrCallback &expressionPtrCallback) const {
  strncpy(buffer, "empty", n);
  return 5;
}

void EmptyExpression::serializeInto(SerializedExpression_s *serialized) const {
  serialized->triggerNum = this->triggerNum;
  serialized->type = empty;
}
