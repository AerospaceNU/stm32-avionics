#include "expression_store.h"

#include <functional>

#include "cli.h"
#include "expression_builder.h"

using std::placeholders::_1;

static VarExpressionBuilder varExpressionBuilder;
static ConstExpressionBuilder constExpressionBuilder;
static UnaryFuncExpressionBuilder unaryFuncExpressionBuilder;
static BinaryFuncExpressionBuilder binaryFuncExpressionBuilder;
static EventExpressionBuilder eventExpressionBuilder;

static ExpressionBuilder* builders[] = {
    &varExpressionBuilder, &constExpressionBuilder, &eventExpressionBuilder,
    &unaryFuncExpressionBuilder, &binaryFuncExpressionBuilder};

static EmptyExpression emptyExpr;

void ExpressionStore::init() {
  for (int i = 0; i < MAX_EXPRESSION; ++i) {
    SerializedExpression_s* serialized = cli_getConfigs()->serializedExprs + i;
    switch (serialized->type) {
      case event:
        expressions[i] =
            EventExpression(serialized->triggerNum, serialized->contents.event);
        break;
      case variable:
        expressions[i] = VarExpression(serialized->triggerNum,
                                       serialized->contents.variable);
        break;
      case constant:
        expressions[i] = ConstExpression(serialized->triggerNum,
                                         serialized->contents.constant);
        break;
      case unaryFunc:
        expressions[i] = UnaryFuncExpression(
            serialized->triggerNum,
            (UnaryFunction_e)serialized->contents.unary.opcode,
            serialized->contents.unary.operandID);
        break;
      case binaryFunc:
        expressions[i] = BinaryFuncExpression(
            serialized->triggerNum,
            (BinaryFunction_e)serialized->contents.binary.opcode,
            serialized->contents.binary.operand1ID,
            serialized->contents.binary.operand2ID);
        break;
      default:
        expressions[i] = EmptyExpression();
        break;
    }
    expressionBuffer[i] = expressions[i];
  }
}

Expression* ExpressionStore::getExpressionPtr(uint16_t expressionNum) {
  ExpressionVariant_v* varPtr = &(expressions[expressionNum]);
  if (Expression* ret = std::get_if<BinaryFuncExpression>(varPtr)) {
    return ret;
  }
  if (Expression* ret = std::get_if<UnaryFuncExpression>(varPtr)) {
    return ret;
  }
  if (Expression* ret = std::get_if<ConstExpression>(varPtr)) {
    return ret;
  }
  if (Expression* ret = std::get_if<EventExpression>(varPtr)) {
    return ret;
  }
  if (Expression* ret = std::get_if<VarExpression>(varPtr)) {
    return ret;
  }
  if (Expression* ret = std::get_if<EmptyExpression>(varPtr)) {
    return ret;
  }
  return &emptyExpr;
}

void ExpressionStore::removeExpressionsForTrigger(int triggerNum) {
  for (int i = 0; i < MAX_EXPRESSION; ++i) {
    if (std::visit([](auto&& var) { return var.triggerNum; },
                   expressionBuffer[i]) == triggerNum) {
      expressionBuffer[i] = EmptyExpression();
    }
  }
}

uint16_t ExpressionStore::getNextExpressionSpot(uint16_t startAt) const {
  if (startAt < 0 || startAt >= MAX_EXPRESSION) {
    return -1;
  }
  for (uint16_t i = startAt; i < MAX_EXPRESSION; ++i) {
    if (std::visit([](auto&& var) { return var.isEmpty(); },
                   expressionBuffer[i])) {
      return i;
    }
  }
  return -1;
}

ExpressionValueType_e ExpressionStore::parseForTrigger(uint16_t* resultID,
                                                       uint16_t triggerNum,
                                                       const StringSlice& slice,
                                                       uint16_t startAt) {
  if (startAt < 0 || startAt >= MAX_EXPRESSION) {
    return ExpressionValueType_e::invalid_type;
  }
  uint16_t nextSpot = getNextExpressionSpot(startAt);
  if (nextSpot < 0) {
    return ExpressionValueType_e::invalid_type;
  }
  ExpressionVariant_v* buildInto = &expressionBuffer[nextSpot];
  ExpressionValueType_e parseResult;
  for (ExpressionBuilder* builder : builders) {
    parseResult = builder->build(slice, this, triggerNum, buildInto, nextSpot);
    if (parseResult != ExpressionValueType_e::invalid_type) {
      *resultID = nextSpot;
      return parseResult;
    }
  }
  return ExpressionValueType_e::invalid_type;
}

bool ExpressionStore::getExprBoolValue(uint16_t expressionNum) const {
  if (expressionNum >= MAX_EXPRESSION || expressionNum < 0) {
    return false;
  }
  return std::visit([&](auto&& var) { return var.getBooleanValue(); },
                    expressions[expressionNum]);
}

float ExpressionStore::getExprNumValue(uint16_t expressionNum) const {
  if (expressionNum >= MAX_EXPRESSION || expressionNum < 0) {
    return false;
  }
  return std::visit([&](auto&& var) { return var.getNumberValue(); },
                    expressions[expressionNum]);
}

void ExpressionStore::tick(FilterData_s* filterData) {
  std::function<Expression*(uint16_t)> pointerCallback =
      std::bind(&ExpressionStore::getExpressionPtr, this, _1);
  for (int i = MAX_EXPRESSION - 1; i >= 0; --i) {
    std::visit([&](auto&& var) { var.evaluate(filterData, pointerCallback); },
               expressions[i]);
  }
}

void ExpressionStore::writeNewConfigs() {
  for (int i = 0; i < MAX_EXPRESSION; ++i) {
    expressions[i] = expressionBuffer[i];
    std::visit(
        [&](auto&& var) {
          var.serializeInto(cli_getConfigs()->serializedExprs + i);
        },
        expressions[i]);
  }
}

void ExpressionStore::conditionToString(uint16_t expressionNum, char* buffer,
                                        int n) {
  if (expressionNum >= MAX_EXPRESSION || expressionNum < 0) {
    return;
  }

  std::function<Expression*(uint16_t)> pointerCallback =
      std::bind(&ExpressionStore::getExpressionPtr, this, _1);
  std::visit([&](auto&& var) { var.toString(buffer, n, pointerCallback); },
             expressions[expressionNum]);
}
