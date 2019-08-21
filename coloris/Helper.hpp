#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <random>

//generate random color
sf::Color generateColor()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255); // distribution in range [0, 255]

	return sf::Color::Color(dist(rng), dist(rng), dist(rng));
}

unsigned generateNumber()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1346123);
	return dist(rng);
}

sf::Vector2f operator* (sf::Vector2u v, float f)
{
	return { (float)v.x * f, (float)v.y * f };
}