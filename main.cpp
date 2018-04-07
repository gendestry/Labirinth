#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <btBulletDynamicsCommon.h>

#include "Shader.h"
#include "Wall.h"
#include "Player.h"
#include "Level2D.h"
#include "Level3D.h"
#include "Cubemap.h"

void init();
void cleanup();

const int SCR_WIDTH = 800, SCR_HEIGHT = 600;

GLFWwindow* window;

// TODO: make this code compatible with runtime error throwing
// TODO: maybe smartpointers dont be lazy >:(
btDiscreteDynamicsWorld* world;
btDefaultCollisionConfiguration* conf;
btCollisionDispatcher* disp;
btBroadphaseInterface* interf;
btSequentialImpulseConstraintSolver* solv;

int main() {
	init();

	std::vector<Wall> walls;
	/*Level2D::loadFromFile("levels/level0.lvl", world);
	walls = Level2D::getWalls();*/

	Level3D::loadFromFile("levels/level3d.lvl", world);
	walls = Level3D::getWalls();

	Shader wallShader("wallShader.vert", "wallShader.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");
	Player player(Level3D::getStartPos(), world);
	Cubemap skybox({ "res/skybox/right.jpg","res/skybox/left.jpg","res/skybox/top.jpg","res/skybox/bottom.jpg","res/skybox/front.jpg","res/skybox/back.jpg" });

	wallShader.use();
	wallShader.setMat4("proj", glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 500.0f));

	skyboxShader.use();
	skyboxShader.setMat4("proj", glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 500.0f));


	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		world->stepSimulation(1.f / 60.f);
		player.update(window);

		wallShader.use();
		wallShader.setMat4("view", player.getViewMatrix());
		wallShader.setVec3("viewPos", player.getPosition());
		
		Wall::bind();
		for (int i = 0; i < walls.size(); i++) {
			wallShader.setVec3("fragPos", walls[i].getPosition());
			wallShader.setMat4("model", walls[i].getModelMatrix());
			wallShader.setInt("tex", walls[i].type);
			Wall::render();
		}

		skyboxShader.use();
		skyboxShader.setMat4("view", glm::mat4(glm::mat3(player.getViewMatrix())));

		glDepthFunc(GL_LEQUAL);
		skybox.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Wall::unbind();

	cleanup();
	return 0;
}

void init() {
	/* WINDOW CODE */
	if (!glfwInit())
		std::cout << "couldnt init glfw\n";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Labirinth", NULL, NULL);
	if (!window) {
		glfwTerminate();
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);

	if (glewInit() != 0)
		std::cout << "couldnt init glew\n";

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.1, 0.3, 0.8, 1.0);


	/* BULLET PHYSICS */

	conf = new btDefaultCollisionConfiguration();
	disp = new btCollisionDispatcher(conf);
	interf = new btDbvtBroadphase();
	solv = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(disp, interf, solv, conf);
	world->setGravity({ 0.0f, -5.0f, 0.0f });
}

void cleanup() {
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject * obj = world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		if (body && body->getMotionState())
			delete body->getMotionState();

		world->removeCollisionObject(obj);
		delete obj;
	}

	delete world;
	delete solv;
	delete interf;
	delete disp;
	delete conf;

	Wall::cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();
}