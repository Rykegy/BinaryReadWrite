#include "objLoader.h"

using namespace HARIKEN;

objLoader::~objLoader()
{
}

objLoader::objLoader(const char * path)
{

	loadObj(path);

}

bool objLoader::loadObj(const char * path)
{

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	/* Old Implementation
	FILE * file;
	fopen_s(&file, path, "r");
	if (file == NULL) {
		printf("Can't open file\n");
		getchar();
		return false;
	}
	
	while (true) {
		char lineHeader[128];

		int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; /// Grr 
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

		}
		else {

			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		int vertexIndex = vertexIndices[i];
		int uvIndex = uvIndices[i];
		int normalIndex = normalIndices[i];

		glm::vec3 vert = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		vertices.push_back(vertex());

		vertices[i].position = vert;
		vertices[i].texture = uv;
		vertices[i].normal = normal;

	}
	*/
	//Sift through OBJ file for relevant information on vertices
	std::string line;

	std::ifstream inFile(path);

	std::vector <std::string> siftedlines;

	while (std::getline(inFile, line)) {

		int skip = 0;

		for (size_t i = 0; i < line.size(); i++) {

			if (i < skip)
				continue;

			if (line[i] != ' ' || line[i] != '#' || line[i] != '\n') {

				std::string read;

				if (line[i] == 'v' && line[i + 1] == ' ') {

					read = line[i];
					siftedlines.push_back(read);

				}

				if (line[i] == 'v' && (line[i + 1] == 'n' || line[i + 1] == 't') && line[i + 2] == ' ') {

					read = line[i];
					read += line[i + 1];
					siftedlines.push_back(read);

				}

				if (line[i] == 'f' && line[i + 1] == ' ') {

					read = line[i];
					siftedlines.push_back(read);

				}

				if (line[i] == '-' || isdigit(line[i])) {

					read = line[i];

					for (size_t j = i + 1; j < line.size(); j++) {

						if (line[j] != ' ')
							read += line[j];

						else {
							skip = j;
							break;
						}

						if (j + 1 == line.size())
							skip = j + 1;

					}

					siftedlines.push_back(read);

				}

			}

		}

	}

	int skip = 0;

	for (size_t i = 0; i < siftedlines.size(); i++) {

		if (static_cast<int>(i) < skip)
			continue;

		if (siftedlines[i] == "v") {

			std::stringstream ss1(siftedlines[i + 1]);
			std::stringstream ss2(siftedlines[i + 2]);
			std::stringstream ss3(siftedlines[i + 3]);

			glm::vec3 vertex;

			ss1 >> vertex.x;
			ss2 >> vertex.y;
			ss3 >> vertex.z;

			temp_vertices.push_back(vertex);

			skip = i + 4;

		}

		if (siftedlines[i] == "vn") {

			std::stringstream ss1(siftedlines[i + 1]);
			std::stringstream ss2(siftedlines[i + 2]);
			std::stringstream ss3(siftedlines[i + 3]);

			glm::vec3 normal;

			ss1 >> normal.x;
			ss2 >> normal.y;
			ss3 >> normal.z;

			temp_normals.push_back(normal);

			skip = i + 4;

		}

		if (siftedlines[i] == "vt") {

			std::stringstream ss1(siftedlines[i + 1]);
			std::stringstream ss2(siftedlines[i + 2]);

			glm::vec2 uv;

			ss1 >> uv.x;
			ss2 >> uv.y;

			uv.y = -uv.y;

			temp_uvs.push_back(uv);

			skip = i + 3;

		}

		if (siftedlines[i] == "f") {

			std::stringstream ss1(siftedlines[i + 1]);
			std::stringstream ss2(siftedlines[i + 2]);
			std::stringstream ss3(siftedlines[i + 3]);

			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			char slash;

			ss1 >> vertexIndex[0];
			ss1 >> slash;
			ss1 >> uvIndex[0];
			ss1 >> slash;
			ss1 >> normalIndex[0];

			ss2 >> vertexIndex[1];
			ss2 >> slash;
			ss2 >> uvIndex[1];
			ss2 >> slash;
			ss2 >> normalIndex[1];

			ss3 >> vertexIndex[2];
			ss3 >> slash;
			ss3 >> uvIndex[2];
			ss3 >> slash;
			ss3 >> normalIndex[2];

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

			if (i + 4 == siftedlines.size()) {

				for (unsigned int i = 0; i < vertexIndices.size(); i++) {

					int vertexIndex = vertexIndices[i];
					int uvIndex = uvIndices[i];
					int normalIndex = normalIndices[i];

					glm::vec3 vert = temp_vertices[vertexIndex - 1];
					glm::vec2 uv = temp_uvs[uvIndex - 1];
					glm::vec3 normal = temp_normals[normalIndex - 1];

					vertices.push_back(HARIKEN::vertex());

					vertices[i].position = vert;
					vertices[i].texture = uv;
					vertices[i].normal = normal;

				}

				verticesList.push_back(vertices);
				vertices.clear();

				break;

			}
			
			else if (siftedlines[i + 4] != "f") {

				for (unsigned int i = 0; i < vertexIndices.size(); i++) {

					int vertexIndex = vertexIndices[i];
					int uvIndex = uvIndices[i];
					int normalIndex = normalIndices[i];

					glm::vec3 vert = temp_vertices[vertexIndex - 1];
					glm::vec2 uv = temp_uvs[uvIndex - 1];
					glm::vec3 normal = temp_normals[normalIndex - 1];

					vertices.push_back(vertex());

					vertices[i].position = vert;
					vertices[i].texture = uv;
					vertices[i].normal = normal;

				}

				verticesList.push_back(vertices);
				vertices.clear();

			}

			skip = i + 4;

		}

	}

	

	/* Old implementation
	while (std::getline(inFile, line)) {

		if (line.size() > 2) {

			if (line[0] == '#' && line[2] == 'o') {

				temp_vertices.clear();
				temp_uvs.clear();
				temp_normals.clear();
				vertexIndices.clear();
				normalIndices.clear();
				uvIndices.clear();
				usemtl = false;

			}

			if (line[0] == 'v' && line[1] == ' ') {

				std::stringstream ss(line);

				glm::vec3 vertex;

				char v;
				ss >> v;

				ss >> vertex.x;
				ss >> vertex.y;
				ss >> vertex.z;

				temp_vertices.push_back(vertex);

			}

			if (line[0] == 'v' && line[1] == 'n') {

				std::stringstream ss(line);

				glm::vec3 normal;

				char v;
				ss >> v;
				char n;
				ss >> n;

				ss >> normal.x;
				ss >> normal.y;
				ss >> normal.z;

				temp_normals.push_back(normal);

			}

			if (line[0] == 'v' && line[1] == 't') {

				std::stringstream ss(line);

				glm::vec2 uv;

				char v;
				ss >> v;
				char t;
				ss >> t;

				ss >> uv.x;
				ss >> uv.y;

				uv.y = -uv.y;

				temp_uvs.push_back(uv);

			}

			if (line[0] == 'f') {

				std::stringstream ss(line);

				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

				char slash;

				char f;
				ss >> f;

				ss >> vertexIndex[0];
				ss >> slash;
				ss >> uvIndex[0];
				ss >> slash;
				ss >> normalIndex[0];

				ss >> vertexIndex[1];
				ss >> slash;
				ss >> uvIndex[1];
				ss >> slash;
				ss >> normalIndex[1];

				ss >> vertexIndex[2];
				ss >> slash;
				ss >> uvIndex[2];
				ss >> slash;
				ss >> normalIndex[2];

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

			}

			if (line[0] == 'u') {

				usemtl = true;

			}

			if (line[0] == '#' && usemtl) {

				for (unsigned int i = 0; i < vertexIndices.size(); i++) {

					int vertexIndex = vertexIndices[i];
					int uvIndex = uvIndices[i];
					int normalIndex = normalIndices[i];

					glm::vec3 vert = temp_vertices[vertexIndex - 1];
					glm::vec2 uv = temp_uvs[uvIndex - 1];
					glm::vec3 normal = temp_normals[normalIndex - 1];

					vertices.push_back(vertex());

					vertices[i].position = vert;
					vertices[i].texture = uv;
					vertices[i].normal = normal;

				}

				verticesList.push_back(vertices);

				vertices.clear();

				usemtl = false;

			}

		}
		
	}
	*/
	return true;

}
