#include <iostream>
using namespace std;

enum class ExpressionType {
  operation,
  i32,
  u32,
  flt,
  boolean,
  variable,
};

enum class Operation {
  leq_o,
  le_o,
  geq_o,
  ge_o,
  eq_o,
  and_o,
  or_o,
  not_o,
  after_o,
  for_o,
  add_o,
  sub_o,
  mul_o,
  div_o
};

static int32_t add(int32_t a, int32_t b) {
  return a + b;
}

typedef struct {
  bool reached;
  uint32_t reachedTimestamp;
} event_s;

typedef struct {
  Operation op;
  ExpressionType expr1_type;
  uint16_t wrapper1_id;
  ExpressionType expr2_type;
  uint16_t wrapper2_id;
} func_s;

typedef union {
  int32_t literal_i32;
  uint32_t literal_u32;
  float literal_flt;
  bool literal_bool;
  uint32_t variable_id;
  func_s func;
} expression_s;

typedef struct {
  bool reached;
  uint32_t firstTrueTimestamp;
  bool active;
  uint32_t activeStartTimestamp;
  ExpressionType expression_type;
  uint16_t expression_id;
} expression_wrapper_s;

typedef union {
  bool boolean;
  int32_t i32;
  uint32_t u32;
  float flt;
} expr_result_u;

typedef struct  {
  ExpressionType type;
  expr_result_u result_value;
} expression_result_s;

static expression_wrapper_s wrappers[100];
static expression_s expressions[100];
static uint8_t numExpressions;

expression_result_s evaluate(expression_wrapper_s expr_wrapper) {
  expr_result_u result_value;
  expression_s expr = expressions[expr_wrapper.expression_id];
  ExpressionType result_type = expr_wrapper.expression_type;
  switch (result_type) {
    case ExpressionType::literal_flt:
      result_value = expr.literal_flt;
      break;
    case ExpressionType::literal_i32:
      result_value = expr.literal_i32;
      break;
    case ExpressionType::literal_u32:
      result_value = expr.literal_u32;
      break;
    case ExpressionType::literal_bool:
      result_value = expr.literal_bool;
      break;
      result_value.boolean =  expressions[expr_wrapper.expression_id];
      break;
  }
}

int main() {
  cout << "hello \n";
  return 0;
}
