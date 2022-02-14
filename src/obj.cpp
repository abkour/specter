#include "obj.hpp"

#include <string>
#include <vector>

#include <sstream>

namespace specter {

enum class ComponentList {
	ALL_COMPONENTS = 0, UV_MISSING, NORMALS_MISSING, ONLY_POSITIONS
};

struct FaceTokenizer {

	unsigned p;
	unsigned t;
	unsigned n;

	FaceTokenizer(const std::string& line, ComponentList cList) {
		p = std::numeric_limits<uint32_t>::max();
		t = std::numeric_limits<uint32_t>::max();
		n = std::numeric_limits<uint32_t>::max();

		auto tokens = tokenize(line, '/');

		p = std::stoul(tokens[0]) - 1;

		switch (cList) {
		case ComponentList::ALL_COMPONENTS:
			t = std::stoul(tokens[1]) - 1;
			n = std::stoul(tokens[2]) - 1;
			break;
		case ComponentList::UV_MISSING:
			n = std::stoul(tokens[1]) - 1;
			break;
		case ComponentList::NORMALS_MISSING:
			t = std::stoul(tokens[1]) - 1;
			break;
		default:
			break;
		}
	}

protected:

	std::vector<std::string> tokenize(const std::string& line, char delimeter) {
		std::vector<std::string> tokens;
		std::size_t stringOffset = 0;
		std::size_t substrLength = line.size();
		while (substrLength > 0) {
			substrLength = line.find_first_of(delimeter, stringOffset);
			if (substrLength == std::string::npos) {
				tokens.push_back(line.substr(stringOffset, line.size() - stringOffset));
				return tokens;
			}
			else {
				tokens.push_back(line.substr(stringOffset, substrLength - stringOffset));
			}
			// Delimeter characters can occur as strings. To get to the next substring 
			// we need to bridge the sequence of delimeter characters.
			std::size_t delimeterStringLength = 1;
			for (int i = substrLength + 1; line[i] == delimeter; ++i) {
				delimeterStringLength++;
			}
			stringOffset = substrLength + delimeterStringLength;
		}
		return tokens;
	}

};

ObjLoader::ObjLoader(const char* filename) {
	std::ifstream objfile(filename, std::ios::binary);
	if (objfile.fail()) {
		throw std::runtime_error("Couldn't load file");
	}

	std::cout << "Loading file " << filename << " ...\n";

	std::vector<vec3f> sVertices;
	std::vector<vec3f> sNormals;
	std::vector<vec2f> sTextureCoordinates;

	bool normalsPresent = false;
	bool uvPresent = false;
	ComponentList cList = ComponentList::ALL_COMPONENTS;

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
		}
		else if (prefix == "vt") {
			// Some object files don't contain texture coordinates. In that case, face processing needs to be adjusted.
			uvPresent = true;
			vec2f v;
			lineStream >> v.x; lineStream >> v.y;
			sTextureCoordinates.push_back(v);
		}
		else if (prefix == "vn") {
			// Some object files don't contain vertex normals. In that case, face processing needs to be adjusted.
			normalsPresent = true;
			vec3f v;
			lineStream >> v.x; lineStream >> v.y; lineStream >> v.z;
			sNormals.push_back(v);
		}
		else if (prefix == "f") {
			if (uvPresent && normalsPresent) cList = ComponentList::ALL_COMPONENTS;
			if (uvPresent && !normalsPresent) cList = ComponentList::NORMALS_MISSING;
			if (!uvPresent && normalsPresent) cList = ComponentList::UV_MISSING;
			if (!uvPresent && !normalsPresent) cList = ComponentList::ONLY_POSITIONS;
			break;
		}
	}

	do {
		std::istringstream lineStream(line);
		std::string prefix;

		lineStream >> prefix;
		if (prefix == "v") {
			vec3f v;
			lineStream >> v.x; lineStream >> v.y; lineStream >> v.z;
			sVertices.push_back(v);
		}
		else if (prefix == "vt") {
			vec2f v;
			lineStream >> v.x; lineStream >> v.y;
			sTextureCoordinates.push_back(v);
		}
		else if (prefix == "vn") {
			vec3f v;
			lineStream >> v.x; lineStream >> v.y; lineStream >> v.z;
			sNormals.push_back(v);
		}
		else if (prefix == "f") {
			std::vector<std::string> lines;
			// wavefront obj files are allowed to have faces defined as triangle fans. 
			// Although not that common, we need to handle cases where more than 1 or 2 triangles are specified.
			while (true) {
				std::string v;
				lineStream >> v;
				if (!v.empty()) {
					lines.push_back(v);
				}
				else {
					break;
				}
			}

			if (lines.size() < 3) {
				throw std::runtime_error("Object file invalid. Face doesn't specify triangle (less than three indices specified)");
			}

			std::vector<FaceElement> faceIndices;
			faceIndices.resize(3);

			FaceTokenizer t0(lines[0], cList);
			FaceTokenizer t1(lines[1], cList);
			FaceTokenizer t2(lines[2], cList);

			faceIndices[0].p = t0.p;
			faceIndices[1].p = t1.p;
			faceIndices[2].p = t2.p;

			switch (cList) {
			case ComponentList::ALL_COMPONENTS:
				faceIndices[0].n = t0.n;
				faceIndices[1].n = t1.n;
				faceIndices[2].n = t2.n;
				faceIndices[0].t = t0.t;
				faceIndices[1].t = t1.t;
				faceIndices[2].t = t2.t;
				break;
			case ComponentList::UV_MISSING:
				faceIndices[0].n = t0.n;
				faceIndices[1].n = t1.n;
				faceIndices[2].n = t2.n;
				break;
			case ComponentList::NORMALS_MISSING:
				faceIndices[0].t = t0.t;
				faceIndices[1].t = t1.t;
				faceIndices[2].t = t2.t;
				break;
			default:
				break;
			}

			faces.push_back(faceIndices[0]);
			faces.push_back(faceIndices[1]);
			faces.push_back(faceIndices[2]);

			// Triangulate the face
			if (lines.size() > 3) {
				for (int i = 2; i <= lines.size() - 2; ++i) {
					FaceTokenizer tx(lines[i], cList);
					FaceTokenizer ty(lines[i + 1], cList);

					switch (cList) {
					case ComponentList::ALL_COMPONENTS:
						faceIndices[1].p = tx.p;
						faceIndices[1].t = tx.t;
						faceIndices[1].n = tx.n;
						faceIndices[2].p = ty.p;
						faceIndices[2].t = ty.t;
						faceIndices[2].n = ty.n;
						break;
					case ComponentList::UV_MISSING:
						faceIndices[1].p = tx.p;
						faceIndices[1].n = tx.n;
						faceIndices[2].p = ty.p;
						faceIndices[2].n = ty.n;
						break;
					case ComponentList::NORMALS_MISSING:
						faceIndices[1].p = tx.p;
						faceIndices[1].t = tx.t;
						faceIndices[2].p = ty.p;
						faceIndices[2].t = ty.t;
						break;
					default:
						break;
					}

					faces.push_back(faceIndices[0]);
					faces.push_back(faceIndices[1]);
					faces.push_back(faceIndices[2]);
				}
			}
		}
	} while (std::getline(objfile, line));

	objfile.close();

	vertices.resize(sVertices.size());
	std::memcpy(vertices.data(), sVertices.data(), sizeof(vec3f) * sVertices.size());

	normals.resize(sNormals.size());
	std::memcpy(normals.data(), sNormals.data(), sizeof(vec3f) * sNormals.size());

	textureCoordinates.resize(sTextureCoordinates.size());
	std::memcpy(textureCoordinates.data(), sTextureCoordinates.data(), sizeof(vec2f) * sTextureCoordinates.size());

	faces.shrink_to_fit();

	std::cout << "Succesfully loaded file in " << timer.elapsedTime() << " seconds.\t\t(Triangles: " << faces.size() / 3 << ", Vertices: " << vertices.size() << ")\n";
	switch (cList) {
	case ComponentList::NORMALS_MISSING:
		std::cout << "Note: Normals are missing!\n";
		break;
	case ComponentList::UV_MISSING:
		std::cout << "Note: UV coordinates missing!\n";
		break;
	case ComponentList::ONLY_POSITIONS:
		std::cout << "Note: Normals and UV coordinates missing!\n";
	default:
		break;
	}
}




}