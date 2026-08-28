#include <cstdlib>          // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>
#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

static const char* kWindowTitle     = "OpenGL template project";
static constexpr int kWindowWidth   = 800;
static constexpr int kWindowHeight  = 600;
static constexpr int kGLVerMajor    = 4;
static constexpr int kGLVerMinor    = 5;

static int glfw_error_code{};
static std::string glfw_error_str{};

static void error_callback(int error, const char *description);
static void framebuffer_size_callback(GLFWwindow* window,
                                      int width, int height);
static void processInput(GLFWwindow *window);
static void print_gl_version(void);

int main()
{
	 // Set error callback before initialization so we can be notified
	glfwSetErrorCallback(error_callback);
	
	// Init GLFW
	if (!glfwInit()) {
		const std::string error_msg = "GLFW initialization failed!";
        const std::string glfw_error_msg = std::to_string(glfw_error_code) +
                                           "): " + glfw_error_str;
		std::cout << error_msg + " - GLFW(" + glfw_error_msg << std::endl;
		return EXIT_FAILURE;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kGLVerMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kGLVerMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(kWindowWidth,
					      kWindowHeight,
					      kWindowTitle, nullptr, nullptr);
	
	if (window == nullptr){
		// Clean resources already created
		glfwTerminate();
		
		const std::string error_msg = "GLFW window creation failed!";
        const std::string glfw_error_msg = std::to_string(glfw_error_code) +
                                           "): " + glfw_error_str;
		std::cout << error_msg + " - GLFW(" + glfw_error_msg << std::endl;
		
		return EXIT_FAILURE;
	}
	
	glfwMakeContextCurrent(window);
	
	// Load GLAD pointers in the current GLFW context
	if (!gladLoadGL(glfwGetProcAddress)) {
		// Clean resources already created
		glfwDestroyWindow(window);
		glfwTerminate();
		
		std::cout << "GLAD initialization failed!" << std::endl;
		
		return EXIT_FAILURE;
	}

	// Show current system OpenGL info
	print_gl_version();

	// Set GLFW callbacks
	//glViewport(0, 0, kWindowWidth, kWindowHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Configure sync with monitor
    // interval: the number of screen updates to wait from the time
    // glfwSwapBuffers was called before swapping the buffers and returning.
	// 0 -> run at code speed (don't sync to monitor)
	// 1 -> sync with motinor refresh rate (code run at monitor refresh rate)
    // 2 -> sync with half monitor refresh rate (code run at half monitor
    //      refresh rate)
	glfwSwapInterval(1);
	
	while(!glfwWindowShouldClose(window)){
		// input
		processInput(window);
		
		// rendering commands here
		glClearColor(51.0f/256, 55.0f/256, 76.0f/256, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	// Clean resources before close
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return EXIT_SUCCESS;
}

void error_callback(int error, const char *description){
    glfw_error_code = error;
    glfw_error_str = std::string(description);
}

void framebuffer_size_callback([[maybe_unused]]  GLFWwindow* window,
                               int width, int height){
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
}

void print_gl_version(void){
    // show gl info
    std::cout << " OpenGL Vendor: "
              << glGetString(GL_VENDOR) << std::endl;
    std::cout << " OpenGL Renderer: "
              << glGetString(GL_RENDERER) << std::endl;
    std::cout << " OpenGL Version: "
              << glGetString(GL_VERSION) << std::endl;
    std::cout << " GLSL Version: "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
