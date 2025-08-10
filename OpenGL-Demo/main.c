#include <GLFW/glfw3.h>

int main(int argc, char** argv) {
	if(!glfwInit()) {
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(600, 400, "Hello, world!", NULL, NULL);

	if(!window) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
