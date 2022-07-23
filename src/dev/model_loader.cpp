#include "model.hpp"
#include "../area_light.hpp"
#include "../vec2.hpp"
#include <fstream>

namespace helper {

void addMeshAttrSize(specter::MeshAttributeSizes& dst, const specter::MeshAttributeSizes& src) {
	dst.fsize += src.fsize;
}

void assignMeshIdxTable(specter::MeshIndexTable& dst, const specter::MeshAttributeSizes& src) {
	dst.f = src.fsize;
};

std::string GetPathWithoutFile(const std::string& path) {
	auto delim_pos = path.find_last_of("/");
	return path.substr(0, delim_pos + 1);
}

std::string GetMtllibPath(const std::string& path, const std::string& libname) {
	return GetPathWithoutFile(path) + libname;
}

static std::string ParseMtllibSuffix(std::istringstream& isstr, const char* filename) {
	std::string libname;
	isstr >> libname;
	return GetMtllibPath(filename, libname);
}

static void ParseWsDelimLine(std::istringstream& isstr, std::vector<std::string>& dst) {
	while (true) {
		std::string v;
		isstr >> v;
		if (!v.empty()) {
			dst.push_back(v);
		} else {
			break;
		}
	}
}

static specter::vec2f ParseVec2(std::istringstream& isstr) {
	specter::vec2f v;
	isstr >> v.x >> v.y;
	return v;
}

static specter::vec3f ParseVec3(std::istringstream& isstr) {
	specter::vec3f v;
	isstr >> v.x >> v.y >> v.z;
	return v;
}

}

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
			} else {
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

void Model::parse(const char* filename) {
	std::ifstream objfile(filename, std::ios::binary);
	if (objfile.fail()) {
		throw std::runtime_error("Couldn't load file");
	}

	std::cout << "Loading file " << filename << " ...\n";

	bool normalsPresent = false;
	bool uvPresent = false;
	ComponentList cList = ComponentList::ALL_COMPONENTS;

	MaterialMap mtl_map;
	MeshAttributeSizes sAggregateSize;
	MeshAttributeSizes sMeshSize;

	specter::Timer timer;

	// Find potentially missing triangle attributes(e.g missing normals) that will change the way 
	// faces have to be processed.
	std::string line;
	while (std::getline(objfile, line)) {

		std::istringstream lineStream(line);
		
		std::string prefix;
		lineStream >> prefix;

		if (prefix == "mtllib") {
			auto libpath = helper::ParseMtllibSuffix(lineStream, filename);
			parseMaterialLibrary(libpath.c_str(), mtl_map);
		} else if (prefix == "usemtl") {
			std::string mtl_name;
			lineStream >> mtl_name;
			
			mesh_attribute_sizes.emplace_back();
			mesh_indices.emplace_back();

			// Find the material name in the material map and assign the material index 
			// to the mesh index table.
			for (const auto& kv : mtl_map) {
				if (mtl_name == kv.first) {
					mesh_indices.back().m = kv.second;
				}
			}
		} else if (prefix == "v") {
			vertices.push_back(helper::ParseVec3(lineStream));
		} else if (prefix == "vt") {
			// Some object files don't contain texture coordinates. In that case, face processing needs to be adjusted.
			uvPresent = true;
			uvs.push_back(helper::ParseVec2(lineStream));
		} else if (prefix == "vn") {
			// Some object files don't contain vertex normals. In that case, face processing needs to be adjusted.
			normalsPresent = true;
			normals.push_back(helper::ParseVec3(lineStream));
		} else if (prefix == "f") {
			if (uvPresent && normalsPresent) cList = ComponentList::ALL_COMPONENTS;
			if (uvPresent && !normalsPresent) cList = ComponentList::NORMALS_MISSING;
			if (!uvPresent && normalsPresent) cList = ComponentList::UV_MISSING;
			if (!uvPresent && !normalsPresent) cList = ComponentList::ONLY_POSITIONS;
			// We now know what attributes are missing. Therefore, we can continue with 
			// the parsing of faces in the following loop.
			break;
		}
	}

	// This loop processes files by reading the 'prefix' of the line (e.g 'vt') 
	// and processing the 'suffix' (e.g two floats) in the appropriate manner.
	// Additionally, materials are correctly associated to their respective 
	// faces.
	do {
		std::istringstream lineStream(line);
		std::string prefix;

		lineStream >> prefix;
		if (prefix == "usemtl") {
			std::string mtl_name;
			lineStream >> mtl_name;
			
			// Add the size of the mesh to the MeshAttributeSizes object
			helper::addMeshAttrSize(mesh_attribute_sizes.back(), sMeshSize);

			// Add an additional mesh for processing
			mesh_attribute_sizes.emplace_back();
			mesh_indices.emplace_back();
			
			// Find the material name in the material map and assign the material index 
			// to the mesh index table.
			for (const auto& kv : mtl_map) {
				if (mtl_name == kv.first) {
					mesh_indices.back().m = kv.second;
				}
			}

			// Update the aggregate size of the model
			helper::addMeshAttrSize(sAggregateSize, sMeshSize);
			// Update the offset to the indices of the current mesh.
			helper::assignMeshIdxTable(mesh_indices.back(), sAggregateSize);

			sMeshSize = MeshAttributeSizes();
		} else if (prefix == "v") {
			vertices.push_back(helper::ParseVec3(lineStream));
		} else if (prefix == "vt") {
			uvs.push_back(helper::ParseVec2(lineStream));
		} else if (prefix == "vn") {
			normals.push_back(helper::ParseVec3(lineStream));
		} else if (prefix == "f") {
			// This vector contains whitespace seperated faces. A face is usually of the form
			// 'v/vt/vn'. 
			std::vector<std::string> lines;
			// wavefront obj files are allowed to have faces defined as triangle fans. 
			// We need to handle cases where more than 1 triangle is specified.
			helper::ParseWsDelimLine(lineStream, lines);

			if (lines.size() < 3) {
				throw std::runtime_error("File: " + std::string(filename) + " invalid. Face doesn't specify triangle (less than three indices specified)");
			}

			std::vector<FaceElement> faceIndices;
			faceIndices.resize(3);

			// Convert the '/' delimited face attributes into unsigned indices.
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

			// Triangulate the face, if necessary.
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
			sMeshSize.fsize += (lines.size() - 2) * 3;
		}
	} while (std::getline(objfile, line));

	objfile.close();

	// Update the "MeshAttributeSize" for the last mesh. This has to be done, because 
	// the stream ends before encountering another line with prefix "usemtl". 
	// Without this line, the last mesh's attribute sizes are not updated.
	helper::addMeshAttrSize(mesh_attribute_sizes.back(), sMeshSize);

	vertices.shrink_to_fit();
	normals.shrink_to_fit();
	uvs.shrink_to_fit();
	faces.shrink_to_fit();

	nMeshes = mesh_attribute_sizes.size();

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

void Model::parseMaterialLibrary(const char* filename, MaterialMap& mtl_map) {
	std::ifstream libfile(filename, std::ios::binary);
	if (libfile.fail()) {
		std::cout << "Error!\n";
	}

	std::string line;
	std::string mtlname;
	MaterialType mtltype;

	while (std::getline(libfile, line)) {
		std::istringstream lineStream(line);
		std::string prefix;
		lineStream >> prefix;
		if (prefix == "newmtl") {
			mtlname.clear();
			lineStream >> mtlname;
			if (mtlname.find("Metal") != std::string::npos) {
				mtltype = MaterialType::Metal;
			} else {
				mtltype = MaterialType::Dielectric;
			}
		} else if (prefix == "Ke") {
			if (mtlname.find("Light") != std::string::npos) {
				materials.emplace_back(std::make_shared<AreaLight>(helper::ParseVec3(lineStream)));
				mtl_map.emplace_back(mtlname, mtl_map.size());
			}
		} else if (prefix == "Kd" && mtltype == MaterialType::Dielectric) {
			materials.emplace_back(std::make_shared<Lambertian>(helper::ParseVec3(lineStream)));
			mtl_map.emplace_back(mtlname, mtl_map.size());
		} else if (prefix == "Ks" && mtltype == MaterialType::Metal) {
			materials.emplace_back(std::make_shared<Metal>(helper::ParseVec3(lineStream)));
			mtl_map.emplace_back(mtlname, mtl_map.size());
		}
	}
}

}