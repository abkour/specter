#include "helpers.hpp"
#include "model.hpp"
#include "../rtx/area_light.hpp"
#include "../math/vec2.hpp"


#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace helper {

bool isEqual(std::string_view& s0, std::string_view& s1) {
	return std::equal(	s0.begin(), s0.end(),
						s1.begin(), s1.end(),
						[](char a, char b)
						{
							return tolower(a) == tolower(b);
						});
}

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

static std::string ParseMtllibSuffix(std::istringstream& isstr, const std::string& filename) {
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

enum class ComponentList : uint8_t {
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

void Model::parse(const std::string& filename) {
	auto file_extension = filesystem::get_file_extension(filename);
	if(file_extension == "scene" || file_extension == "obj") {
		parse_obj_file(filename);
	} else if(file_extension == "sff") {
		std::cout << "SFF files not supported yet.\n";
	} else {
		std::cout << "Unknown file format.\n";
	}
}

void Model::parse_obj_file(const std::string& filename) {
	Model model;
	
	std::ifstream objfile(filename.data(), std::ios::binary);
	if (objfile.fail()) {
		throw std::runtime_error("Couldn't load file");
	}

	std::cout << "Loading file " << filename << " ...\n";

	bool normalsPresent = false;
	bool uvPresent = false;
	bool attrDetermined = false;
	ComponentList cList = ComponentList::ALL_COMPONENTS;

	MaterialMap mtl_map;
	MeshAttributeSizes sAggregateSize;
	MeshAttributeSizes sMeshSize;

	specter::Timer timer;

	// This loop processes files by reading the 'prefix' of the line (e.g 'vt') 
	// and processing the 'suffix' (e.g two floats) in the appropriate manner.
	// Additionally, materials are correctly associated to their respective 
	// faces.
	std::string line;
	while (std::getline(objfile, line)) {
		std::istringstream lineStream(line);
		std::string prefix;

		lineStream >> prefix;
		if (prefix == "mtllib") {
			auto libpath = helper::ParseMtllibSuffix(lineStream, filename);
			parseMaterialLibrary(libpath, mtl_map);
		} else if (prefix == "usemtl") {
			std::string mtl_name;
			lineStream >> mtl_name;
			
			meshNames.emplace_back(mtl_name);
			meshName_hashes.emplace_back(std::hash<std::string>{}(mtl_name));
			// Add the size of the mesh to the MeshAttributeSizes object
			if (!mesh_attribute_sizes.empty()) {
				helper::addMeshAttrSize(mesh_attribute_sizes[mesh_attribute_sizes.size() - 1], sMeshSize);
				// Update the aggregate size of the IModel
				helper::addMeshAttrSize(sAggregateSize, sMeshSize);
			}

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

			// Update the offset to the indices of the current mesh.
			helper::assignMeshIdxTable(mesh_indices.back(), sAggregateSize);

			sMeshSize = MeshAttributeSizes();
		} else if (prefix == "v") {
			attrDetermined = false;
			vertices.push_back(helper::ParseVec3(lineStream));
		} else if (prefix == "vt") {
			uvPresent = true;
			uvs.push_back(helper::ParseVec2(lineStream));
		} else if (prefix == "vn") {
			normalsPresent = true;
			normals.push_back(helper::ParseVec3(lineStream));
		} else if (prefix == "f") {
			if (!attrDetermined) {
				if (uvPresent && normalsPresent) cList = ComponentList::ALL_COMPONENTS;
				if (uvPresent && !normalsPresent) cList = ComponentList::NORMALS_MISSING;
				if (!uvPresent && normalsPresent) cList = ComponentList::UV_MISSING;
				if (!uvPresent && !normalsPresent) cList = ComponentList::ONLY_POSITIONS;
				uvPresent = false;
				normalsPresent = false;
				attrDetermined = true;
			}
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

			if (cList == ComponentList::NORMALS_MISSING) {
				const vec3f e0 = vertices[t1.p] - vertices[t0.p];
				const vec3f e1 = vertices[t2.p] - vertices[t0.p];
				normals.emplace_back(normalize(cross(e0, e1)));
				faceIndices[0].n = faceIndices[1].n = faceIndices[2].n = normals.size() - 1;
			}

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
						faceIndices[1].n = faceIndices[0].n;
						faceIndices[2].p = ty.p;
						faceIndices[2].t = ty.t;
						faceIndices[2].n = faceIndices[0].n;
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
	}

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

void Model::parse_sff_file(const std::string& filename) {

}


void Model::parseMaterialLibrary(const std::string& filename, MaterialMap& mtl_map) {
	std::ifstream libfile(filename, std::ios::binary);
	if (libfile.fail()) {
		std::cout << "Error!\n";
	}

	std::string line;
	MaterialType mtltype;
	
	bool first_material = true;

	filesystem::ObjMtlComponent material_component;

	while (std::getline(libfile, line)) {
		std::istringstream lineStream(line);
		std::string prefix;
		lineStream >> prefix;
		if (prefix == "newmtl") {
			// We will need to iterate over all components within the material.
			// However, we only want to add the material once all of the available components have been read.
			// The only guaranteed component is the material name. Therefore, this is the point where we 
			// are inserting the material into the materials vector. 
			// We add the material to the vector only once all the components have been parsed.
			// Past the loop we will add the last material into the vector.
			if (!first_material) {
				material_components.emplace_back(material_component);
				// Reset the material to invalidate the previous values.
				material_component = filesystem::ObjMtlComponent();
			} else {
				first_material = false;
			}
			lineStream >> material_component.name;
			material_component.name_hash = std::hash<std::string>{}(material_component.name);
		} else if(prefix == "Ns") {
			lineStream >> material_component.ns;
		} else if(prefix == "Ni") {
			lineStream >> material_component.ni;
		} else if(prefix == "d") {
			lineStream >> material_component.d;
		} else if(prefix == "Tr") {
			lineStream >> material_component.tr;
		} else if(prefix == "Tf") {
			material_component.tf = helper::ParseVec3(lineStream);
		} else if (prefix == "Ka") {
			material_component.ka = helper::ParseVec3(lineStream);
		} else if (prefix == "Ke") {
			material_component.ke = helper::ParseVec3(lineStream);
		} else if (prefix == "Kd") {
			material_component.kd = helper::ParseVec3(lineStream);
		} else if (prefix == "Ks") {
			material_component.ks = helper::ParseVec3(lineStream);
		} else if (prefix.find("map_") != std::string::npos) {
			std::string texname;
			lineStream >> texname;
			std::string texpath = helper::GetPathWithoutFile(filename) + texname;
			std::string_view map_ext(prefix);
			if (map_ext.ends_with("Kd")) {
				material_component.texture_types.emplace_back(TextureType::Diffuse);
			} else if (map_ext.ends_with("Ke")) {
				material_component.texture_types.emplace_back(TextureType::Emissive);
			} else if (map_ext.ends_with("Ks")) {
				material_component.texture_types.emplace_back(TextureType::Specular);
			} else if (map_ext.ends_with("bump")) {
				material_component.texture_types.emplace_back(TextureType::Bump);
			} else {
				// Skip insertion of texpath into the texture_paths vector
				continue;
			}
			material_component.texture_paths.emplace_back(texpath);
			material_component.tp_hashes.emplace_back(std::hash<std::string>{}(texpath));
		}
	}

	// There is a final material that we have not added into the vector. Do it now.
	material_components.emplace_back(material_component);

	std::cout << "Validating material premises...\n";
	bool validation_successful = true;
	for (const auto& mtl_comp : material_components) {
		if (!mtl_comp.validate_vectorsize_premise()) {
			std::cout << "Failed validation test for material: " << mtl_comp.name << '\n';
			validation_successful = false;
		}
	}

	if (validation_successful) {
		std::cout << "Validation successful!\n";
	} else {
		std::cout << "Validation unsuccessful!\n";
	}
}

Model::~Model() {
	for (auto& texture : texture_data) {
		stbi_image_free(texture);
	}
}

}