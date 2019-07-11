#include "Background.hpp"

Background::Background()
{
	sprite.setTexture(texture);
}

void Background::setColor(sf::Color a, sf::Color b)
{
	sf::Image image;
	image.create(20, 20);

	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
			image.setPixel(i, j, a);

	for (int i = 10; i < 20; ++i)
		for (int j = 0; j < 10; ++j)
			image.setPixel(i, j, b);

	for (int i = 0; i < 10; ++i)
		for (int j = 10; j < 20; ++j)
			image.setPixel(i, j, b);

	for (int i = 10; i < 20; ++i)
		for (int j = 10; j < 20; ++j)
			image.setPixel(i, j, a);

	texture.loadFromImage(image);
	texture.setRepeated(true);
}

void Background::setSize(sf::Vector2u _size)
{
	size = _size;
	sprite.setTextureRect({ 0, 0, (int)_size.x, (int)_size.y });
}

sf::FloatRect Background::getRect() const
{
	return sf::FloatRect(getPosition(), { (float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height });
}