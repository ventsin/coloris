#pragma once

#include <SFML/Graphics.hpp>

class Background : public sf::Drawable, public sf::Transformable
{
public:
	Background();
	//Background(sf::FloatRect rect);

	bool visible = false;

	//void move(const sf::Vector2f& offset);

	void setColor(sf::Color, sf::Color);

	void setSize(sf::Vector2u size);
	sf::FloatRect getRect() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible)
		{
			states.transform *= getTransform();
			sf::RenderTexture tex;
			tex.create(size.x, size.y);
			sf::Sprite spr;
			spr.setTexture(tex.getTexture());
			tex.draw(sprite, states);
			target.draw(spr);
		}
	}

	sf::Vector2u size;

	sf::Sprite sprite;
	sf::Texture texture;
};