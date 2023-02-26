#ifndef SYSTEM_EXPRESSIONS_EXPRESSION_H_
#define SYSTEM_EXPRESSIONS_EXPRESSION_H_

#include <inttypes.h>

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
   * @param expressions Array of expression pointers, for expressions that
   * rely on the value of other expressions.
   */
  virtual void evaluate(FilterData_s *filterData,
                        Expression *expressions[]) = 0;

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
   * Set the value of this expression to be a numeric value.
   * @param value
   */
  void setNumberValue(float num);

  /**
   * Set the value of this expression to be a boolean.
   * @param value
   */
  void setBooleanValue(bool bl);

  /**
   * Set the trigger number that this expression is used for.
   * @param triggerNum
   */
  void setTriggerNum(int triggerNum);

  /**
   * Put a string representation of this expression into a buffer.
   * @param buffer Buffer to print into.
   * @param n Size of buffer available
   * @param expressions Other expressions.
   * @return Number of characters used by this expression.
   */
  virtual int toString(char *buffer, int n,
                       Expression *expressions[]) const = 0;

  /**
   * Serialize this expression into a location.
   * @param serialized Struct to serialize into.
   */
  virtual void serializeInto(SerializedExpression_s *serialized) const = 0;
};

#endif  // SYSTEM_EXPRESSIONS_EXPRESSION_H_