#include "optionFunctions.h"

using namespace RKGBin;

void optionFunctions::displayImages(std::vector<RKGBin::image> images)
{

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	std::vector<SDL_Rect> rects;

	std::vector<SDL_Texture*> textures;

	window = SDL_CreateWindow("RKG Binary Texture Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 10, 10, SDL_WINDOW_BORDERLESS);

	renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Surface* surface;

	for (size_t i = 0; i < images.size(); i++) {

		surface = images[i].surface;
		textures.push_back(SDL_CreateTextureFromSurface(renderer, surface));

		rects.push_back(SDL_Rect());

		rects[i].w = surface->w;
		rects[i].h = surface->h;

	}

	int index = 0;

	std::cout << std::endl << "Use the arrow keys to cycle through the textures" << std::endl << "Press escape to exit texture viewer" << std::endl;
	std::cout << "Now Showing: " << images[index].name << std::endl;

	SDL_Texture* renderTexture = textures[index];

	while (true) {

		while (SDL_PollEvent(&event)) {

			switch (event.type) {

			case SDL_EventType::SDL_QUIT:
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				return;

			case SDL_EventType::SDL_KEYDOWN:
				switch (event.key.keysym.sym) {

				case SDLK_ESCAPE:

					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					return;

				case SDLK_LEFT:
					index--;
					if (index < 0)
						index += textures.size();
					renderTexture = textures[index];
					std::cout << std::endl << "Use the arrow keys to cycle through the textures" << std::endl << "Press escape to exit texture viewer" << std::endl;
					std::cout << "Now Showing: " << images[index].name << std::endl;
					break;

				case SDLK_RIGHT:
					index++;
					if (index >= static_cast<int>(textures.size()))
						index = 0;
					renderTexture = textures[index];
					std::cout << std::endl << "Use the arrow keys to cycle through the textures" << std::endl << "Press escape to exit texture viewer" << std::endl;
					std::cout << "Now Showing: " << images[index].name << std::endl;
					break;

				default:
					break;

				}

			default:
				break;

			}

		}

		SDL_SetWindowSize(window, rects[index].w, rects[index].h);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowTitle(window, std::string("RKG Binary Texture Viewer - " + images[index].name).c_str());

		SDL_SetRenderTarget(renderer, renderTexture);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		SDL_RenderClear(renderer);

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, renderTexture, NULL, NULL);

		SDL_RenderPresent(renderer);

	}

}

void optionFunctions::createFile()
{

	std::vector<RKGBin::image> loadedImages;
	std::vector<RKGBin::sound> loadedSounds;
	std::vector<RKGBin::object3D> loadedObjects;

	while (true) {

		std::cout << std::endl << "Enter" << std::endl;
		std::cout << "0: Cancel and return to main menu" << std::endl;
		std::cout << "1: Add images" << std::endl;
		std::cout << "2: Add sounds" << std::endl;
		std::cout << "3: Add objects" << std::endl;
		std::cout << "Enter nothing to create file" << std::endl;

		std::cout << "Input: ";
		std::string ans;
		std::getline(std::cin, ans);

		if (ans == "0") {

			return;

		}

		if (ans == "1") {

			loadImages(&loadedImages);

		}

		if (ans == "2") {

			loadSounds(&loadedSounds);

		}

		if (ans == "3") {

			loadObjects(&loadedObjects);

		}

		else if (ans == "") {

			std::cout << "Enter a name for your file: ";

			std::cout << "Input: ";
			std::string ans;
			std::getline(std::cin, ans);

			if (loadedImages.size() <= 0 && loadedSounds.size() <= 0 && loadedObjects.size() <= 0) {

				std::cout << std::endl << "File could not be created" << std::endl;
				system("pause");

				std::cout << std::endl;

				return;

			}

			RKGBinary::createFile(ans, loadedImages, loadedSounds, loadedObjects);
			system("pause");

			std::cout << std::endl;
			//Clean up
			for (size_t i = 0; i < loadedImages.size(); i++)
				SDL_FreeSurface(loadedImages[i].surface);

			loadedImages.clear();

			for (size_t i = 0; i < loadedSounds.size(); i++)
				alDeleteBuffers(1, &loadedSounds[i].buffer);

			loadedSounds.clear();

			loadedObjects.clear();

			return;

		}

	}

}
void optionFunctions::read(std::string ans)
{

	std::vector<image> images;
	std::vector<sound> sounds;
	std::vector<object3D> objects;

	RKGBinary::readFile(ans, &images, &sounds, &objects);

	while (true) {

		std::cout << std::endl << "There are " << images.size() << " images in this file" << std::endl;
		std::cout << std::endl << "There are " << sounds.size() << " sounds in this file" << std::endl;
		std::cout << std::endl << "There are " << objects.size() << " objects in this file" << std::endl;

		std::cout << "Enter" << std::endl << "1: List images" << std::endl << "2: Display images" << std::endl << "3: List sounds" << std::endl << "4: Play sounds" << std::endl << "5: List Objects" << std::endl << "Or enter nothing to return" << std::endl;

		std::cout << "Input: ";
		ans = "";
		std::getline(std::cin, ans);

		if (ans == "1") {

			std::cout << std::endl;

			for (size_t i = 0; i < images.size(); i++) {

				std::cout << images[i].name << std::endl;

			}

			//system("pause");

		}

		if (ans == "2") {

			optionFunctions::displayImages(images);

		}

		if (ans == "3") {

			std::cout << std::endl;

			for (size_t i = 0; i < sounds.size(); i++) {

				std::cout << sounds[i].name << std::endl;

			}

			system("pause");

		}

		if (ans == "4") {

			optionFunctions::playSounds(sounds);

		}

		if (ans == "5") {

			std::cout << std::endl;

			for (size_t i = 0; i < objects.size(); i++) {

				std::cout << objects[i].name << std::endl;

				for (size_t j = 0; j < objects[i].verticesList.size(); j++)
					std::cout << "Trimesh " << static_cast<int>(j) + 1 << ": " << objects[i].verticesList[j].size() / 3 << " Triangles" << std::endl;

			}

		}

		if (ans == "") {

			break;

		}

	}

}

