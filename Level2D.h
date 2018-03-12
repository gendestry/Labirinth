#pragma once
#include <vector>

#include "Wall.h"

class Level2D {
private:
	static std::vector<Wall> walls;
public:
	static void makeFromArray(int* arr, int width, int height);
	static void loadFromFile(const char* filePath);
	inline static std::vector<Wall>& getWalls() { return walls; }
};