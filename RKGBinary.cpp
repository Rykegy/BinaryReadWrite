#include "RKGBinary.h"

std::string EXECUTABLE_LOCATION = "";
std::vector<RKGBin::image> IMAGE_ASSETS;
std::vector<RKGBin::sound> SOUND_ASSETS;

using namespace RKGBin;

void RKGBin::RKGBinary::createFile(std::string fileName, std::vector<RKGBin::image> images, std::vector<RKGBin::sound> sounds, std::vector<RKGBin::object3D> objects)
{

	versionHeader version = versionHeader();

	file temp(fileName);
	temp.imageAmount = static_cast<int>(images.size());
	temp.soundAmount = static_cast<int>(sounds.size());
	temp.objectAmount = static_cast<int>(objects.size());

	FILE* fptr;

	std::string filePath = EXECUTABLE_LOCATION + "Output/" + fileName + ".rkg";

	if (errno_t error = fopen_s(&fptr, filePath.c_str(), "wb") != 0) {
		std::cout << "fopen error: " << error << std::endl;
		std::cout << "Could not create " << filePath << std::endl;
		return;
	}

	char HeadCheck[10] = "RKGBINARY";

	fwrite(&HeadCheck, sizeof(char), 10, fptr);
	fwrite(&version, sizeof(versionHeader), 1, fptr);
	fwrite(&temp, sizeof(file), 1, fptr);

	for (size_t i = 0; i < images.size(); i++) {

		SDL_Surface* surface = images[i].surface;
		imageBinary tempImage = imageBinary(images[i].surface, images[i].name);

		fwrite(&tempImage, sizeof(imageBinary), 1, fptr);

		for (int j = 0; j < tempImage.h * tempImage.w; j++) {

			Uint32 p = getpixel(surface, j, surface->format->BytesPerPixel);
			fwrite(&p, sizeof(Uint32), 1, fptr);

		}

	}

	for (size_t i = 0; i < sounds.size(); i++) {

		soundInfo* tempInfo = &sounds[i].info;

		fwrite(&tempInfo, sizeof(soundInfo), 1, fptr);

		for (int j = 0; j < tempInfo->size / 8; j++) {

			fwrite(&tempInfo->data[j], sizeof(int64_t), 1, fptr);

		}

		alDeleteBuffers(1, &sounds[i].buffer);

	}

	for (size_t i = 0; i < objects.size(); i++) {

		object3DInfo tempInfo = object3DInfo(objects[i]);

		fwrite(&tempInfo, sizeof(object3DInfo), 1, fptr);

		for (int j = 0; j < 100; j++) {

			if (tempInfo.verticesListSize[j] <= 0)
				break;

			for (int k = 0; k < tempInfo.verticesListSize[j]; k++)
				fwrite(&objects[i].verticesList[j][k], sizeof(HARIKEN::vertex), 1, fptr);

		}

	}

	fclose(fptr);

	std::cout << "File created successfully" << std::endl;

}
/* Test function
void RKGBin::RKGBinary::createFileTest(std::string fileName, std::vector<RKGBin::image> images, std::vector<RKGBin::sound> sounds)
{

	file temp(fileName);
	temp.imageAmount = images.size();
	temp.soundAmount = sounds.size();
	
	FILE* fptr;

	std::string filePath = EXECUTABLE_LOCATION + "Output/" + fileName + ".rkg";

	if (errno_t error = fopen_s(&fptr, filePath.c_str(), "wb") != 0) {
		std::cout << "fopen error: " << error << std::endl;
		std::cout << "Could not create " << filePath << std::endl;
		return;
	}
	
	else
		fwrite(&temp, sizeof(file), 1, fptr);
	
	for (size_t i = 0; i < images.size(); i++) {

		SDL_Surface* surface = images[i].surface;
		imageBinary tempImage = imageBinary(images[i].surface, images[i].name);

		fwrite(&tempImage, sizeof(imageBinary), 1, fptr);

		for (int i = 0; i < tempImage.h * tempImage.w; i++) {

			Uint32 p = getpixel(surface, i, surface->format->BytesPerPixel);
			fwrite(&p, sizeof(Uint32), 1, fptr);

		}

	}

	for (size_t i = 0; i < sounds.size(); i++) {

		soundInfo tempInfo = sounds[i].info;

		fwrite(&tempInfo, sizeof(soundInfo), 1, fptr);

		for (int i = 0; i < tempInfo.size / 8; i++) {

			int64_t data = tempInfo.data[i];
			fwrite(&data, sizeof(int64_t), 1, fptr);

		}

		alDeleteBuffers(1, &sounds[i].buffer);

	}

	fclose(fptr);
	 
	std::cout << "File created successfully" << std::endl;

}
*/

versionHeader RKGBin::RKGBinary::readVersion(std::string filePath, FILE** filePointer)
{

	FILE* fptr = *filePointer;

	std::string path;

	for (size_t i = 0; i < filePath.size(); i++) {

		if (filePath[i] != '"')
			path += filePath[i];

	}

	if (path == "") {

		std::cout << "File cannot be opened" << std::endl;
		return versionHeader(false);

	}

	versionHeader version;

	try {

		fopen_s(&fptr, path.c_str(), "rb");

	}

	catch (errno_t error) {

		std::cout << "File cannot be opened" << std::endl;
		std::cout << error << std::endl;

		return versionHeader(false);

	}
	
	char check[10];
	char checkPhrase[10] = "RKGBINARY";
	bool checkMatch = true;

	fread(&check, sizeof(char), 10, fptr);

	for (int i = 0; i < 10; i++) {

		if (check[i] != checkPhrase[i]) {

			checkMatch = false;
			break;

		}

	}

	if (!checkMatch) {

		rewind(fptr);
		*filePointer = fptr;
		return versionHeader(true);

	}

	fread(&version, sizeof(versionHeader), 1, fptr);

	*filePointer = fptr;

	return version;

}

