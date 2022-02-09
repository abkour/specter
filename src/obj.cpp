#include "obj.hpp"

#include <string>
#include <vector>

namespace specter {

// Initializes the mesh through data in filename.obj. This method does NOT check for validity of the object file.
// Invalid object files will lead to undefined program behavior.
ObjLoader::ObjLoader(const char* filename) {
	std::ifstream objfile(filename, std::ios::binary);
	if (objfile.fail()) {
		throw std::runtime_error("Couldn't load file");
	}

	std::cout << "Loading file " << filename << " ...\n";

	std::vector<vec3f> sVertices;
	std::vector<vec3f> sNormals;
	std::vector<vec2f> sTextureCoordinates;
	
	Timer timer;

	std::string line;
	while (std::getline(objfile, line)) {

		std::istringstream lineStream(line);
		std::string prefix;
		
		lineStream >> prefix;

		if (prefix == "v") {
			vec3f v;
			lineStream >> v.x; lineStream >> v.y; lineStream >> v.z;
			sVertices.push_back(v);
		} else if (prefix == "vt") {
			vec2f v;
			lineStream >> v.x; lineStream >> v.y;
			sTextureCoordinates.push_back(v);
		} else if (prefix == "vn") {
			vec3f v;
			lineStream >> v.x; lineStream >> v.y; lineStream >> v.z;
			sNormals.push_back(v);
		} else if (prefix == "f") {
			// Holds the indices to the vertices/normals/uvCoords that make up one triangle.
			std::vector<vec3u> sFaces;
			sFaces.reserve(3);

			// Working array to store temporary indices.
			char tmp[16];
			int i = 0; 
			while (!lineStream.eof()) {
				vec3u face;
				lineStream.getline(tmp, 16, '//');
				face.x = std::stoul(tmp);
				lineStream.getline(tmp, 16, '//');
				face.y = std::stoul(tmp);
				lineStream.getline(tmp, 16, ' ');
				face.z = std::stoul(tmp);
				sFaces.push_back(face);
			}

			// Wavefront OBJ specifies faces as triangle fans, therefore the first index stays constant.
			for (int i = 1; i <= sFaces.size() - 2; ++i) {
				faces.push_back(sFaces[0]);
				faces.push_back(sFaces[i]);
				faces.push_back(sFaces[i + 1]);
			}
		}
	}

	vertices.resize(sVertices.size());
	std::memcpy(vertices.data(), sVertices.data(), sizeof(vec3f) * sVertices.size());

	normals.resize(sNormals.size());
	std::memcpy(normals.data(), sNormals.data(), sizeof(vec3f) * sNormals.size());

	textureCoordinates.resize(sTextureCoordinates.size());
	std::memcpy(textureCoordinates.data(), sTextureCoordinates.data(), sizeof(vec2f) * sTextureCoordinates.size());

	std::cout << "Succesfully loaded file in " << timer.elapsedTime() << " seconds.\n";
}

}