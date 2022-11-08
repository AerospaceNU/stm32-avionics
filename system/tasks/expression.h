#ifndef SYSTEM_TASKS_EXPRESSION_H_
#define SYSTEM_TASKS_EXPRESSION_H_

#include <inttypes.h>

#include "filters.h"

// A given expression has to evaluate to one of these types
// invalid is if evaluating fails.

enum class ExpressionValueType { num, boolean, invalid };

// From the above value type, this is the union defining what each
// result actually is
typedef union {
  float num;
  bool boolean;
} ExpressionValue;

// Supported variables (for now)
enum class Variable { world_acc_z, pos_z_agl, world_vel_z };

// Supported flight events
enum class Event {launch, apogee, touchdown};

// Possible types of an expression
enum class ExpressionType { operation, variable, num, boolean, event};

// For an operation, the supported operations there are
enum class OperationType {
  and_o,
  or_o,
  not_o,
  leq_o,
  less_o,
  geq_o,
  greater_o,
  eq_o,
  after_o,
  for_o,
  add_o,
  sub_o,
  mul_o,
  div_o
};

// An operation specifically needs to know the operation and the two
// expressions it uses as arguments. Some arguments might be a unary
// which is fine, they just use the first ID
typedef struct {
  OperationType op;
  uint16_t expr_id_1;
  uint16_t expr_id_2;
} Operation;

// The contents of an expression are either an Operation,
// the Variable, or just one of the possible literal values
typedef union {
  Operation operation;
  Variable variable;
  bool boolean;
  float num;
  Event event;
} ExpressionContents;

typedef struct {
  ExpressionValueType type;
  ExpressionValue value;
} ExpressionResult;

typedef ExpressionResult (*ExprOperation)(uint16_t, uint16_t);

// The overall struct to represent one expression
struct Expression {
  ExpressionType type;
  ExpressionContents contents;
  ExpressionValueType value_type;
  ExpressionValue value;
  // If boolean, the timestamp that started a continuous
  // run of being true
  uint32_t true_since;
  // If boolean, the timestamp where it first became true
  uint32_t first_true;
  // Constructor for an operation expression
  explicit Expression(Operation op);
  // Contructor for a number expression
  // note that we need to support both
  // float, double, and int
  explicit Expression(float num);
  explicit Expression(double num);
  explicit Expression(int32_t num);
  // Constructor for a boolean expression
  explicit Expression(bool boolean);
  // Constructor for a variable expression
  explicit Expression(Variable var);
  // Constructor for an event expression
  explicit Expression(Event event);
  // Empty constructor
  Expression();
  void evaluate(ExprOperation funcs[], FilterData_s *filterData, bool eventStatus[]);
};

#endif  // SYSTEM_TASKS_EXPRESSION_H_
