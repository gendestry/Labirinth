#include "Level3D.h"

#include <fstream>
#include <string>

glm::vec3 Level3D::startPos;
std::vector<Wall> Level3D::walls;

void Level3D::loadFromFile(const char* filePath, btDiscreteDynamicsWorld* world) {
	std::ifstream file;
	file.open(filePath);

	if (file.is_open()) {
		struct Layer {
			std::string data;
			int width, height;
			Layer(int w, int h, std::string d) : width(w), height(h), data(d) {}
		};

		std::vector<Layer> layers;
		bool setStartPos = false;

		{
			std::string line, data;
			int width = 0, height = 0;

			while (getline(file, line)) {
				if (line.size() == 0) {
					layers.emplace_back(width, height, data);
					width = 0;
					height = 0;
					data = "";
				}
				else {
					data += line;
					width = line.size();
					height++;
				}
			}

			layers.emplace_back(width, height, data);
		}

		walls.clear();
		walls.reserve(layers[0].width * layers[0].height * layers.size());

		std::cout << "Loaded level: " << filePath << "\n";

		for (int k = layers.size() - 1; k >= 0; k--) {
			int width = layers[k].width;
			int height = layers[k].height;
			std::string data = layers[k].data;

			for (int j = height - 1; j >= 0; j--) {
				for (int i = 0; i < width; i++) {
					int index = i + (height - j - 1) * height;
					float x =  i / 2.0f;
					float z = -j / 2.0f;
					float y =  (layers.size() - k) / 2.0f;

					if (data.at(index) == '#')
						walls.emplace_back(glm::vec3(x, y, z), 0.5f, Wall::WALL, world);
					else if (data.at(index) == 'F')
						walls.emplace_back(glm::vec3(x, y, z), 0.5f, Wall::FINISH, world);
					else if(data.at(index) == 'T')
						walls.emplace_back(glm::vec3(x, y, z), 0.5f, Wall::FLOOR, world);
					else if (data.at(index) == 'S') {
						if (!setStartPos) {
							walls.emplace_back(glm::vec3(x, y, z), 0.5f, Wall::START, world);
							startPos = glm::vec3(x, y + 0.5f, z);
							setStartPos = true;
						}
						else {
							walls.emplace_back(glm::vec3(x, y, z), 0.5f, Wall::FLOOR, world);
							std::cout << "There are more than one start positions in the file!\nThis one will be ignored!\n";
						}
					}
				}
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