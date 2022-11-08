#include "expression.h"

#include "filters.h"
#include "hardware_manager.h"

float get_variable(Variable var, FilterData_s *filterData) {
  switch (var) {
    case Variable::pos_z_agl:
      return filterData->pos_z_agl;
    case Variable::world_acc_z:
      return filterData->world_acc_z;
    case Variable::world_vel_z:
      return filterData->world_vel_z;
    default:
      return 0;
  }
}

Expression::Expression(Operation op) {
  this->type = ExpressionType::operation;
  this->contents.operation = op;
  switch (op.op) {
    case OperationType::and_o:
    case OperationType::or_o:
    case OperationType::not_o:
    case OperationType::leq_o:
    case OperationType::less_o:
    case OperationType::geq_o:
    case OperationType::greater_o:
    case OperationType::eq_o:
    case OperationType::after_o:
    case OperationType::for_o:
      this->value_type = ExpressionValueType::boolean;
      break;
    default:
      this->value_type = ExpressionValueType::num;
      break;
  }
  this->first_true = 0;
  this->true_since = 0;
}

Expression::Expression(float num) {
  this->type = ExpressionType::num;
  this->contents.num = num;
  this->value_type = ExpressionValueType::num;
  this->first_true = 0;
  this->true_since = 0;
}

Expression::Expression(double num) {
  this->type = ExpressionType::num;
  this->contents.num = (float)num;
  this->value_type = ExpressionValueType::num;
  this->first_true = 0;
  this->true_since = 0;
}

Expression::Expression(int32_t num) {
  this->type = ExpressionType::num;
  this->contents.num = (float)num;
  this->value_type = ExpressionValueType::num;
  this->first_true = 0;
  this->true_since = 0;
}

Expression::Expression(bool boolean) {
  this->type = ExpressionType::boolean;
  this->contents.boolean = boolean;
  this->value_type = ExpressionValueType::boolean;
  this->first_true = 0;
  this->true_since = 0;
}

Expression::Expression() {
  this->type = ExpressionType::boolean;
  this->contents.boolean = false;
  this->value_type = ExpressionValueType::boolean;
  this->first_true = 0;
  this->true_since = 0;
}

Expression::Expression(Variable var) {
  this->type = ExpressionType::variable;
  this->contents.variable = var;
  // all variables are numbers
  this->value_type = ExpressionValueType::num;
  this->first_true = 0;
  this->true_since = 0;
}

Expression::Expression(Event event) {
  this->type = ExpressionType::event;
  this->contents.event = event;
  // all variables are numbers
  this->value_type = ExpressionValueType::boolean;
  this->first_true = 0;
  this->true_since = 0;
}

void Expression::evaluate(ExprOperation funcs[], FilterData_s *filterData,
                          bool eventStatus[]) {
  switch (type) {
    case ExpressionType::boolean: {
      bool previously_true = contents.boolean;
      value.boolean = contents.boolean;
      if (contents.boolean && previously_true) {
        true_since = hm_millis();
      }
      if (contents.boolean && first_true == 0) {
        first_true = hm_millis();
      }
      return;
    }
    case ExpressionType::num:
      value.num = contents.num;
      return;
    case ExpressionType::operation: {
      ExpressionResult apply_result = funcs[(uint8_t)contents.operation.op](
          contents.operation.expr_id_1, contents.operation.expr_id_2);
      if (apply_result.type == value_type) {
        value = apply_result.value;
      }
      return;
    }
    case ExpressionType::variable:
      value =
          ExpressionValue{.num = get_variable(contents.variable, filterData)};
      return;
    case ExpressionType::event: {
      bool prev_status = value.boolean;
      bool status = eventStatus[(uint8_t)contents.event];
      value = ExpressionValue{.boolean = status};
      if (status && prev_status) {
        true_since = hm_millis();
      }
      if (status && first_true == 0) {
        first_true = 0;
      }
      return;
    }
    default:
      return;
  }
}
