#pragma once

//include standard library headers
#include <vector>
#include <map>

//include SFML headers
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

typedef std::shared_ptr<sf::Texture> T;
typedef std::shared_ptr<sf::Font> F;
typedef std::pair<std::string, T> Texture;
typedef std::pair<std::string, F> Font;

class ResourceManager
{
public:
	T loadTexture(std::string file);
	T loadTextureFromImage(const sf::Image& img);
	F loadFont(std::string file);

	std::string getPath(T);
	std::string getPath(F);

	void update();

	std::string toString() const;

private:
	std::vector<Texture> textures;
	std::vector<Font> fonts;
};