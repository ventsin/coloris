#include "ResourceManager.hpp"

T ResourceManager::loadTexture(std::string file)
{
	for (auto& t : textures)
	{
		if (t.first == file)
			return t.second;
	}
	
	auto p = std::make_shared<sf::Texture>();
	if (p->loadFromFile(file))
	{
		textures.push_back(std::make_pair(file, p));
		return p;
	}

	return nullptr;
}

F ResourceManager::loadFont(std::string file)
{
	for (auto& f : fonts)
	{
		if (f.first == file)
			return f.second;
	}

	auto p = std::make_shared<sf::Font>();
	if (p->loadFromFile(file))
	{
		fonts.push_back(std::make_pair(file, p));
		return p;
	}

	return nullptr;
}

std::string ResourceManager::getPath(T t)
{
	auto it = std::find_if(textures.begin(), textures.end(), [&t](const Texture& texture) -> bool { return texture.second == t; });
	if (it != textures.end())
		return it->first;
	return "";
}

std::string ResourceManager::getPath(F f)
{
	auto it = std::find_if(fonts.begin(), fonts.end(), [&f](const Font& font) -> bool { return font.second == f; });

	if (it != fonts.end())
		return it->first;
	return "";
}

void ResourceManager::update()
{
	for (auto it = textures.begin(); it != textures.end();)
	{
		if (it->second.use_count() <= 1)
		{
			it = textures.erase(it);
		}
		else
			it++;
	}

	for (auto it = fonts.begin(); it != fonts.end();)
	{
		if (it->second.use_count() <= 1)
		{
			it = fonts.erase(it);
		}
		else
			it++;
	}
}

std::string ResourceManager::toString() const
{
	std::string s = "";
	for (auto& t : textures)
	{
		s += "Path: ";
		s += t.first;
		s += "; UseCount: ";
		s += std::to_string(t.second.use_count());
		s += ".\n";
	}

	for (auto& f : fonts)
	{
		s += "Path: ";
		s += f.first;
		s += "; UseCount: ";
		s += std::to_string(f.second.use_count());
		s += ".\n";
	}
	return s;
}
