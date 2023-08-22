#include "Application.h"	
#include "FrameUpdate.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void Application::tickApplication() {
	// std::vector<nlohmann::json> data_frame = socket->dump_messages();

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

	GLFWwindow* window = glfwCreateWindow(1280, 720, "FCB Sim GUI", NULL, NULL);
	if (window == NULL)
		return;
	glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef GL_MULTISAMPLE
	glEnable(GL_MULTISAMPLE);
#endif
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImPlot::CreateContext();

	// // font sizing
	// // https://github.com/ocornut/imgui/blob/master/docs/FONTS.md see this link to change the font
	// ImGuiIO& io = ImGui::GetIO(); (void)io;

	// float font_scale = 1.3;
	// ImFontConfig cfg;
	// cfg.SizePixels = 13 * font_scale;
	// cfg.GlyphOffset.y = font_scale;
	// ImGui::GetIO().Fonts->AddFontDefault(&cfg);

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

// // runs forever until destructor is called
// // keeps trying to reconnect
// void Application::runNetworker() {
// 	while (true) {
// 		std::cout << "Trying to reconnect to " + hdl << std::endl;
// 		// didn't work without making a new one not sure what's the deal here but this shouldn't memory leak 
// 		// pray
// 		socket = std::make_unique<NetworkClient>();
// 		try {
// 			socket->run(hdl);
// 		} catch (...) {
// 			// do nothing because this is fine and shouldn't happen either?
// 		}
// 		// // IF WE GET HERE THE SOCKET IS CLOSED
// 		if (destruct) break;
// 		usleep(1000000); // 1s delay
// 	}

// }

void Application::run() {

	// socket_thread = std::thread(&Application::runNetworker, this);

	this->runGUI();
}

Application::~Application() {
	// end the socket thread
	// destruct = true;
	// socket->shutdown();
	// socket_thread.join();
}