void optionFunctions::direct(std::string argv)
{

	ALenum format;
	void* data;
	ALsizei size;
	ALsizei freq;
	ALboolean loop;

	std::string path;
	std::string name;

	for (size_t i = 0; i < argv.size(); i++) {

		if (argv[i] != '"')
			path += argv[i];

	}

	if (path != "") {

		std::vector<image> images;
		std::vector<sound> sounds;
		std::vector<object3D> objects;

		//Find out where the extension starts if it has one
		std::string name; 
		std::string extension;
		
		if (std::filesystem::is_directory(std::filesystem::status(path))) {

			for (const auto& file : std::filesystem::directory_iterator(path)) {

				std::string filePath = file.path().string();

				getExtension(filePath, &name, &extension);

				if (extension == ".jpg" || extension == ".png") {

					std::cout << "Loading " << filePath << std::endl;

					if (SDL_Surface* surface = IMG_Load(filePath.c_str()); surface != nullptr)
						images.push_back(RKGBin::image(surface, name));

					else {

						std::cout << "File could not be read" << std::endl;
						std::cout << std::endl;
						continue;

					}

					std::cout << "File loaded successfully" << std::endl;

				}

				else if (extension == ".wav") {

					std::cout << "Loading " << filePath << std::endl;

					ALuint sndBuffer;

					alGenBuffers(1, &sndBuffer);

					alutLoadWAVFile((ALbyte*)filePath.c_str(), &format, &data, &size, &freq, &loop);

					int maybeSize = size / 8;

					int64_t* dataExtract = (int64_t*)malloc(size);

					int64_t* tempPtr = (int64_t*)data;

					for (int i = 0; i < maybeSize; i++) {

						dataExtract[i] = tempPtr[i];

					}

					sounds.push_back(sound(name, soundInfo(name, format, dataExtract, size, freq, loop), sndBuffer));

					alutUnloadWAV(format, data, size, freq);

					std::cout << "File loaded successfully" << std::endl;

				}

				else if (extension == ".obj") {

					for (const auto& file : std::filesystem::directory_iterator(path)) {

						std::string filePath = file.path().string();

						std::string extension;

						bool start = false;

						for (size_t i = 0; i < filePath.size(); i++) {

							if (filePath[i] == '.')
								start = true;

							if (start)
								extension += filePath[i];

						}

						if (extension == ".obj") {

							std::cout << "Loading " << filePath << std::endl;

							std::vector<std::vector<HARIKEN::vertex>> verticesList_ = HARIKEN::objLoader(filePath.c_str()).verticesList;

							if (verticesList_.size() <= 0) {

								std::string name;

								for (size_t i = 0; i < filePath.size(); i++) {

									name += filePath[i];

									if (filePath[i] == '\/' || filePath[i] == '\\')
										name = "";

								}

								objects.push_back(object3D(name, verticesList_));

							}

							else {

								std::cout << "File could not be read" << std::endl;
								std::cout << std::endl;
								continue;

							}

							std::cout << "File loaded successfully" << std::endl;

						}

					}

				}

			}

		}

		else {

			getExtension(path, &name, &extension);

			if (extension == ".jpg" || extension == ".png") {

				std::cout << "Loading " << path << std::endl;

				if (SDL_Surface* surface = IMG_Load(path.c_str()); surface != nullptr)
					images.push_back(RKGBin::image(surface, name));

				else {

					std::cout << "File could not be read" << std::endl;
					std::cout << std::endl;
					return;

				}

				std::cout << "File loaded successfully" << std::endl;

			}

			else if (extension == ".wav") {

				std::cout << "Loading " << path << std::endl;

				ALuint sndBuffer;

				alGenBuffers(1, &sndBuffer);

				alutLoadWAVFile((ALbyte*)path.c_str(), &format, &data, &size, &freq, &loop);

				int maybeSize = size / 8;

				int64_t* dataExtract = (int64_t*)malloc(size);

				int64_t* tempPtr = (int64_t*)data;

				for (int i = 0; i < maybeSize; i++) {

					dataExtract[i] = tempPtr[i];

				}

				sounds.push_back(sound(name, soundInfo(name, format, dataExtract, size, freq, loop), sndBuffer));

				alutUnloadWAV(format, data, size, freq);

			}

			else if (extension == ".obj") {

				std::cout << "Loading " << std::endl;

				std::vector<std::vector<HARIKEN::vertex>> verticesList_ = HARIKEN::objLoader(path.c_str()).verticesList;

				if (verticesList_.size() >= 0) {

					objects.push_back(object3D(name, verticesList_));

				}

				else {

					std::cout << "File could not be read" << std::endl;
					std::cout << std::endl;
					return;

				}

				std::cout << "File loaded successfully" << std::endl;

			}

			else if (extension == ".rkg") {

				read(path);
				return;

			}

		}

		std::cout << std::endl << "Creating Binary File" << std::endl;
		RKGBinary::createFile(name, images, sounds, objects);

	}

}

