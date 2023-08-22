#include "Application.h"
#include "widgets/SensorDataBox.h"
#include "widgets/Widget.h"
#include <memory>

int main(int argC, char** argv) {
  Application app;
	std::unique_ptr<Widget> rb = std::make_unique<SensorDataBox>();
  app.addWidget(rb);
  app.run();
}
