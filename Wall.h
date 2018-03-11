#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>

class Wall {
private:
	glm::vec3 position;
	glm::vec3 scale;
	static unsigned int vao, vbo;
	static unsigned int t_wall, t_start, t_finish, t_floor;
	static bool generated;

	void generateModel();
	void generateTextures();
public:
	enum Type {
		WALL,
		START,
		FINISH,
		FLOOR
	} type;

	Wall(glm::vec3 pos, glm::vec3 scale = {1.0f, 1.0f, 1.0f}, Type t = WALL);

	static void render();
	static void cleanup();
	static void bind();
	inline static void unbind() { glBindVertexArray(0); }

	inline const glm::vec3& getPosition() const { return position; }
	inline const glm::vec3& getScale() const { return scale; }
	const glm::mat4 getModelMatrix() const;
};