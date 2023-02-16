//
// Created by sam on 2/15/23.
//

#ifndef CTEST_EXPRESSION_BUILDER_H
#define CTEST_EXPRESSION_BUILDER_H

#include "expression_store.h"
#include "var_expression.h"
#include "const_expression.h"
#include "unary_func_expression.h"
#include "binary_func_expression.h"
#include "event_expression.h"
#include "small_strtod.h"

#define MAX_SLICE 4

/**
 * An expression builder is a general class of objects that can parse
 * strings into expressions.
 */
class ExpressionBuilder {
public:
    /**
     * Build an expressionbuilder
     * @param slice The string slice to parse.
     * @param expressionStore The expression store to build into
     * @param triggerNum The trigger number to parse for
     * @param parseInto The location to parse this expression into
     * @param destinationID The index in the expressionstore that this will go into.
     * @return The value type built by the builder, which may be invalid.
     */
    virtual ExpressionValueType_e build(StringSlice &slice, ExpressionStore *expressionStore,
                                        int triggerNum, ExpressionVariant_v *parseInto, int destinationID) = 0;
};

/**
 * Variable expression builder.
 */
class VarExpressionBuilder : public ExpressionBuilder {
public:
    ExpressionValueType_e
    build(StringSlice &slice, ExpressionStore *expressionStore, int triggerNum, ExpressionVariant_v *parseInto,
          int destinationID) {
        FilterData_e variableType = getVariableEnumFromString((char *)slice.startPtr());
        if (variableType != FilterData_e::invalid) {
            *parseInto = VarExpression(triggerNum, variableType);
            return ExpressionValueType_e::number;
        }
        return ExpressionValueType_e::invalid_type;
    }
};

/**
 * Variable expression builder.
 */
class EventExpressionBuilder : public ExpressionBuilder {
public:
    ExpressionValueType_e
    build(StringSlice &slice, ExpressionStore *expressionStore, int triggerNum, ExpressionVariant_v *parseInto,
          int destinationID) {
    	for (int i = 0; i < Event_e::NUM_EVENT; ++i) {
    		if (slice == eventStrings[i]) {
    			*parseInto = EventExpression(triggerNum, (Event_e)i);
    			return ExpressionValueType_e::boolean;
    		}
    	}
        return ExpressionValueType_e::invalid_type;
    }
};


/**
 * Constant expression builder.
 */
class ConstExpressionBuilder : public ExpressionBuilder {
public:
    ExpressionValueType_e
    build(StringSlice &slice, ExpressionStore *expressionStore, int triggerNum, ExpressionVariant_v *parseInto,
          int destinationID) {
        static char *endptr;
        float value = smallStrtod((char *) slice.startPtr(), &endptr);

        if (endptr != slice.endptr()) {
            return ExpressionValueType_e::invalid_type;
        }
        *parseInto = ConstExpression(triggerNum, value);
        return ExpressionValueType_e::number;
    }
};

/**
 * Unary Function expression builder.
 */
class UnaryFuncExpressionBuilder : public ExpressionBuilder {
public:
    ExpressionValueType_e
    build(StringSlice &slice, ExpressionStore *expressionStore, int triggerNum, ExpressionVariant_v *parseInto,
          int destinationID) {
        if (slice[0] != '(' || slice[slice.length() - 1] != ')') {
            return ExpressionValueType_e::invalid_type;
        }
        StringSlice slices[MAX_SLICE];

        StringSlice innerSlice = slice.inner();
        int argCount = StringSlice::split(innerSlice, slices, MAX_SLICE, ' ');
        if (argCount != 2) {
            return ExpressionValueType_e::invalid_type;
        }
        int argDest;
        ExpressionValueType_e argumentType = expressionStore->parseForTrigger(argDest, triggerNum, slices[1],
                                                                              destinationID + 1);
        if (argDest < 0) {
            return ExpressionValueType_e::invalid_type;
        }
        for (int i = 0; i < NUM_UNARY_FUNCTION; ++i) {
            if (unaryFunctionWrappers[i].matchesSlice(slices[0])) {
                if (unaryFunctionWrappers[i].acceptsArgumentType(argumentType)) {
                    *parseInto = UnaryFuncExpression(triggerNum, (UnaryFunction_e) i, argDest);
                    return unaryFunctionWrappers[i].valueType;
                }
            }
        }
        return ExpressionValueType_e::invalid_type;
    }
};

/**
 * Binary Function expression builder.
 */
class BinaryFuncExpressionBuilder : public ExpressionBuilder {
public:
    ExpressionValueType_e
    build(StringSlice &slice, ExpressionStore *expressionStore, int triggerNum, ExpressionVariant_v *parseInto,
          int destinationID) {
        StringSlice slices[MAX_SLICE];
        if (slice[0] != '(' || slice[slice.length() - 1] != ')') {
            return ExpressionValueType_e::invalid_type;
        }
        StringSlice innerSlice = slice.inner();
        int argCount = StringSlice::split(innerSlice, slices, MAX_SLICE, ' ');
        if (argCount != 3) {
            return ExpressionValueType_e::invalid_type;
        }
        int arg1Dest;
        ExpressionValueType_e argument1Type = expressionStore->parseForTrigger(arg1Dest, triggerNum, slices[0],
                                                                               destinationID + 1);
        if (arg1Dest < 0) {
            return ExpressionValueType_e::invalid_type;
        }
        int arg2Dest;
        ExpressionValueType_e argument2Type = expressionStore->parseForTrigger(arg2Dest, triggerNum, slices[2],
                                                                               expressionStore->getNextExpressionSpot(
                                                                                       destinationID + 1));
        if (arg2Dest < 0) {
            return ExpressionValueType_e::invalid_type;
        }
        for (int i = 0; i < NUM_BINARY_FUNCTION; ++i) {
            if (binaryFunctionWrappers[i].matchesSlice(slices[1])) {
                if (binaryFunctionWrappers[i].acceptsArgument1Type(argument1Type) &&
                    binaryFunctionWrappers[i].acceptsArgument2Type(argument2Type)) {
                    *parseInto = BinaryFuncExpression(triggerNum, (BinaryFunction_e) i, arg1Dest, arg2Dest);
                    return binaryFunctionWrappers[i].valueType;
                }
            }
        }
        return ExpressionValueType_e::invalid_type;
    }
};

#endif //CTEST_EXPRESSION_BUILDER_H
