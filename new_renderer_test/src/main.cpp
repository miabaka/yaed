#define GLFW_INCLUDE_NONE

#include <cstdint>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

#include <GLFW/glfw3.h>
#include <GL/gl3w.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

namespace fs = std::filesystem;

struct __attribute__((packed)) InputTile {
	glm::u16vec2 position;
	uint16_t material;
};

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
	static const InputTile tiles[] = {
			{{0, 0}, 0},
			{{1, 1}, 0},
			{{2, 1}, 1},
			{{3, 2}, 0}
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tiles), tiles, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 1);
	glVertexAttribIPointer(0, 3, GL_UNSIGNED_SHORT, sizeof(InputTile), static_cast<void *>(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

	{
		glm::mat4 projection = glm::ortho(0.f, 20.f, 15.f, 0.f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjection"), 1, false, glm::value_ptr(projection));
	}

	GLuint vao = createVertexArray();
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 4);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}