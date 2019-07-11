/*
 * ImageWrite.hpp - This file's purpose is to encode RAW RGBA data
 * into png, jpg, bmp and tga buffers.
 *
 * This file will (likely) be either upgraded or discontinued when
 * the SFML Team merges our pull request: https://github.com/SFML/SFML/pull/1597
 */

#pragma once

#include <string>
#include <vector>

namespace cl
{
	struct RGBA
	{
		const unsigned char* inBuffer = nullptr;
		const size_t& width = 0;
		const size_t& height = 0;
	};

	bool saveToMemory(const std::string& format, const RGBA& image, std::vector<unsigned char>& buffer);
}