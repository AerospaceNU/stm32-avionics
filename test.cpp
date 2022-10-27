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

typedef expression_result_s (*lambda)(expression_result_s, expression_result_s);

static lambda funcs[] = {
   [](expression_result_s a, expression_result_s b) {
        return expression_result_s{ExpressionType::boolean, a.result_value.boolean && b.result_value.boolean};
    }
};

int main() {
    expr_result_u param1, param2;
    param1.boolean = true;
    param2.boolean = false;
    cout << (funcs[0]({ExpressionType::boolean, param1}, {ExpressionType::boolean, true})).result_value.boolean << endl;
}