fileVerOne RKGBin::RKGBinary::readHeaderVerOne(std::string filePath, FILE** filePointer)
{

	FILE* fptr = *filePointer;
	
	std::string path;

	for (size_t i = 0; i < filePath.size(); i++) {

		if (filePath[i] != '"')
			path += filePath[i];

	}

	fopen_s(&fptr, path.c_str(), "rb");
	
	fileVerOne header;

	fread(&header, sizeof(fileVerOne), 1, fptr);

	*filePointer = fptr;

	return header;

}

file RKGBin::RKGBinary::readHeader(FILE** filePointer)
{

	FILE* fptr = *filePointer;

	file header;

	fread(&header, sizeof(file), 1, fptr);

	*filePointer = fptr;

	return header;

}

void RKGBin::RKGBinary::readImages(file header, FILE** filePointer, std::vector<image>* images)
{

	FILE* fptr = *filePointer;

	imageBinary tempImg;

	for (int i = 0; i < header.imageAmount; i++) {

		fread(&tempImg, sizeof(imageBinary), 1, fptr);

		imageBinary img = imageBinary(SDL_CreateRGBSurface(0, tempImg.w, tempImg.h, tempImg.format.BitsPerPixel, tempImg.format.Rmask, tempImg.format.Gmask, tempImg.format.Bmask, tempImg.format.Amask), tempImg.name);

		SDL_Surface* surface = createSurface(img);

		SDL_UnlockSurface(surface);

		for (int i = 0; i < tempImg.h * tempImg.w; i++) {

			Uint32 p;
			fread(&p, sizeof(Uint32), 1, fptr);

			putpixel(surface, i, img.format.BytesPerPixel, p);

		}

		SDL_LockSurface(surface);

		if (images != nullptr)
			images->push_back(image(surface, img.name));

	}

	*filePointer = fptr;

}

void RKGBin::RKGBinary::readSounds(file header, FILE** filePointer, std::vector<sound>* sounds)
{

	FILE* fptr = *filePointer;

	soundInfo tempSnd;

	for (int i = 0; i < header.soundAmount; i++) {

		fread(&tempSnd, sizeof(soundInfo), 1, fptr);

		int64_t* data = (int64_t*)malloc(tempSnd.size);

		for (int j = 0; j < tempSnd.size / 8; j++) {

			fread(&data[j], sizeof(int64_t), 1, fptr);

		}

		soundInfo info = soundInfo(tempSnd.name, tempSnd.format, data, tempSnd.size, tempSnd.freq, tempSnd.loop);

		ALuint sndBuffer;

		alGenBuffers(1, &sndBuffer);

		alBufferData(sndBuffer, tempSnd.format, data, tempSnd.size, tempSnd.freq);

		if (sounds != nullptr)
			sounds->push_back(sound(info.name, info, sndBuffer));

	}

	*filePointer = fptr;

}

void RKGBin::RKGBinary::readObjects(file header, FILE** filePointer, std::vector<object3D>* objects)
{

	FILE* fptr = *filePointer;

	object3DInfo tempInfo;

	for (int i = 0; i < header.objectAmount; i++) {

		fread(&tempInfo, sizeof(object3DInfo), 1, fptr);

		object3D tempObject = object3D(tempInfo.name);

		for (int j = 0; j < 100; j++) {

			if (tempInfo.verticesListSize[j] <= 0)
				break;

			std::vector<HARIKEN::vertex> tempVertices;

			HARIKEN::vertex tempVert;

			for (int k = 0; k < tempInfo.verticesListSize[j]; k++) {

				fread(&tempVert, sizeof(HARIKEN::vertex), 1, fptr);

				tempVertices.push_back(tempVert);

			}

			tempObject.verticesList.push_back(tempVertices);

		}

		objects->push_back(tempObject);

	}

	*filePointer = fptr;

}

void RKGBin::RKGBinary::readAssets(versionHeader version, file header, FILE** filePointer, std::vector<image>* images, std::vector<sound>* sounds, std::vector<object3D>* objects)
{

	readImages(header, filePointer, images);

	readSounds(header, filePointer, sounds);

	if (version.versionNumber < 2.0f)
		return;

	readObjects(header, filePointer, objects);

}

void RKGBin::RKGBinary::readFile(std::string filePath, std::vector<image>* images, std::vector<sound>* sounds, std::vector<object3D>* objects)
{

	FILE* fptr = new FILE();

	versionHeader version = readVersion(filePath, &fptr);

	file header;

	if (version.versionNumber < 2.0) {

		fileVerOne headerVerOne = readHeaderVerOne(filePath, &fptr);

		header = file(headerVerOne);

	}

	else {

		header = readHeader(&fptr);

	}

	readAssets(version, header, &fptr, images, sounds, objects);

	fclose(fptr);
	
}

Uint32 RKGBin::RKGBinary::getpixel(SDL_Surface* surface, int i, int bpp)
{

	Uint8* p = (Uint8*)surface->pixels + i * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16*)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32*)p;
		break;

	default:
		return 0;
	}

}

void RKGBin::RKGBinary::putpixel(SDL_Surface* surface, int i, int bpp, Uint32 pixel)
{

	Uint8* p = (Uint8*)surface->pixels + i * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16*)p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32*)p = pixel;
		break;
	}

}

SDL_Surface* RKGBin::RKGBinary::createSurface(imageBinary img)
{

	SDL_Surface* surface = SDL_CreateRGBSurface(0, img.w, img.h, img.format.BitsPerPixel, img.format.Rmask, img.format.Gmask, img.format.Bmask, img.format.Amask);

	surface->pitch = img.pitch;

	return surface;

}

