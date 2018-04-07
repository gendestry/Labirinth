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
		bool setStartPos = false;
		int width = 0, height = 0;

		while (getline(file, line)) {
			data += line;
			height++;
		}
		width = line.size();

		walls.clear();
		std::cout << "Loaded level: " << filePath << "\nWidth: " << width << ", height: " << height << "\n";

		for (float z = 0; z < height / 2; z += 0.5f) {
			for (float x = 0; x < width / 2; x += 0.5f) {
				int index = (int)(x * 2 + z * 2 * width);
				if (data.at(index) == '#')
					walls.emplace_back(glm::vec3(x, 0.0f, -height / 2.0f + z + 0.5f), 0.5f, Wall::WALL, world);
				else if (data.at(index) == 'S') {
					if (!setStartPos) {
						walls.emplace_back(glm::vec3(x, -0.5f, -height / 2.0f + z + 0.5f), 0.5f, Wall::START, world);
						startPos = glm::vec3(x, 0.1f, -height / 2.0f + z + 0.5f);
						setStartPos = true;
					}
					else {
						walls.emplace_back(glm::vec3(x, -0.5f, -height / 2.0f + z + 0.5f), 0.5f, Wall::FLOOR, world);
						std::cout << "More than one starting position detected!\n";
					}
				}
				else if (data.at(index) == 'F')
					walls.emplace_back(glm::vec3(x, -0.5f, -height / 2.0f + z + 0.5f), 0.5f, Wall::FINISH, world);
				else
					walls.emplace_back(glm::vec3(x, -0.5f, -height / 2.0f + z + 0.5f), 0.5f, Wall::FLOOR, world);
			}
		}

		for (int i = 0; i < walls.size(); i++)
			walls[i].body->setUserPointer(&walls[i]);

		if (!setStartPos)
			startPos = glm::vec3(0, 5, 0);
	}
	else
		std::cout << "Couldn't open the file: " << filePath << "!\n";

	file.close();
}