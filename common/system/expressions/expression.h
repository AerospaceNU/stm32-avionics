#ifndef COMMON_SYSTEM_EXPRESSIONS_EXPRESSION_H_
#define COMMON_SYSTEM_EXPRESSIONS_EXPRESSION_H_

#include <inttypes.h>

#include <functional>

#include "expression_sharedtypes.h"
#include "filterdata_types.h"
#include "string_slice.h"

typedef enum { invalid_type, boolean, number } ExpressionValueType_e;

typedef union {
  bool boolean;
  float number;
} ExpressionValue_u;

/**
 * Abstract base class for all configuration expressions. Additional expression
 * types should extend this class.
 */
class Expression {
 private:
  ExpressionValueType_e valueType;
  ExpressionValue_u value;

 public:
  /**
   * An ExpressionPtrCallback is a callback function that allows an expression
   * pointer to be obtained given the index of the expression to find.
   * @param expressionNum The index/number of the expression in the
   * ExpressionStore to fetch from.
   * @return Pointer to that expression as the base class Expression.
   */
  using ExpressionPtrCallback = std::function<Expression*(uint16_t)>;
  uint32_t firstTrue;
  uint32_t trueSince;
  uint16_t triggerNum;

  /**
   * Default constructor. Initializes some good base values.
   */
  Expression();

  /**
   * Destructor.
   */
  virtual ~Expression();

  /**
   * Evaluate this expression.
   * @param filterData Filter data to evaluate with.
   * @param expressionPtrCallback ExpressionPtrCallback to get a pointer to a
   * given child expression.
   */
  virtual void evaluate(FilterData_s* filterData,
                        ExpressionPtrCallback& expressionPtrCallback) = 0;

  /**
   * If this is an empty expression.
   * @return bool for empty or not.
   */
  virtual bool isEmpty() const;

  /**
   * Get the boolean value of an expression.
   * @return The boolean value. This will be false if the value is not actually
   * a boolean.
   */
  bool getBooleanValue() const;

  /**
   * Get the number value of an expression.
   * @return The number value. This will be 0 if the value is not actually a
   * number.
   */
  float getNumberValue() const;

  /**
   * Set the value of this expression to be a numeric value. Will also mark that
   * this expression contains a numeric value.
   * @param value Float value to set to.
   */
  void setNumberValue(float num);

  /**
   * Set the value of this expression to be a boolean. Will also mark that this
   * expression contains a boolean value.
   * @param value Boolean value to set to.
   */
  void setBooleanValue(bool bl);

  /**
   * Set the trigger number that this expression is used for.
   * @param triggerNum Trigger number to associate with this expression.
   */
  void setTriggerNum(uint16_t triggerNum);

  /**
   * Put a string representation of this expression into a buffer.
   * @param buffer Buffer to print into.
   * @param n Size of buffer available
   * @param expressionPtrCallback ExpressionPtrCallback to get a pointer to a
   * given child expression.
   * @return Number of characters used by this expression in the buffer.
   */
  virtual int toString(char* buffer, int n,
                       ExpressionPtrCallback& expressionPtrCallback) const = 0;

  /**
   * Serialize this expression into a location.
   * @param serialized Struct to serialize into.
   */
  virtual void serializeInto(SerializedExpression_s* serialized) const = 0;
};

#endif  // COMMON_SYSTEM_EXPRESSIONS_EXPRESSION_H_
