#include "expression_store.h"

#include <math.h>

#include "hardware_manager.h"
static Expression expressions[MAX_EXPRESSION];

#define RESULT_INVALID                                             \
  ExpressionResult {                                               \
    ExpressionValueType::invalid, ExpressionValue { .boolean = 0 } \
  }

#define VERIFY_TYPES(typea, typeb) \
  (expressions[a].value_type == typea && expressions[b].value_type == typeb)
#define VERIFY_TYPE_A(typea) (expressions[a].value_type == typea)
#define VERIFY_TYPE_B(typeb) (expressions[b].value_type == typeb)

typedef ExpressionResult (*ExprOperation)(uint16_t, uint16_t);

static ExprOperation funcs[] = {
    // and_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPES (ExpressionValueType::boolean,
                       ExpressionValueType::boolean) {
        return ExpressionResult{
            ExpressionValueType::boolean,
            ExpressionValue{.boolean = expressions[a].value.boolean &&
                                       expressions[b].value.boolean}};
      }
      return RESULT_INVALID;
    },
    // or_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPES (ExpressionValueType::boolean,
                       ExpressionValueType::boolean) {
        return ExpressionResult{
            ExpressionValueType::boolean,
            ExpressionValue{.boolean = expressions[a].value.boolean ||
                                       expressions[b].value.boolean}};
      }
      return RESULT_INVALID;
    },
    // not_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPE_A (ExpressionValueType::boolean) {
        return ExpressionResult{
            ExpressionValueType::boolean,
            ExpressionValue{.boolean = !expressions[a].value.boolean}};
      }
      return RESULT_INVALID;
    },
    // leq_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPES (ExpressionValueType::num, ExpressionValueType::num) {
        return ExpressionResult{
            ExpressionValueType::boolean,
            ExpressionValue{
                .boolean =
                    funcs[(uint8_t)OperationType::less_o](a, b).value.boolean ||
                    funcs[(uint8_t)OperationType::eq_o](a, b).value.boolean}};
      }
      return RESULT_INVALID;
    },
    // less_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPES (ExpressionValueType::num, ExpressionValueType::num) {
        return ExpressionResult{
            ExpressionValueType::boolean,
            ExpressionValue{.boolean = expressions[a].value.num <
                                       expressions[b].value.num}};
      }
      return RESULT_INVALID;
    },
    // geq_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPES (ExpressionValueType::num, ExpressionValueType::num) {
        return ExpressionResult{
            ExpressionValueType::boolean,
            ExpressionValue{
                .boolean =
                    funcs[(uint8_t)OperationType::greater_o](a, b)
                        .value.boolean ||
                    funcs[(uint8_t)OperationType::eq_o](a, b).value.boolean}};
      }
      return RESULT_INVALID;
    },
    // greater_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPES (ExpressionValueType::num, ExpressionValueType::num) {
        return ExpressionResult{
            ExpressionValueType::boolean,
            ExpressionValue{.boolean = expressions[a].value.num >
                                       expressions[b].value.num}};
      }
      return RESULT_INVALID;
    },
    // eq_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPES (ExpressionValueType::num, ExpressionValueType::num) {
        return ExpressionResult{
            ExpressionValueType::boolean,
            ExpressionValue{.boolean =
                                abs(expressions[a].value.num -
                                    expressions[b].value.num) < 1.0e-05}};
      }
      return RESULT_INVALID;
    },
    // after_o
    [](uint16_t a, uint16_t b) {
      if (VERIFY_TYPE_A(ExpressionValueType::num) &&
          VERIFY_TYPE_B(ExpressionValueType::boolean)) {
        return ExpressionResult{
            ExpressionValueType::boolean,
            ExpressionValue{.boolean =
                                hm_millis() - expressions[b].first_true >=
                                expressions[a].value.num}};
      }
      return RESULT_INVALID;
    },
    // for_o
    [](uint16_t a, uint16_t b) {
      if (VERIFY_TYPE_A(ExpressionValueType::num) &&
          VERIFY_TYPE_B(ExpressionValueType::boolean)) {
        return ExpressionResult{
            ExpressionValueType::boolean,
            ExpressionValue{.boolean =
                                hm_millis() - expressions[b].true_since >=
                                expressions[a].value.num}};
      }

      return RESULT_INVALID;
    },
    // add_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPES (ExpressionValueType::num, ExpressionValueType::num) {
        return ExpressionResult{
            ExpressionValueType::num,
            ExpressionValue{.num = expressions[a].value.num +
                                   expressions[b].value.num}};
      }
      return RESULT_INVALID;
    },
    // sub_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPES (ExpressionValueType::num, ExpressionValueType::num) {
        return ExpressionResult{
            ExpressionValueType::num,
            ExpressionValue{.num = expressions[a].value.num -
                                   expressions[b].value.num}};
      }
      return RESULT_INVALID;
    },
    // mul_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPES (ExpressionValueType::num, ExpressionValueType::num) {
        return ExpressionResult{
            ExpressionValueType::num,
            ExpressionValue{.num = expressions[a].value.num *
                                   expressions[b].value.num}};
      }
      return RESULT_INVALID;
    },
    // div_o
    [](uint16_t a, uint16_t b) {
      if VERIFY_TYPES (ExpressionValueType::num, ExpressionValueType::num) {
        return ExpressionResult{
            ExpressionValueType::num,
            ExpressionValue{.num = expressions[a].value.num /
                                   expressions[b].value.num}};
      }
      return RESULT_INVALID;
    }};

void ExpressionStore::tick(FilterData_s* filterData, bool eventStatus[]) {
  for (int i = MAX_EXPRESSION; i >= 0; --i) {
    expressions[i].evaluate(funcs, filterData, eventStatus);
  }
}
bool checkExpression(uint8_t exprNum) {
  return expressions[exprNum].value.boolean;
}

ExpressionStore::ExpressionStore() {
  expressions[10] = Expression((int32_t)100);
  expressions[9] = Expression((int32_t)100);
  expressions[8] = Expression(Operation{OperationType::geq_o, 9, 10});
  expressions[7] = Expression((int32_t)5);
  expressions[6] = Expression((int32_t)10);
  expressions[5] = Expression(Operation{OperationType::add_o, 6, 7});
  expressions[4] = Expression((int32_t)60);
  expressions[3] = Expression((int32_t)3.9998);
  expressions[2] = Expression(Operation{OperationType::div_o, 4, 3});
  expressions[1] = Expression(Operation{OperationType::eq_o, 2, 5});
  expressions[0] = Expression(Operation{OperationType::and_o, 1, 8});
}
