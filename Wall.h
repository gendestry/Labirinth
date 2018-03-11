#pragma once
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Wall {
private:
	glm::vec3 position;
	glm::vec3 scale;
	static unsigned int vao, vbo;
	static bool generated;

	void generateModel();
public:
	Wall(glm::vec3 pos, glm::vec3 scale = {1.0f, 1.0f, 1.0f});
	Wall(float x, float y, float z, float sx = 1.0f, float sy = 1.0f, float sz = 1.0f) : Wall(glm::vec3(x, y, z), glm::vec3(sx, sy, sz)) {}

	static void render();
	static void deleteModel();

	inline static void bind() { glBindVertexArray(vao); }
	inline static void unbind() { glBindVertexArray(0); }
	inline const glm::vec3& getPosition() const { return position; }
	inline const glm::vec3& getScale() const { return scale; }
	const glm::mat4 getModelMatrix() const;
};