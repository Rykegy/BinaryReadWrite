#pragma once

#include <filesystem>

#include "RKGBinary.h"
#include "objLoader.h"

class optionFunctions
{

public:

	static void displayImages(std::vector<RKGBin::image> filePath);
	static void createFile();

	static void read(std::string path);

	static void direct(std::string argv);

	static void playSounds(std::vector<RKGBin::sound> sounds);

	static void loadAssets();

private:
	static void loadImages(std::vector<RKGBin::image>* images);
	static void loadSounds(std::vector<RKGBin::sound>* sounds);
	static void loadObjects(std::vector<RKGBin::object3D>* objects);

	static void getExtension(std::string path, std::string* name, std::string* extension);

};

