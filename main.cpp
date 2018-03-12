#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Wall.h"
#include "Player.h"
#include "Level2D.h"

const int SCR_WIDTH = 800, SCR_HEIGHT = 600;

int main() {
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Labirinth", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);

	if (glewInit() != 0)
		return -1;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.1, 0.3, 0.8, 1.0);

	Level2D::loadFromFile("levels/level0.lvl");
	std::vector<Wall> walls = Level2D::getWalls();

	Shader wallShader("wallShader.vert", "wallShader.frag");
	Player player(glm::vec3(Level2D::getStartPos().x / 2, Level2D::getStartPos().y / 2, Level2D::getStartPos().z / 2), walls);

	//std::cout << Level2D::getStartPos();

	wallShader.use();
	wallShader.setMat4("proj", glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 500.0f)); // 20.0f / 45.0f
	wallShader.setMat4("view", player.getViewMatrix());
	wallShader.setMat4("model", glm::mat4(1.0));

	Wall::bind();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		player.update(window);
		wallShader.setMat4("view", player.getViewMatrix());

		for (int i = 0; i < walls.size(); i++) {
			wallShader.setMat4("model", walls[i].getModelMatrix());
			wallShader.setInt("tex", walls[i].type);
			Wall::render();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Wall::cleanup();
	glfwTerminate();
	return 0;
}