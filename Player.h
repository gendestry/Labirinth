#pragma once
#include "Wall.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

class Player {
private:
	bool falling;

	glm::vec3 position, rotation, velocity;
	glm::mat4 view;

	const std::vector<Wall> &walls;

	void updatePosition();
	void updateRotation(glm::vec3 rot);
	void updateMatrix();
	void collision(glm::vec3 &trans);

	friend std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);
public:
	Player(glm::vec3 pos, const std::vector<Wall> &walls);

	void update(GLFWwindow* window);
	void move(GLFWwindow* window);

	inline glm::mat4 getViewMatrix() const { return view; }
	inline glm::vec3 getPosition() const { return position; }
};