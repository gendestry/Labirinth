#pragma once
#include <vector>
#include <btBulletDynamicsCommon.h>

#include "Wall.h"

class Level2D {
private:
	static std::vector<Wall> walls;
	static glm::vec3 startPos;
public:
	static void loadFromFile(const char* filePath, btDiscreteDynamicsWorld* world);
	inline static std::vector<Wall>& getWalls() { return walls; }
	inline static glm::vec3 getStartPos() { return startPos; }
};

