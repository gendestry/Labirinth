#pragma once
#include <vector>
#include <string>

class Cubemap {
private:
	unsigned int id;
	unsigned int vao, vbo;
	bool success;

	void loadCubemap(std::vector<std::string> faces);
public:
	/// RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK
	/// +X -X +Y -Y +Z -Z
	Cubemap(std::vector<std::string> faces);
	~Cubemap();

	void bind(unsigned pos = 0);
	bool getSuccess() const { return success; }
};