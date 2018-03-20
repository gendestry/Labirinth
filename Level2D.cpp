#include "Level2D.h"

#include <iostream>
#include <string>
#include <fstream>

std::vector<Wall> Level2D::walls;
glm::vec3 Level2D::startPos;

void Level2D::loadFromFile(const char* filePath, btDiscreteDynamicsWorld* world) {
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
		std::cout << "Loaded level: " << filePath << "\nWidth: " << width << ", height: " << height << "\n";

		/*for (int z = 0; z < height; z++) {
			for (int x = 0; x < width; x++) {
				if (data.at(x + z * width) == '#')
					walls.emplace_back(glm::vec3(x, 0.0f, -height + z + 1), 0.5f, Wall::WALL);
				else if (data.at(x + z * width) == 'S') {
					walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), 0.5f, Wall::START);
					startPos = glm::vec3(x, 0.0f, -height + z + 1);
				}
				else if (data.at(x + z * width) == 'F')
					walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), 0.5f, Wall::FINISH);
				else
					walls.emplace_back(glm::vec3(x, -1.0f, -height + z + 1), 0.5f, Wall::FLOOR);
			}
		}
		*/
		for (float z = 0; z < height / 2; z += 0.5f) {
			for (float x = 0; x < width / 2; x += 0.5f) {
				int index = (int)(x * 2 + z * 2 * width);

				if (data.at(index) == '#')
					walls.emplace_back(glm::vec3(x, 0.0f, -height / 2.0f + z + 0.5f), 0.5f, Wall::WALL, world);
				else if (data.at(index) == 'S') {
					walls.emplace_back(glm::vec3(x, -0.5f, -height / 2.0f + z + 0.5f), 0.5f, Wall::START, world);
					startPos = glm::vec3(x, 0.5f, -height / 2.0f + z + 0.5f);
				}
				else if (data.at(index) == 'F')
					walls.emplace_back(glm::vec3(x, -0.5f, -height / 2.0f + z + 0.5f), 0.5f, Wall::FINISH, world);
				else
					walls.emplace_back(glm::vec3(x, -0.5f, -height / 2.0f + z + 0.5f), 0.5f, Wall::FLOOR, world);
			}
		}
	}
	else
		std::cout << "Couldn't open the file: " << filePath << "!\n";

	file.close();
}