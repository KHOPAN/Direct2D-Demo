#define GLEW_STATIC
#include <stdio.h>
#include <GL/glew.h>
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

	if(glewInit() != GLEW_OK) {
		return 1;
	}

	printf("OpenGL: %s\n", glGetString(GL_VERSION));

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