void optionFunctions::playSounds(std::vector<sound> sounds)
{

	std::cout << std::endl << "Use the arrow keys to cycle through the sounds" << std::endl << "Press escape to exit sound player" << std::endl;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	SDL_Rect rects;

	SDL_Texture* textures;

	window = SDL_CreateWindow("RKG Binary Sound Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 10, 10, SDL_WINDOW_BORDERLESS);

	ALfloat position[3] = {

		0.0f, 0.0f, 1.0f

	};

	alListenerfv(AL_POSITION, position);

	renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Surface* surface;

	surface = IMAGE_ASSETS[0].surface;
	textures = SDL_CreateTextureFromSurface(renderer, surface);

	rects.w = surface->w;
	rects.h = surface->h;

	int index = 0;

	std::cout << "Now Playing: " << sounds[index].name << std::endl;

	SDL_Texture* renderTexture = textures;
	
	ALuint source;

	alGenSources(1, &source);

	alSourcei(source, AL_BUFFER, sounds[index].buffer);

	alSourcePlay(source);

	while (true) {

		while (SDL_PollEvent(&event)) {

			switch (event.type) {

			case SDL_EventType::SDL_QUIT:
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				alSourceStop(source);
				alDeleteSources(1, &source);
				return;

			case SDL_EventType::SDL_KEYDOWN:
				switch (event.key.keysym.sym) {

				case SDLK_ESCAPE:
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					alSourceStop(source);
					alDeleteSources(1, &source);
					return;

				case SDLK_LEFT:
					index--;
					if (index < 0)
						index = sounds.size() - 1;
					alSourceStop(source);
					alSourcei(source, AL_BUFFER, sounds[index].buffer);
					alSourcePlay(source);
					std::cout << std::endl << "Use the arrow keys to cycle through the sounds" << std::endl << "Press escape to exit sound player" << std::endl;
					std::cout << "Now Playing: " << sounds[index].name << std::endl;
					break;

				case SDLK_RIGHT:
					index++;
					if (index >= static_cast<int>(sounds.size()))
						index = 0;
					alSourceStop(source);
					alSourcei(source, AL_BUFFER, sounds[index].buffer);
					alSourcePlay(source);
					std::cout << std::endl << "Use the arrow keys to cycle through the sounds" << std::endl << "Press escape to exit sound player" << std::endl;
					std::cout << "Now Playing: " << sounds[index].name << std::endl;
					break;

				default:
					break;

				}

			default:
				break;

			}

		}

		SDL_SetWindowSize(window, rects.w, rects.h);
		//SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowTitle(window, std::string("RKG Binary Sound Player - " + sounds[index].name).c_str());

		SDL_SetRenderTarget(renderer, renderTexture);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		SDL_RenderClear(renderer);

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, renderTexture, NULL, NULL);

		SDL_RenderPresent(renderer);

	}


}

