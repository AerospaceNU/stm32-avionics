#ifndef COMMON_SYSTEM_EXPRESSIONS_EXPRESSION_SHAREDTYPES_H_
#define COMMON_SYSTEM_EXPRESSIONS_EXPRESSION_SHAREDTYPES_H_

#include <inttypes.h>

#include "event_manager.h"
#include "filterdata_types.h"

// The maximum number of expressions that will be allocated.
#define MAX_EXPRESSION 50

// The possible types an expression can have.
typedef enum {
  empty,
  event,
  variable,
  constant,
  unaryFunc,
  binaryFunc
} ExpressionType_e;

/**
 * To store an expression, it must be serialized into a form where it can be
 * reconstructed. These structs define the information necessary to store an
 * expression.
 */
typedef struct {
  uint16_t opcode;
  uint16_t operandID;
} SerializedUnaryFunc_s;

typedef struct {
  uint16_t opcode;
  uint16_t operand1ID;
  uint16_t operand2ID;
} SerializedBinaryFunc_s;

typedef union {
  Event_e event;
  float constant;
  FilterData_e variable;
  SerializedUnaryFunc_s unary;
  SerializedBinaryFunc_s binary;
} SerializedExpression_u;

typedef struct {
  uint16_t triggerNum;
  ExpressionType_e type;
  SerializedExpression_u contents;
} SerializedExpression_s;

#endif  // COMMON_SYSTEM_EXPRESSIONS_EXPRESSION_SHAREDTYPES_H_
