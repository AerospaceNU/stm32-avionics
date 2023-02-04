#ifndef WATCHDOG_GUI_MISSIONCONTROL
#define WATCHDOG_GUI_MISSIONCONTROL

/*
 * Main program. Hosts the GLFW rendering, Networking, and essentially everything
 * Actual definitions are in main.cpp
 */

#include <algorithm>
#include <iostream>
#include <memory>
#include <thread>

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>


#include "imgui.h"
// #include "imgui/imgui_demo.cpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"

#include "Widget.h"
#include <vector>


class Application {
public:
	Application() = default;
	~Application();
	void tickApplication();
	void run();
	void addWidget(std::unique_ptr<Widget>&);
private:
	void runGUI();
	
	std::vector<std::unique_ptr<Widget> > widgets;
	std::thread socket_thread;
	bool destruct = false;
};

#endif // WATCHDOG_GUI_MISSIONCONTROL
