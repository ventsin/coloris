/*
 * ImageWrite.hpp - This file's purpose is to encode RAW RGBA data
 * into png, jpg, bmp and tga buffers.
 *
 * This file will (likely) be either upgraded or discontinued when
 * the SFML Team merges our pull request: https://github.com/SFML/SFML/pull/1597
 */

#include "ImageWrite.hpp"

#include <iostream>
#include <algorithm>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

void bufferFromCallback(void* context, void* data, int size)
{
	unsigned char* ptr = static_cast<unsigned char*>(data);
	std::copy(ptr, ptr + size, std::back_inserter(*static_cast<std::vector<unsigned char>*>(context)));
}

bool cl::saveToMemory(const std::string& format, const RGBA& image, std::vector<unsigned char>& buffer)
{
	// Make sure the image is not empty
	if (image.inBuffer && (image.width > 0) && (image.height > 0))
	{
		// Choose function based on format
		std::string specified;
		for (auto i = format.begin(); i != format.end(); ++i)
			specified.push_back(static_cast<char>(std::tolower(*i)));

		if (specified == "bmp")
		{
			// BMP format
			if (stbi_write_bmp_to_func(&bufferFromCallback, &buffer, image.width, image.height, 4, image.inBuffer))
				return true;
		}
		else if (specified == "tga")
		{
			// TGA format
			if (stbi_write_tga_to_func(&bufferFromCallback, &buffer, image.width, image.height, 4, image.inBuffer))
				return true;
		}
		else if (specified == "png")
		{
			// PNG format
			if (stbi_write_png_to_func(&bufferFromCallback, &buffer, image.width, image.height, 4, image.inBuffer, 0))
				return true;
		}
		else if (specified == "jpg" || specified == "jpeg")
		{
			// JPG format
			if (stbi_write_jpg_to_func(&bufferFromCallback, &buffer, image.width, image.height, 4, image.inBuffer, 90))
				return true;
		}
	}
	return false;
}
