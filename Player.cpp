#include "Player.h"
#include "Wall.h"

#include <iostream>

bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2) {
	Wall* wall = (Wall*)obj1->getCollisionObject()->getUserPointer();
	if (wall->type == Wall::FINISH) {
		static bool first = true;
		if (first) {
			std::cout << "Level completed succesfully!\n";
			first = false;
		}
	}
	return false;
}

Player::Player(glm::vec3 pos, btDiscreteDynamicsWorld* world) : rotation(glm::vec3(0.0f)) {
	float mass = 1.0f;

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	btBoxShape* shape = new btBoxShape(btVector3(0.05f, 0.245f, 0.05f));
	// btCapsuleShape* shape = new btCapsuleShape(0.05f, 0.40f);
	btMotionState* motion = new btDefaultMotionState(trans);
	btVector3 inertia(0, 0, 0); shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, shape, inertia);
	body = new btRigidBody(info);
	body->setAngularFactor(btVector3(0, 0, 0));
	body->setSleepingThresholds(0, 1);
	body->setRestitution(0.0f);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK | btCollisionObject::CF_CHARACTER_OBJECT);
	world->addRigidBody(body);
	gContactAddedCallback = callbackFunc;
}

void Player::update(GLFWwindow* window) {
	// Time variables
	static float oldTime = 0.0f;
	float time = glfwGetTime();
	float timeDiff = time - oldTime;
	oldTime = glfwGetTime();

	// Mouse variables
	double newX, newY;
	glfwGetCursorPos(window, &newX, &newY);
	static double oldX = newX, oldY = newY;

	// Speed related stuff
	float moveSpeed = timeDiff * 100.0f * 1.1f;
	float jumpSpeed = timeDiff * 100.0f * 5.0f;
	float lookSpeed = timeDiff * 10.0f;

	glm::vec3 velocity(0.0f);

	// Keyboard Code - Position
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		velocity.x -= moveSpeed;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		velocity.x += moveSpeed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		velocity.z -= moveSpeed;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		velocity.z += moveSpeed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		velocity.y += jumpSpeed;


	// Mouse Code - Rotation
	double dx = newX - oldX;
	double dy = newY - oldY;
	oldX = newX;
	oldY = newY;
	rotate({ dy * lookSpeed, 0.0f, 0.0f });
	rotate({ 0.0f, dx * lookSpeed, 0.0f });

	move(velocity); // update the actual position :v)
}

void Player::move(glm::vec3 pos) {
	glm::vec3 temp(0.0f);
	temp.x = cos(glm::radians(rotation.y)) * pos.x - sin(glm::radians(rotation.y)) * pos.z;
	temp.y = pos.y;
	temp.z = sin(glm::radians(rotation.y)) * pos.x + cos(glm::radians(rotation.y)) * pos.z;

	body->setLinearVelocity(btVector3(temp.x, body->getLinearVelocity().getY(), temp.z));
}

void Player::rotate(glm::vec3 rot) {
	rotation += rot;
	if (rotation.x > 90.0f) rotation.x = 90.0f;
	if (rotation.x < -90.0f) rotation.x = -90.0f;
}

// GETTERS 
glm::mat4 Player::getViewMatrix() const {
	glm::mat4 view(1.0f);
	view = glm::rotate(view, glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f }); // pitch
	view = glm::rotate(view, glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f }); // yaw
	view = glm::translate(view, -getPosition());
	return view;
}

glm::vec3 Player::getPosition() const {
	btTransform trans; body->getMotionState()->getWorldTransform(trans);
	return glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
}

glm::vec3 Player::getRotation() const {
	return rotation;
}

glm::vec3 Player::getViewDir() const {
	glm::vec3 viewDir;
	viewDir.x = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	viewDir.y = -sin(glm::radians(rotation.x));
	viewDir.z = -cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));

	return viewDir;
}