#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <btBulletDynamicsCommon.h>
#include <iostream>

class Wall {
private:
	glm::vec3 position;
	float scale;

	static unsigned int vao, vbo;
	static unsigned int t_wall, t_start, t_finish, t_floor;
	static bool generated, cleaned;

	void generateModel();
	void generateTextures();
public:
	btRigidBody* body;
	
	enum Type {
		WALL,
		START,
		FINISH,
		FLOOR
	} type;

	Wall(glm::vec3 pos, float sc, Type t, btDiscreteDynamicsWorld* world);
	~Wall();

	static void render();
	static void bind();
	static void unbind();

	inline const glm::vec3& getPosition() const { return position; }
	inline const float getScale() const { return scale; }
	glm::mat4 getModelMatrix() const;
};

