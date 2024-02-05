#ifndef COMMON_SYSTEM_EXPRESSIONS_EXPRESSION_BUILDER_H_
#define COMMON_SYSTEM_EXPRESSIONS_EXPRESSION_BUILDER_H_

#include "binary_func_expression.h"
#include "const_expression.h"
#include "event_expression.h"
#include "expression_store.h"
#include "small_strtod.h"
#include "unary_func_expression.h"
#include "var_expression.h"

// The maximum number of slices to split a string into. The maximum for a valid
// expression is 3 (for a binary expression), so 4+ will always be
// invalid.
#define MAX_SLICE 4

/**
 * An expression builder is a general class of objects that can parse
 * strings into expressions. This is used as an interface between the user (via
 * CLI) and the expression store so that neither has to interact directly.
 */
class ExpressionBuilder {
 public:
  /**
   * Build an expressionbuilder
   * @param slice The string slice to parse.
   * @param expressionStore The expression store to build into
   * @param triggerNum The trigger number to parse for
   * @param parseInto The location to parse this expression into
   * @param destinationID The index in the expressionstore that this will go
   * into.
   * @return The value type built by the builder, which may be invalid.
   */
  virtual ExpressionValueType_e build(const StringSlice& slice,
                                      ExpressionStore* expressionStore,
                                      uint16_t triggerNum,
                                      ExpressionVariant_v* parseInto,
                                      uint16_t destinationID) = 0;

  /**
   * Destructor
   */
  virtual ~ExpressionBuilder() = default;
};

/**
 * Variable expression builder. This builder attempts to parse a string
 * as a variable name and create a VarExpression for that variable, if a match
 * is found.
 */
class VarExpressionBuilder : public ExpressionBuilder {
 public:
  ExpressionValueType_e build(const StringSlice& slice,
                              ExpressionStore* expressionStore,
                              uint16_t triggerNum,
                              ExpressionVariant_v* parseInto,
                              uint16_t destinationID) override;
};

/**
 * Event expression builder. This builder attempts to parse a string as
 * the name of an event and create an EventExpression for that event, if a match
 * is found.
 */
class EventExpressionBuilder : public ExpressionBuilder {
 public:
  ExpressionValueType_e build(const StringSlice& slice,
                              ExpressionStore* expressionStore,
                              uint16_t triggerNum,
                              ExpressionVariant_v* parseInto,
                              uint16_t destinationID) override;
};

/**
 * Constant expression builder. This builder attempts to parse a string as
 * a float or integer and create a ConstExpression from that value, if a valid
 * constant is found.
 */
class ConstExpressionBuilder : public ExpressionBuilder {
 public:
  ExpressionValueType_e build(const StringSlice& slice,
                              ExpressionStore* expressionStore,
                              uint16_t triggerNum,
                              ExpressionVariant_v* parseInto,
                              uint16_t destinationID) override;
};

/**
 * Unary Function expression builder. This attempts to parse a string
 * by checking if it is wrapped in (), verifying that there are 2 separate
 * arguments in the parentheses, checking if the first value is a valid unary
 * function name, and then checking if the second value is both a valid separate
 * expression and if the type of value it holds (boolean or number) is a valid
 * input to the function already determined.
 */
class UnaryFuncExpressionBuilder : public ExpressionBuilder {
 public:
  ExpressionValueType_e build(const StringSlice& slice,
                              ExpressionStore* expressionStore,
                              uint16_t triggerNum,
                              ExpressionVariant_v* parseInto,
                              uint16_t destinationID) override;
};

/**
 * Binary Function expression builder. This attempts to parse a string
 * by checking if it is wrapped in (), verifying that there are 3 separate
 * arguments in the parentheses, checking if the middle value is a valid
 * binary function name, and then checking if the first and third values are
 * both valid separate expressions and if the types of values they hold
 * (boolean or number) are valid inputs to the function already determined.
 */
class BinaryFuncExpressionBuilder : public ExpressionBuilder {
 public:
  ExpressionValueType_e build(const StringSlice& slice,
                              ExpressionStore* expressionStore,
                              uint16_t triggerNum,
                              ExpressionVariant_v* parseInto,
                              uint16_t destinationID) override;
};

#endif  // COMMON_SYSTEM_EXPRESSIONS_EXPRESSION_BUILDER_H_