void optionFunctions::loadAssets()
{

	RKGBinary::readFile(EXECUTABLE_LOCATION + "Assets/MusicNote.rkg", &IMAGE_ASSETS, nullptr, nullptr);
	if (IMAGE_ASSETS.size() == 0) {

		std::cout << "Failed to load required assets" << std::endl;
		SDL_Quit();
		exit(0);

	}

}

void optionFunctions::loadImages(std::vector<RKGBin::image>* images)
{

	while (true) {

		std::cout << std::endl << "Enter the file path to the file or folder you wish to load" << std::endl;
		std::cout << "Enter nothing to finish loading images" << std::endl;

		std::cout << "Input: ";
		std::string ans;
		std::getline(std::cin, ans);

		std::string path;

		for (size_t i = 0; i < ans.size(); i++) {

			if (ans[i] != '"')
				path += ans[i];

		}

		if (path != "") {
			//Find out where the extension starts if it has one
			std::string name;
			std::string extension;

			if (!std::filesystem::is_directory(std::filesystem::status(path))) {

				getExtension(path, &name, &extension);

				if (extension != ".jpg" && extension != ".png") {

					std::cout << "Image must be a .jpg or .png format" << std::endl << std::endl;
					continue;

				}

				if (SDL_Surface* image_ = IMG_Load(path.c_str()); image_ != nullptr)
					images->push_back(RKGBin::image(IMG_Load(path.c_str()), name));

				else {

					std::cout << "File could not be read " << SDL_Error << std::endl;
					system("pause");
					std::cout << std::endl;
					continue;

				}

				std::cout << "File loaded successfully" << std::endl;

			}

			else {

				for (const auto& file : std::filesystem::directory_iterator(path)) {

					std::string filePath = file.path().string();

					getExtension(filePath, &name, &extension);

					if (extension == ".jpg" || extension == ".png") {

						std::cout << "Loading " << filePath << std::endl;

						if (SDL_Surface* image_ = IMG_Load(filePath.c_str()); image_ != nullptr)
							images->push_back(RKGBin::image(image_, name));

						else {

							std::cout << "File could not be read" << std::endl;
							std::cout << std::endl;
							continue;

						}

						std::cout << "File loaded successfully" << std::endl;

					}

				}

			}

		}

		else 
			return;

	}

}

void optionFunctions::loadSounds(std::vector<sound>* sounds)
{

	ALenum format;
	void* data;
	ALsizei size;
	ALsizei freq;
	ALboolean loop;

	while (true) {

		std::cout << std::endl << "Enter the file path to the file or folder you wish to load" << std::endl;
		std::cout << "Enter nothing to finish loading sounds" << std::endl;

		std::cout << "Input: ";
		std::string ans;
		std::getline(std::cin, ans);

		std::string path;

		for (size_t i = 0; i < ans.size(); i++) {

			if (ans[i] != '"')
				path += ans[i];

		}

		if (path != "") {
			//Find out where the extension starts if it has one
			std::string name;
			std::string extension;

			if (!std::filesystem::is_directory(std::filesystem::status(path))) {

				getExtension(path, &name, &extension); 
				
				if (extension != ".wav") {

					std::cout << "File must be in a signed 16 bit PCM .wav format" << std::endl << std::endl;
					continue;

				}

				ALuint sndBuffer;

				alGenBuffers(1, &sndBuffer);

				alutLoadWAVFile((ALbyte*)path.c_str(), &format, &data, &size, &freq, &loop);

				int maybeSize = size / 8;

				int64_t* dataExtract = (int64_t*)malloc(size);

				int64_t* tempPtr = (int64_t*)data;

				for (int i = 0; i < maybeSize; i++) {

					dataExtract[i] = tempPtr[i];

				}

				sounds->push_back(sound(name, soundInfo(name, format, dataExtract, size, freq, loop), sndBuffer));

				alutUnloadWAV(format, data, size, freq);

			}

			else {

				for (const auto& file : std::filesystem::directory_iterator(path)) {

					std::string filePath = file.path().string();

					getExtension(path, &name, &extension);

					if (extension == ".wav") {

						std::cout << "Loading " << filePath << std::endl;

						std::string name;

						for (size_t i = 0; i < filePath.size(); i++) {

							name += filePath[i];

							if (filePath[i] == '\/' || filePath[i] == '\\')
								name = "";

						}

						ALuint sndBuffer;

						alGenBuffers(1, &sndBuffer);

						alutLoadWAVFile((ALbyte*)filePath.c_str(), &format, &data, &size, &freq, &loop);

						int maybeSize = size / 8;

						int64_t* dataExtract = (int64_t*)malloc(size);

						int64_t* tempPtr = (int64_t*)data;

						for (int i = 0; i < maybeSize; i++) {

							dataExtract[i] = tempPtr[i];

						}

						sounds->push_back(sound(name, soundInfo(name, format, dataExtract, size, freq, loop), sndBuffer));

						alutUnloadWAV(format, data, size, freq);

					}

				}

			}

		}

		else
			return;

	}

}

