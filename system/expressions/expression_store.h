//
// Created by sam on 2/14/23.
//

#ifndef CTEST_EXPRESSION_STORE_H
#define CTEST_EXPRESSION_STORE_H


#include "expression_sharedtypes.h"
#include "expression.h"
#include "const_expression.h"
#include "var_expression.h"
#include "unary_func_expression.h"
#include "binary_func_expression.h"
#include "empty_expression.h"
#include "event_expression.h"
#include "string_slice.h"
#include <variant>


typedef std::variant<
        EmptyExpression,
        ConstExpression,
        VarExpression,
		EventExpression,
        UnaryFuncExpression,
        BinaryFuncExpression> ExpressionVariant_v;

class ExpressionStore {
private:
    /**
     * An array of actual expressions.
     */
    ExpressionVariant_v expressions[MAX_EXPRESSION];

    /**
     * Expression pointers. These need to be set to refer to expressions as the general Expression
     * base class.
     */
    Expression *expressionPtrs[MAX_EXPRESSION];

    /**
     * A buffer of expressions. This is used when parsing to develop a new set of expressions
     * without changing the actual list in use.
     */
    ExpressionVariant_v expressionBuffer[MAX_EXPRESSION];

public:
    /**
     * Determine which is the next available spot for an expression, starting the search
     * at a specified location.
     * @param startAt Index to start at
     * @return The index of the next available expression spot.
     */
    int getNextExpressionSpot(int startAt);

    /**
     * Construct an ExpressionStore.
     */
    ExpressionStore();

    /**
     * Remove all of the expressions for a certain trigger number.
     * @param triggerNum Trigger number to remove everything for.
     * Note that this only affects the buffer of expressions, and
     * writeNewConfigs() needs to be called to actually remove them.
     */
    void removeExpressionsForTrigger(int triggerNum);

    /**
     * Parse a given string slice as a new expression.
     * @param resultID The resulting ID that the base of the expression gets.
     * @param triggerNum Trigger number to build for.
     * @param slice String slice to parse.
     * @param startAt The start location to potentially store this expression.
     * @return The value type of this expression, or invalid if it couldn't parse.
     */
    ExpressionValueType_e parseForTrigger(int &resultID, int triggerNum, StringSlice &slice, int startAt);

    /**
     * Get the boolean status for a given trigger.
     * @param triggerNum
     * @return Status of true or false for that expression.
     */
    bool getStatusFor(int triggerNum);

    /**
     * Should be run once per cycle. Evaluates all expressions with updated data.
     * @param filterData
     */
    void tick(FilterData_s *filterData);

    /**
     * After a delete/update of triggers, actually write all of the changes to the used values.
     */
    void writeNewConfigs();

    /**
     * Put a trigger's configuration into a string.
     * @param triggerNum trigger number to format.
     * @param buffer Buffer to format into.
     * @param n Maximum size of buffer.
     */
    void conditionToString(int triggerNum, char *buffer, int n);
};


#endif //CTEST_EXPRESSION_STORE_H
