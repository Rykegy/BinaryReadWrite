#pragma once
#include <vector>
#include <glm.hpp>
#include <string> 
#include <sstream> 
#include<fstream>
#include "Mesh.h"

namespace HARIKEN {

	class objLoader
	{
	public:
		~objLoader();

		std::vector<vertex> vertices;
		std::vector<std::vector<vertex>> verticesList;

			/// C11 precautions 
		objLoader(const objLoader&) = delete;  /// Copy constructor
		objLoader(objLoader&&) = delete;       /// Copy operator
		objLoader& operator=(const objLoader&) = delete; /// Move constructor
		objLoader& operator=(objLoader&&) = delete;      /// Move operator

		objLoader(const char* path);
		bool loadObj(const char* path);
	};

}