void optionFunctions::loadObjects(std::vector<RKGBin::object3D>* objects)
{

	while (true) {

		std::cout << std::endl << "Enter the file path to the file or folder you wish to load" << std::endl;
		std::cout << "Enter nothing to finish loading objects" << std::endl;

		std::cout << "Input: ";
		std::string ans;
		std::getline(std::cin, ans);

		std::string path;

		for (size_t i = 0; i < ans.size(); i++) {

			if (ans[i] != '"')
				path += ans[i];

		}

		if (path != "") {
			//Find out where the extension starts if it has one
			std::string name;
			std::string extension;

			if (!std::filesystem::is_directory(std::filesystem::status(path))) {

				getExtension(path, &name, &extension);

				if (extension != ".obj") {

					std::cout << "Object must be in a .obj format" << std::endl << std::endl;
					continue;

				}

				std::vector<std::vector<HARIKEN::vertex>> verticesList_;

				if (verticesList_ = HARIKEN::objLoader(path.c_str()).verticesList; verticesList_.size() > 0) {

					std::string name;

					for (size_t i = 0; i < ans.size(); i++) {

						name += path[i];

						if (path[i] == '\/' || path[i] == '\\')
							name = "";

					}

					objects->push_back(object3D(name, verticesList_));

				}

				else {

					std::cout << "File could not be read " << SDL_Error << std::endl;
					system("pause");
					std::cout << std::endl;
					continue;

				}

				std::cout << "File loaded successfully" << std::endl;

			}

			else {

				for (const auto& file : std::filesystem::directory_iterator(path)) {

					std::string filePath = file.path().string();

					std::string extension;

					bool start = false;

					for (size_t i = 0; i < filePath.size(); i++) {

						if (filePath[i] == '.')
							start = true;

						if (start)
							extension += filePath[i];

					}

					if (extension == ".obj") {

						std::cout << "Loading " << filePath << std::endl;

						std::vector<std::vector<HARIKEN::vertex>> verticesList_ = HARIKEN::objLoader(filePath.c_str()).verticesList;

						if (verticesList_.size() <= 0) {

							std::string name;

							for (size_t i = 0; i < filePath.size(); i++) {

								name += filePath[i];

								if (filePath[i] == '\/' || filePath[i] == '\\')
									name = "";

							}

							objects->push_back(object3D(name, verticesList_));

						}

						else {

							std::cout << "File could not be read" << std::endl;
							std::cout << std::endl;
							continue;

						}

						std::cout << "File loaded successfully" << std::endl;

					}

				}

			}

		}

		else
			return;

	}

}

void optionFunctions::getExtension(std::string path, std::string* name_, std::string* extension_)
{

	int counter = 1;
	std::string extension;
	std::string name;

	for (size_t i = path.size() - 1; i >= 0; i--) {

		if (path[i] != ' ') {

			if (path[i] == '.') {

				for (size_t j = i; j < i + counter; j++)
					extension += path[j];

				for (size_t j = i - 1; j >= 0; j--) {

					if (path[j] == '/' || path[j] == '\\')
						break;

					name = path[j] + name;
				}

				*name_ = name;
				*extension_ = extension;
				break;

			}

			counter++;

		}

		if (counter >= 5)
			break;

	}

}
