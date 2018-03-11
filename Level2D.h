#pragma once
#include <vector>
#include "Wall.h"

class Level2D {
private:
	static std::vector<Wall> walls;
public:
	//static void makeFromArray(int* arr, int size);
	static void makeFromArray(int* arr, int width, int height);
	inline static std::vector<Wall>& getWalls() { return walls; }
};

std::vector<Wall> Level2D::walls;

void Level2D::makeFromArray(int* arr, int width, int height) {
	walls.clear();
	for (int z = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
			if (arr[x + z * width] == 1)
				walls.emplace_back(glm::vec3(x, 0.0f, -height+z+1), glm::vec3(0.5f), Wall::WALL);
			else if (arr[x + z * width] == 2)
				walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), glm::vec3(0.5f), Wall::START);
			else if (arr[x + z * width] == 3)
				walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), glm::vec3(0.5f), Wall::FINISH);
			else	
				walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), glm::vec3(0.5f), Wall::FLOOR);
		}
	}
}