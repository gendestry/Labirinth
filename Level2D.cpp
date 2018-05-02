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
			if (width < line.size())
				width = line.size();
			data += line;
			height++;
		}

		walls.clear();
		walls.reserve(width * height);

		std::cout << "Loaded level: " << filePath << "\nWidth: " << width << ", height: " << height << "\n";

		for (int j = height - 1; j >= 0; j--) {
			for (int i = 0; i < width; i++) {
				int index = i + (height - j - 1) * height;
				float x = i / 2.0f;
				float z = -j / 2.0f;

				if (data.at(index) == '#')
					walls.emplace_back(glm::vec3(x, 0, z), 0.5f, Wall::WALL , world);
				else if (data.at(index) == 'S') {
					if (!setStartPos) {
						walls.emplace_back(glm::vec3(x, -0.5, z), 0.5f, Wall::START, world);
						startPos = glm::vec3(x, 0, z);
						setStartPos = true;
					}
					else {
						walls.emplace_back(glm::vec3(x, -0.5, z), 0.5f, Wall::FLOOR, world);
						std::cout << "There are more than one start positions in the file!\n";
						std::cout << "This one will be ignored!\n";
					}
				}
				else if (data.at(index) == 'F')
					walls.emplace_back(glm::vec3(x, -0.5, z), 0.5f, Wall::FINISH, world);
				else
					walls.emplace_back(glm::vec3(x, -0.5, z), 0.5f, Wall::FLOOR, world);
			}
		}

		if (!setStartPos) {
			std::cout << "No start position set, choosing default.\n";
			startPos = glm::vec3(0, 5, 0);
		}
	}
	else
		std::cout << "Couldn't open the file: " << filePath << "!\n";

	file.close();
}