#include "expression.h"

#include "hardware_manager.h"

Expression::Expression() {
  this->trueSince = -1;
  this->firstTrue = -1;
  this->setBooleanValue(false);
}

Expression::~Expression() {}

bool Expression::isEmpty() const { return false; }

bool Expression::getBooleanValue() const {
  return (this->valueType == boolean) && this->value.boolean;
}

float Expression::getNumberValue() const {
  return (this->valueType == number) ? this->value.number : 0;
}

void Expression::setNumberValue(float num) {
  this->valueType = number;
  this->value.number = num;
}

void Expression::setBooleanValue(bool bl) {
  this->valueType = boolean;
  // This conditional represents that the only important case is
  // when we currently are true and the next value is true.
  // in this case, we do not want to change the trueSince value
  // because we continue to be true.
  // In any other case, we either are not true now in which case "true" for 0ms
  // or we are becoming true for the first time.
  if (this->trueSince == (uint32_t)-1 || (!this->value.boolean && bl) || !bl) {
    this->trueSince = hm_millis();
  }
  this->value.boolean = bl;
  // If this is the first time we've been true. Otherwise it should not update
  if (bl && this->firstTrue == (uint32_t)-1) {
    this->firstTrue = hm_millis();
  }
}

void Expression::setTriggerNum(uint16_t triggerNum) {
  this->triggerNum = triggerNum;
}
