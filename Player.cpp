#include "Player.h"

Player::Player(glm::vec3 pos, const std::vector<Wall> &w) : position(pos), rotation(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)), view(glm::mat4()), falling(true), walls(w) {}

void Player::update(GLFWwindow* window) {
	move(window);
	updatePosition();
	velocity = glm::vec3(0.0f);
}

void Player::move(GLFWwindow* window) {
	// Time variables
	static float oldTime = 0.0f;
	float time = glfwGetTime();
	float timeDiff = time - oldTime;
	oldTime = glfwGetTime();

	// Mouse variables
	static double oldX, oldY;
	static bool first = true;
	double newX, newY;
	glfwGetCursorPos(window, &newX, &newY);

	// Speed related stuff
	float movementSpeed = 1.5f * timeDiff;
	float lookSpeed = 8.0f * timeDiff;
	float jumpSpeed = 8.0f * timeDiff;

	// Keyboard Code - Position
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		velocity.x -= movementSpeed;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		velocity.x += movementSpeed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		velocity.z -= movementSpeed;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		velocity.z += movementSpeed;

	/* TEMPORARY CHEAT FUNCTION */
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if(position.y < 5.0f)
			velocity.y += 5.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
		if(position.y > 0.0f)
			velocity.y -= 5.0f;
	}

	/*if (!falling) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {

		}
	}
	else {

	}*/

	// Mouse Code - Rotation
	if (first) {
		oldX = newX;
		oldY = newY;
		first = false;
	}
	else {
		double dx = newX - oldX;
		double dy = newY - oldY;
		oldX = newX;
		oldY = newY;
		updateRotation({ dy * lookSpeed, 0.0f, 0.0f });
		updateRotation({ 0.0f, dx * lookSpeed, 0.0f });
	}
}

void Player::collision(glm::vec3 &trans) {
	float scale =0.5f;
	float offset = (scale * 13) / 20;
	glm::vec3 newPos = position + trans;

	for (int i = 0; i < walls.size(); i++) {
		glm::vec3 wallPos = walls[i].getPosition();
		wallPos *= scale; // get them to player space

		if (wallPos.x - offset < position.x + trans.x && position.x + trans.x < wallPos.x + offset &&
			wallPos.z - offset < position.z + trans.z && position.z + trans.z < wallPos.z + offset && position.y == wallPos.y) {
			float xd = (position.x + trans.x) - (wallPos.x - offset);
			if(xd > offset)
				xd = (position.x + trans.x) - (wallPos.x + offset);
			
			float zd = (position.z + trans.z) - (wallPos.z - offset);
			if (zd > offset)
				zd = (position.z + trans.z) - (wallPos.z + offset);

			if (abs(xd) < abs(zd))
				trans.x -= xd;
			else
				trans.z -= zd;
		}

		if (wallPos.x - offset < position.x + trans.x && position.x + trans.x < wallPos.x + offset &&
			wallPos.z - offset < position.z + trans.z && position.z + trans.z < wallPos.z + offset && position.y - scale == wallPos.y) { // if it is under you
			if(walls[i].type == Wall::FINISH) { 
				std::cout << "You've won\n";
				trans.y += 5.0f;
			}
		}
	}
}

void Player::updatePosition() {
	glm::vec3 temp, oldPos = position;

	temp.x = cos(glm::radians(rotation.y)) * velocity.x - sin(glm::radians(rotation.y)) * velocity.z;
	temp.y = velocity.y;
	temp.z = sin(glm::radians(rotation.y)) * velocity.x + cos(glm::radians(rotation.y)) * velocity.z;

	collision(temp);

	position += temp;
	updateMatrix();
}

void Player::updateRotation(glm::vec3 rot) {
	rotation += rot;
	if (rotation.x > 90.0f) rotation.x = 90.0f;
	if (rotation.x < -90.0f) rotation.x = -90.0f;
	updateMatrix();
}

void Player::updateMatrix() {
	view = glm::mat4(1.0f);
	view = glm::rotate(view, glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f }); // pitch
	view = glm::rotate(view, glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f }); // yaw
	view = glm::translate(view, -position);
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
	os << vec.x << ", " << vec.y << ", " << vec.z<< "\n";
	return os;
}