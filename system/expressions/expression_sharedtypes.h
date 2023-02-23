#ifndef SYSTEM_EXPRESSIONS_EXPRESSION_SHAREDTYPES_H_
#define SYSTEM_EXPRESSIONS_EXPRESSION_SHAREDTYPES_H_

#include "event_manager.h"
#include "filterdata_types.h"
#include "inttypes.h"

#define MAX_EXPRESSION 50

typedef enum {
  empty,
  event,
  variable,
  constant,
  unaryFunc,
  binaryFunc
} ExpressionType_e;

typedef struct {
  Event_e event;
} SerializedEvent_s;

typedef struct {
  float value;
} SerializedConstant_s;

typedef struct {
  FilterData_e variable;
} SerializedVariable_s;

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
  SerializedEvent_s event;
  SerializedConstant_s constant;
  SerializedVariable_s variable;
  SerializedUnaryFunc_s unary;
  SerializedBinaryFunc_s binary;
} SerializedExpression_u;

typedef struct {
  uint16_t triggerNum;
  ExpressionType_e type;
  SerializedExpression_u contents;
} SerializedExpression_s;

#endif  // SYSTEM_EXPRESSIONS_EXPRESSION_SHAREDTYPES_H_
