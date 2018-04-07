#include "Level3D.h"

#include <fstream>
#include <string>

glm::vec3 Level3D::startPos;
std::vector<Wall> Level3D::walls;

void Level3D::loadFromFile(const char* filePath, btDiscreteDynamicsWorld* world) {
	std::ifstream file;
	file.open(filePath);

	if (file.is_open()) {
		struct Chunk {
			std::string data;
			int width, height;

			Chunk(std::string d, int w, int h) : data(d), width(w), height(h) {}
		};
		
		std::vector<Chunk> chunks;

		{
			std::string line, data;

			int cwidth = 0, cheight = 0;

			while (getline(file, line)) {
				if (line.size() == 0) {
					chunks.emplace_back(data, cwidth, cheight);
					cwidth = 0;
					cheight = 0;
					data = "";
				}
				else {
					data += line;
					cwidth = line.size();
					cheight++;
				}
			}

			chunks.emplace_back(data, cwidth, cheight);
		}
		walls.clear();

		// assuming all the chunks have the same widht / height
		std::cout << "Loaded level: " << filePath << "\nX-size: " << chunks[0].width << ", Z-size: " << chunks[0].height << ", Height: " << chunks.size() << "\n";

		bool setStartPos = false;
		for (int i = 0; i < chunks.size(); i++) {
			float height = chunks[i].height;
			float width =  chunks[i].width;
			std::string data = chunks[i].data;
			float y = float(chunks.size() - i) / 2.0f - 0.5f;

			for (float z = 0; z < height / 2; z += 0.5f) {
				for (float x = 0; x < width / 2; x += 0.5f) {
					int index = (int)(x * 2 + z * 2 * width);
					if (data.at(index) == '#')
						walls.emplace_back(glm::vec3(x, y, -height / 2.0f + z + 0.5f), 0.5f, Wall::WALL, world);
					else if (data.at(index) == 'S') {
						if (!setStartPos) {
							walls.emplace_back(glm::vec3(x, y, -height / 2.0f + z + 0.5f), 0.5f, Wall::START, world);
							startPos = glm::vec3(x, y + 0.5f, -height / 2.0f + z + 0.5f);
							setStartPos = true;
						}
						else {
							walls.emplace_back(glm::vec3(x, y, -height / 2.0f + z + 0.5f), 0.5f, Wall::WALL, world);
							std::cout << "More than one starting position detected! \n";
						}
					}
					else if (data.at(index) == 'F')
						walls.emplace_back(glm::vec3(x, y, -height / 2.0f + z + 0.5f), 0.5f, Wall::FINISH, world);
					else if (data.at(index) == 'T') {
						walls.emplace_back(glm::vec3(x, y, -height / 2.0f + z + 0.5f), 0.5f, Wall::FLOOR, world);
					}
				}
			}
		}
		
		for (int i = 0; i < walls.size(); i++)
			walls[i].body->setUserPointer(&walls[i]);

		if(!setStartPos)
			startPos = glm::vec3(0, 5, 0);
	}
	else
		std::cout << "Couldn't open the file: " << filePath << "!\n";

	file.close();
}