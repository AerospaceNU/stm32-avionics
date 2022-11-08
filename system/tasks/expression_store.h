#ifndef SYSTEM_TASKS_EXPRESSION_STORE_H_
#define SYSTEM_TASKS_EXPRESSION_STORE_H_
#include "expression.h"
#include "filters.h"

#define MAX_EXPRESSION 50

class ExpressionStore {
  Expression expressions[MAX_EXPRESSION];

 public:
  ExpressionStore();
  void tick(FilterData_s *filterData, bool eventStatus[]);
  bool checkExpression(uint8_t exprNum);
};

#endif  // SYSTEM_TASKS_EXPRESSION_STORE_H_
