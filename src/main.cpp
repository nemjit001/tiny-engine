#include <cstdint>

#include <GLFW/glfw3.h>

static constexpr char const* WindowTitle = "Tiny Renderer";
static constexpr uint32_t const WindowWidth = 1600;
static constexpr uint32_t const WindowHeight = 900;

int main()
{
	if (glfwInit() == GLFW_FALSE) {
		return 1;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	GLFWwindow* pWindow = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, nullptr, nullptr);
	if (pWindow == nullptr) {
		return 1;
	}

	while (!glfwWindowShouldClose(pWindow))
	{
		glfwPollEvents();

		//
	}

	glfwDestroyWindow(pWindow);
	glfwTerminate();
	return 0;
};
