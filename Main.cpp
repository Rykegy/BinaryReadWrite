//Binary read and writer for working with Binary files created for the HARIKEN Engine
//Created by Jonathan Leung

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#undef main

#include "optionFunctions.h"

int main(int argc, char* argv[]) {

	std::cout << "RKG Binary suite " << VERSION << " by Jonathan Leung" << std::endl;
	//Init required libraries
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "SDL video init failed: %s\n", SDL_GetError());
		return 1;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0) {

		fprintf(stderr, "SDL image init failed: %s\n", SDL_GetError());
		return 1;

	}

	alutInit(0, NULL);
	alGetError();
	//Find out where the program is being run
	{

		char basePath[255] = "";
		_fullpath(basePath, argv[0], sizeof(basePath));

		std::string fullPath = basePath;

		std::string name;

		for (size_t i = 0; i < fullPath.size(); i++) {

			name += fullPath[i];

			if (fullPath[i] == '\/' || fullPath[i] == '\\')
				name = "";

		}

		int pathSize = fullPath.size() - name.size();

		std::string path;
		for (int i = 0; i < pathSize; i++)
			path += fullPath[i];

		EXECUTABLE_LOCATION = path;

	}
	//Load required assets (Music Note)
	optionFunctions::loadAssets();
	//If program is executed with flags (Direct input)
	if (argc == 2) {

		std::cout << std::endl << "Processing: " << argv[1] << std::endl;
		optionFunctions::direct(argv[1]);
		SDL_Quit();
		system("pause");
		exit(0);

	}

	if (argc > 2) {

		std::cout << "Too many arguments: " << std::endl;
		
		SDL_Quit();
		exit(0);

	}

	while (true) {

		std::cout << "Enter" << std::endl << "1: Create new file" << std::endl << "2: Read existing file" << std::endl << "Or enter nothing to exit" << std::endl;

		std::cout << "Input: ";
		std::string ans;
		std::getline(std::cin, ans);

		if (ans == "1") {

			optionFunctions::createFile();

		}

		else if (ans == "2") {

			std::cout << "Enter the path to the file you wish to open: ";

			std::string ans;
			std::getline(std::cin, ans);

			optionFunctions::read(ans);

			std::cout << std::endl;

		}

		else if (ans == "") {

			break;

		}

		else {

			std::cout << std::endl << "Input not recognized" << std::endl << std::endl;

		}

	}


	SDL_Quit();

}