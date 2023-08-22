#include "Application.h"	
#include "FrameUpdate.h"
#include <fmt/format.h>

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

struct GlobalContext {
	GLFWwindow* window;
	int fontScale = 2; 
	int userScale = 2;
	std::vector<ImFont*> fonts;
	bool reloadFonts;
};
static GlobalContext gContext;

// From https://github.com/wpilibsuite/allwpilib/blob/89e738262c7dd80daa3702432250e49fa57d7f55/wpigui/src/main/native/cpp/wpigui.cpp#L152C1-L172C2
static void UpdateFontScale() {
  // Scale based on OS window content scaling
  float windowScale = 1.0;
#ifndef __APPLE__
  glfwGetWindowContentScale(gContext.window, &windowScale, nullptr);
#endif
  // map to closest font size: 0 = 0.5x, 1 = 0.75x, 2 = 1.0x, 3 = 1.25x,
  // 4 = 1.5x, 5 = 1.75x, 6 = 2x
  int fontScale =
      gContext.userScale + static_cast<int>((windowScale - 1.0) * 4);
  if (fontScale < 0) {
    fontScale = 0;
  }
  if (gContext.fontScale != fontScale) {
    gContext.reloadFonts = true;
    gContext.fontScale = fontScale;
  }
}
// the range is based on 13px being the "nominal" 100% size and going from
// ~0.5x (7px) to ~2.0x (25px)
static void ReloadFonts() {
  auto& io = ImGui::GetIO();
  io.Fonts->Clear();
  gContext.fonts.clear();
  float size = 7.0f + gContext.fontScale * 3.0f;
  bool first = true;
  for (auto&& makeFont : gContext.makeFonts) {
    if (makeFont.second) {
      ImFontConfig cfg;
      std::snprintf(cfg.Name, sizeof(cfg.Name), "%s", makeFont.first);
      ImFont* font = makeFont.second(io, size, &cfg);
      if (first) {
        ImGui::GetIO().FontDefault = font;
        first = false;
      }
      gContext.fonts.emplace_back(font);
    }
  }
}

void Application::tickApplication() {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
			if (ImGui::BeginMenu("Zoom")) {

				for (int i = 50; i<=300; i+=50) {
					if (ImGui::MenuItem(fmt::format("{}%", i).c_str())) {
						// ImGui::
						// ImGui::GetStyle().ScaleAllSizes(i / 100.0);
						// ReloadFonts();
					}
				}
				
            ImGui::EndMenu();
			}
            ImGui::EndMenu();
        }


        ImGui::EndMainMenuBar();
    }

	// timestamp maybe
	FrameUpdate frame_update;

	for (auto& w: widgets) {
		w->update_widget(frame_update);
		w->display();
	}
}

void Application::addWidget(std::unique_ptr<Widget>& w) {
	widgets.push_back(std::move(w));
}

// a lot of this is boilerplate, nothing *too* interesting here
// only change this if you want to change fonts, scaling, or some config stuff
void Application::runGUI() {
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return;

		// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
		// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context

	gContext.window = glfwCreateWindow(1280, 720, "FCB Sim GUI", NULL, NULL);
	if (window == NULL)
		return;
	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef GL_MULTISAMPLE
	glEnable(GL_MULTISAMPLE);
#endif
	glfwMakeContextCurrent(gContext.window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImPlot::CreateContext();

	// // font sizing
	// // https://github.com/ocornut/imgui/blob/master/docs/FONTS.md see this link to change the font
	// float font_scale = 1.3;
	// ImFontConfig cfg;
	// cfg.SizePixels = 13 * font_scale;
	// cfg.GlyphOffset.y = font_scale;
	// ImGui::GetIO().Fonts->AddFontDefault(&cfg);

	// Load Fonts
	UpdateFontScale();
	ReloadFonts();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		tickApplication();


		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.4f, 0.6f, 0.8f, 1.0f); // background color - can change this if wanted
		glClear(GL_COLOR_BUFFER_BIT); 
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		// usleep(100); using 2 buffers automatically vsyncs so the delay isnt actually necessary, will delete this later
	}	

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();

	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();


}

void Application::run() {
	this->runGUI();
}

Application::~Application() {
	// end the socket thread
	// destruct = true;
	// socket->shutdown();
	// socket_thread.join();
}


