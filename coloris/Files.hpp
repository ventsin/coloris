#pragma once
/*
 * Files.hpp - Load and Save files as std::string buffers.
 */

#include <string>
#include <vector>

#include <SFML/Config.hpp>

std::string getFileBinary(std::string file);
void setFileBinary(std::string file, const std::string& content);
void setFileBinary(std::string file, const std::vector<sf::Uint8>& content);