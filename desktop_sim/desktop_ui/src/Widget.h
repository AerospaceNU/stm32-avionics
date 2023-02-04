#ifndef WATCHDOG_WIDGET
#define WATCHDOG_WIDGET


/*
 * Widget interface that every box needs to implement
 * To create a new widget, create a class that uses this interface and define a display() and update_widget(data)
 * Both are called each frame, display() is meant to display the widget, and update_widget(data) is meant to update a widget
 * based off of one json of data.
*/
#include <string>
#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"


class Widget {
public:
	Widget(std::string n) : widget_name(n) {};
	virtual ~Widget() = default;

	// display()
	virtual void display() const = 0; 
	// update_widget(data updates)
	virtual void update_widget() = 0;
protected:
	std::string widget_name;
};


#endif // WATCHDOG_WIDGET
