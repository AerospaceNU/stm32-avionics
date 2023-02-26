#include "expression_builder.h"
/**
 * Variable expression builder. This builder attempts to parse a string
 * as a variable name and create a VarExpression for that variable, if a match
 * is found.
 */

ExpressionValueType_e VarExpressionBuilder::build(
    const StringSlice &slice, ExpressionStore *expressionStore, int triggerNum,
    ExpressionVariant_v *parseInto, int destinationID) {
  FilterData_e variableType =
      getVariableEnumFromString((char *)slice.startPtr());
  if (variableType != FilterData_e::invalid) {
    *parseInto = VarExpression(triggerNum, variableType);
    return ExpressionValueType_e::number;
  }
  return ExpressionValueType_e::invalid_type;
}

/**
 * Event expression builder. This builder attempts to parse a string as
 * the name of an event and create an EventExpression for that event, if a match
 * is found.
 */

ExpressionValueType_e EventExpressionBuilder::build(
    const StringSlice &slice, ExpressionStore *expressionStore, int triggerNum,
    ExpressionVariant_v *parseInto, int destinationID) {
  for (int i = 0; i < Event_e::NUM_EVENT; ++i) {
    if (slice == eventStrings[i]) {
      *parseInto = EventExpression(triggerNum, (Event_e)i);
      return ExpressionValueType_e::boolean;
    }
  }
  return ExpressionValueType_e::invalid_type;
}

/**
 * Constant expression builder. This builder attempts to parse a string as
 * a float or integer and create a ConstExpression from that value, if a valid
 * constant is found.
 */
ExpressionValueType_e ConstExpressionBuilder::build(
    const StringSlice &slice, ExpressionStore *expressionStore, int triggerNum,
    ExpressionVariant_v *parseInto, int destinationID) {
  char *endptr;
  float value = smallStrtod((char *)slice.startPtr(), &endptr);

  if (endptr != slice.endptr()) {
    return ExpressionValueType_e::invalid_type;
  }
  *parseInto = ConstExpression(triggerNum, value);
  return ExpressionValueType_e::number;
}

/**
 * Unary Function expression builder. This attempts to parse a string
 * by checking if it is wrapped in (), verifying that there are 2 separate
 * arguments in the parentheses, checking if the first value is a valid unary
 * function name, and then checking if the second value is both a valid separate
 * expression and if the type of value it holds (boolean or number) is a valid
 * input to the function already determined.
 */

ExpressionValueType_e UnaryFuncExpressionBuilder::build(
    const StringSlice &slice, ExpressionStore *expressionStore, int triggerNum,
    ExpressionVariant_v *parseInto, int destinationID) {
  if (slice[0] != '(' || slice[slice.length() - 1] != ')') {
    return ExpressionValueType_e::invalid_type;
  }
  StringSlice slices[MAX_SLICE];

  StringSlice innerSlice = slice.inner();
  int argCount = innerSlice.split(slices, MAX_SLICE, ' ');
  if (argCount != 2) {
    return ExpressionValueType_e::invalid_type;
  }
  int argDest;
  ExpressionValueType_e argumentType = expressionStore->parseForTrigger(
      &argDest, triggerNum, slices[1], destinationID + 1);
  if (argDest < 0) {
    return ExpressionValueType_e::invalid_type;
  }
  for (int i = 0; i < NUM_UNARY_FUNCTION; ++i) {
    if (unaryFunctionWrappers[i].matchesSlice(slices[0])) {
      if (unaryFunctionWrappers[i].acceptsArgumentType(argumentType)) {
        *parseInto =
            UnaryFuncExpression(triggerNum, (UnaryFunction_e)i, argDest);
        return unaryFunctionWrappers[i].valueType;
      }
    }
  }
  return ExpressionValueType_e::invalid_type;
}

/**
 * Binary Function expression builder. This attempts to parse a string
 * by checking if it is wrapped in (), verifying that there are 3 separate
 * arguments in the parentheses, checking if the middle value is a valid
 * binary function name, and then checking if the first and third values are
 * both valid separate expressions and if the types of values they hold
 * (boolean or number) are valid inputs to the function already determined.
 */
ExpressionValueType_e BinaryFuncExpressionBuilder::build(
    const StringSlice &slice, ExpressionStore *expressionStore, int triggerNum,
    ExpressionVariant_v *parseInto, int destinationID) {
  StringSlice slices[MAX_SLICE];
  if (slice[0] != '(' || slice[slice.length() - 1] != ')') {
    return ExpressionValueType_e::invalid_type;
  }
  StringSlice innerSlice = slice.inner();
  int argCount = innerSlice.split(slices, MAX_SLICE, ' ');
  if (argCount != 3) {
    return ExpressionValueType_e::invalid_type;
  }
  int arg1Dest;
  ExpressionValueType_e argument1Type = expressionStore->parseForTrigger(
      &arg1Dest, triggerNum, slices[0], destinationID + 1);
  if (arg1Dest < 0) {
    return ExpressionValueType_e::invalid_type;
  }
  int arg2Dest;
  ExpressionValueType_e argument2Type = expressionStore->parseForTrigger(
      &arg2Dest, triggerNum, slices[2],
      expressionStore->getNextExpressionSpot(destinationID + 1));
  if (arg2Dest < 0) {
    return ExpressionValueType_e::invalid_type;
  }
  for (int i = 0; i < NUM_BINARY_FUNCTION; ++i) {
    if (binaryFunctionWrappers[i].matchesSlice(slices[1])) {
      if (binaryFunctionWrappers[i].acceptsArgument1Type(argument1Type) &&
          binaryFunctionWrappers[i].acceptsArgument2Type(argument2Type)) {
        *parseInto = BinaryFuncExpression(triggerNum, (BinaryFunction_e)i,
                                          arg1Dest, arg2Dest);
        return binaryFunctionWrappers[i].valueType;
      }
    }
  }
  return ExpressionValueType_e::invalid_type;
}
