#ifndef WATCHDOG_SensorData_BOX
#define WATCHDOG_SensorData_BOX

/*
 * Widget to display SensorDatas.
 */

#include <string>
#include <vector>

#include "Widget.h"


class SensorDataBox : public Widget {
public:
	SensorDataBox() : Widget("SensorDataBox") {};
	void display() const override;
	void update_widget(const FrameUpdate&) override;

private:
	FrameUpdate state;
};

#endif // WATCHDOG_SensorData_BOX
