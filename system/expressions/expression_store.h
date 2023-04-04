#ifndef SYSTEM_EXPRESSIONS_EXPRESSION_STORE_H_
#define SYSTEM_EXPRESSIONS_EXPRESSION_STORE_H_

#include <variant>

#include "binary_func_expression.h"
#include "const_expression.h"
#include "empty_expression.h"
#include "event_expression.h"
#include "expression.h"
#include "expression_sharedtypes.h"
#include "string_slice.h"
#include "unary_func_expression.h"
#include "var_expression.h"

/**
 * The possible expression types that could be stored in a given expression
 * "spot".
 */
typedef std::variant<EmptyExpression, ConstExpression, VarExpression,
                     EventExpression, UnaryFuncExpression, BinaryFuncExpression>
    ExpressionVariant_v;

/**
 * The ExpressionStore is responsible for holding and managing all of the
 * individual expressions used by the system. The expressions themselves work
 * independently from how they are stored, but because they need a common way of
 * being created, destroyed, and evaluated, the ExpressionStore class provides
 * an interface to interact with and use the expression functionality without
 * directly accessing expressions.
 */
class ExpressionStore {
 private:
  /**
   * An array of actual expressions. These are used for evaluating the current
   * triggers and for anything else that uses current expressions.
   */
  ExpressionVariant_v expressions[MAX_EXPRESSION];

  /**
   * A buffer of expressions. This is used when parsing to develop a new set of
   * expressions without changing the actual list in use.
   */
  ExpressionVariant_v expressionBuffer[MAX_EXPRESSION];

 public:
  /**
   * Determine which is the next available spot for an expression, starting the
   * search at a specified location.
   * @param startAt Index to start at
   * @return The index of the next available expression spot.
   */
  int getNextExpressionSpot(int startAt) const;

  /**
   * Construct an ExpressionStore.
   */
  ExpressionStore() = default;

  /**
   * Initialize an ExpressionStore. Fetches serialized configuration from stored
   * CLI configs and initializes expressions appropriately.
   */
  void init();

  /**
   * Gets a pointer to the specified expression.
   * @param expressions Expression variants to get pointers from.
   * @param expressionNum Index of the expression to get a pointer to.
   * @return Pointer to that expression.
   */
  Expression *getExpressionPtr(uint16_t expressionNum);

  /**
   * Remove all of the expressions for a certain trigger number.
   * @param triggerNum Trigger number to remove everything for.
   * Note that this only affects the buffer of expressions, and
   * writeNewConfigs() needs to be called to actually remove them.
   */
  void removeExpressionsForTrigger(int triggerNum);

  /**
   * Parse a given string slice as a new expression and add to the expression
   * store. Will also remove current expressions for that trigger.
   * @param resultID The resulting ID that the base of the expression gets.
   * @param triggerNum Trigger number to build for.
   * @param slice String slice to parse.
   * @param startAt The start location to potentially store this expression.
   * @return The value type of this expression, or invalid if it couldn't parse.
   */
  ExpressionValueType_e parseForTrigger(int *resultID, int triggerNum,
                                        const StringSlice &slice, int startAt);

  /**
   * Get the boolean status for a given expression.
   * @param expressionNum
   * @return Status of true or false for that expression.
   */
  bool getExprBoolValue(uint16_t expressionNum) const;

  /**
   * Get the numerical value for a given expression.
   * @param expressionNum
   * @return Numeric float value of that expression.
   */
  float getExprNumValue(uint16_t expressionNum) const;

  /**
   * Should be run once per cycle. Evaluates all expressions with updated data.
   * @param filterData
   */
  void tick(FilterData_s *filterData);

  /**
   * After a delete/update of triggers, actually write all of the changes to the
   * used values.
   */
  void writeNewConfigs();

  /**
   * Put an expression into a string.
   * @param expressionNum Expression number to format.
   * @param buffer Buffer to format into.
   * @param n Maximum size of buffer.
   */
  void conditionToString(uint16_t expressionNum, char *buffer, int n);
};
#endif  // SYSTEM_EXPRESSIONS_EXPRESSION_STORE_H_
