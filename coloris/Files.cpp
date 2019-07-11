/*
 * Files.cpp - Load and Save files as std::string buffers.
 */

#include "Files.hpp"

#include <fstream>
#include <sstream>

#include <vector>

#include <SFML/Config.hpp>

std::string getFileBinary(std::string file)
{
	std::ifstream input(file, std::ios::binary);
	if (input.is_open())
	{
		std::ostringstream s;
		s << input.rdbuf();
		return s.str();
	}
	else
		return "";
}

void setFileBinary(std::string file, const std::string& content)
{
	std::ofstream output(file, std::ios::binary);
	if (output.is_open())
		output.write(content.data(), content.size());
}

void setFileBinary(std::string file, const std::vector<sf::Uint8>& content)
{
	std::ofstream output(file, std::ios::binary);
	if (output.is_open())
		output.write(reinterpret_cast<const char*>(content.data()), content.size());
}
