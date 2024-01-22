#include "sensor.h"

class BadBarometer : public Barometer {
 public:
  BadBarometer(const int countLength)
      : m_count{0}, m_countLength{countLength} {}

  const SensorStatus getStatus() const override { return SensorStatus::ONLINE; }

  UpdateStatus update(DataType& dataOut) override {
    if (m_count++ < m_countLength) {
      return UpdateStatus::NO_CHANGE;
    } else {
      dataOut = BarometerData_s{38.3, 1.01};
      m_count = 0;
      return UpdateStatus::NEW_DATA;
    }
  }

 private:
  int m_count;
  const int m_countLength;
};