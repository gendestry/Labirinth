#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>

class Player {
private:
	btRigidBody* body;
	glm::vec3 rotation;

	void move(glm::vec3 pos);
	void rotate(glm::vec3 rot);

public:
	Player(glm::vec3 pos, btDiscreteDynamicsWorld* wld);

	void update(GLFWwindow* window);

	glm::mat4 getViewMatrix() const;
	glm::vec3 getPosition() const;
	glm::vec3 getRotation() const;
	glm::vec3 getViewDir() const;
};