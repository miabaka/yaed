#define GLFW_INCLUDE_NONE

#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

#include <GLFW/glfw3.h>
#include <GL/gl3w.h>

namespace fs = std::filesystem;

static void addShader(GLuint program, GLenum type, const char *source) {
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	glAttachShader(program, shader);

	glDeleteShader(shader);
}

static void addShaderFromFile(GLuint program, GLenum type, const fs::path &path) {
	std::ifstream file(path);

	std::stringstream source;
	source << file.rdbuf();

	if (!file.good())
		return;

	addShader(program, type, source.str().c_str());
}

static void linkShaderProgram(GLuint program) {
	glLinkProgram(program);

	GLint logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength < 1)
		return;

	std::string log(logLength, '\0');
	glGetProgramInfoLog(program, logLength, nullptr, log.data());

	std::cout << "- - Shader program output - -\n" << log << std::endl;
}

static GLuint createShaderProgram() {
	GLuint program = glCreateProgram();

	addShaderFromFile(program, GL_FRAGMENT_SHADER, "data/shaders/main.fs.glsl");
	addShaderFromFile(program, GL_VERTEX_SHADER, "data/shaders/main.vs.glsl");

	linkShaderProgram(program);

	return program;
}

static GLuint createVertexArray() {
	GLuint vao;
	glGenVertexArrays(1, &vao);

	return vao;
}

int main() {
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, false);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

	GLFWwindow *window = glfwCreateWindow(800, 600, "Ayaya! Ayaya!", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	gl3wInit();

	GLuint shaderProgram = createShaderProgram();
	glUseProgram(shaderProgram);

	GLuint dummyVao = createVertexArray();
	glBindVertexArray(dummyVao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}