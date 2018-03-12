#include "Level2D.h"

#include <string>
#include <fstream>

std::vector<Wall> Level2D::walls;
glm::vec3 Level2D::startPos;

void Level2D::makeFromArray(int* arr, int width, int height) {
	walls.clear();
	for (int z = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
			if (arr[x + z * width] == 1)
				walls.emplace_back(glm::vec3(x, 0.0f, -height + z + 1), glm::vec3(0.5f), Wall::WALL);
			else if (arr[x + z * width] == 2) {
				walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), glm::vec3(0.5f), Wall::START);
				startPos = glm::vec3(x, 0.0f, -height + z + 1);
			}
			else if (arr[x + z * width] == 3)
				walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), glm::vec3(0.5f), Wall::FINISH);
			else
				walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), glm::vec3(0.5f), Wall::FLOOR);
		}
	}
}

void Level2D::loadFromFile(const char* filePath) {
	std::ifstream file;
	file.open(filePath);

	if (file.is_open()) {
		std::string line, data;
		int width(0), height(0);

		while (getline(file, line)) {
			data += line;
			height++;
		}
		width = line.size();

		walls.clear();
		std::cout << "Loaded level: " << filePath << "!\nWidth: " << width << ", height: " << height << "\n";

		for (int z = 0; z < height; z++) {
			for (int x = 0; x < width; x++) {
				if (data.at(x + z * width) == '#')
					walls.emplace_back(glm::vec3(x, 0.0f, -height + z + 1), glm::vec3(0.5f), Wall::WALL);
				else if (data.at(x + z * width) == 'S') {
					walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), glm::vec3(0.5f), Wall::START);
					startPos = glm::vec3(x, 0.0f, -height + z + 1);
				}
				else if (data.at(x + z * width) == 'F')
					walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), glm::vec3(0.5f), Wall::FINISH);
				else
					walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), glm::vec3(0.5f), Wall::FLOOR);
			}
		}
	}
	else
		std::cout << "Couldn't open the file: " << filePath << "!\n";

	file.close();
}