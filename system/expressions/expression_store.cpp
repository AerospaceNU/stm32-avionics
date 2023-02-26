#include "expression_store.h"

#include "cli.h"
#include "expression_builder.h"

static VarExpressionBuilder varExpressionBuilder;
static ConstExpressionBuilder constExpressionBuilder;
static UnaryFuncExpressionBuilder unaryFuncExpressionBuilder;
static BinaryFuncExpressionBuilder binaryFuncExpressionBuilder;
static EventExpressionBuilder eventExpressionBuilder;

static ExpressionBuilder *builders[] = {
    &varExpressionBuilder, &constExpressionBuilder, &eventExpressionBuilder,
    &unaryFuncExpressionBuilder, &binaryFuncExpressionBuilder};

/**
 * A variant_cast allows a typesafe method to cast a variant to a supertype
 * where every possible type of that variant is a subclass of the supertype.
 * @tparam T The final type to return as.
 */
template <typename T>
constexpr auto variant_cast = [](auto &&var) -> T {
  return std::visit([](auto &&var) -> T { return &var; }, *var);
};

ExpressionStore::ExpressionStore() {
  // Generate a list of pointers
  for (int i = 0; i < MAX_EXPRESSION; ++i) {
    SerializedExpression_s *serialized = cli_getConfigs()->serializedExprs + i;
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

    this->expressionPtrs[i] = variant_cast<Expression *>(&expressions[i]);
    expressionBuffer[i] = expressions[i];
  }
}

void ExpressionStore::removeExpressionsForTrigger(int triggerNum) {
  for (int i = 0; i < MAX_EXPRESSION; ++i) {
    Expression *buffExpr = variant_cast<Expression *>(&expressionBuffer[i]);
    if (buffExpr->triggerNum == triggerNum) {
      expressionBuffer[i] = EmptyExpression();
    }
  }
}

int ExpressionStore::getNextExpressionSpot(int startAt) {
  for (int i = startAt; i < MAX_EXPRESSION; ++i) {
    Expression *buffExpr = variant_cast<Expression *>(&expressionBuffer[i]);
    if (buffExpr->isEmpty()) {
      return i;
    }
  }
  return -1;
}

ExpressionValueType_e ExpressionStore::parseForTrigger(int *resultID,
                                                       int triggerNum,
                                                       const StringSlice &slice,
                                                       int startAt = 0) {
  int nextSpot = getNextExpressionSpot(startAt);
  if (nextSpot < 0) {
    return ExpressionValueType_e::invalid_type;
  }
  ExpressionVariant_v *buildInto = &expressionBuffer[nextSpot];
  ExpressionValueType_e parseResult;
  for (ExpressionBuilder *builder : builders) {
    parseResult = builder->build(slice, this, triggerNum, buildInto, nextSpot);
    if (parseResult != ExpressionValueType_e::invalid_type) {
      *resultID = nextSpot;
      return parseResult;
    }
  }
  return ExpressionValueType_e::invalid_type;
}

bool ExpressionStore::getStatusFor(uint16_t expressionNum) {
  if (expressionNum >= MAX_EXPRESSION) {
    return false;
  }
  return expressionPtrs[expressionNum]->getBooleanValue();
}

void ExpressionStore::tick(FilterData_s *filterData) {
  for (int i = MAX_EXPRESSION - 1; i >= 0; --i) {
    this->expressionPtrs[i] = variant_cast<Expression *>(&expressions[i]);
  }
  for (int i = MAX_EXPRESSION - 1; i >= 0; --i) {
    expressionPtrs[i]->evaluate(filterData, expressionPtrs);
  }

// <<<<<<< HEAD
//     for (int i = MAX_EXPRESSION - 1; i >= 0; --i) {
// #ifdef IS_DESKTOP_SIM
//         // At least on Matt's laptop, we need to re-cast this every loop
//         // (Or, specifically the second loop?)
//         // do it every time for safety
//         variant_cast<Expression *>(expressions[i])->evaluate(filterData, expressionPtrs);
// #else
//         expressionPtrs[i]->evaluate(filterData, expressionPtrs);
// #endif // IS_DESKTOP_SIM
//     }
// =======
//   for (int i = MAX_EXPRESSION - 1; i >= 0; --i) {
//     expressionPtrs[i]->evaluate(filterData, expressionPtrs);
//   }
// >>>>>>> expression-config
}

void ExpressionStore::writeNewConfigs() {
  for (int i = 0; i < MAX_EXPRESSION; ++i) {
    expressions[i] = expressionBuffer[i];
    this->expressionPtrs[i] = variant_cast<Expression *>(&expressions[i]);
    this->expressionPtrs[i]->serializeInto(cli_getConfigs()->serializedExprs +
                                           i);
  }
}

void ExpressionStore::conditionToString(uint16_t expressionNum, char *buffer,
                                        int n) {
  if (expressionNum >= MAX_EXPRESSION) {
    return;
  }
  expressionPtrs[expressionNum]->toString(buffer, n, expressionPtrs);
}
