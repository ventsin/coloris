#pragma once

#include <SFML/Graphics.hpp>

class Selection : public sf::Drawable, public sf::Transformable
{
public:
	enum Contact
	{
		Nothing = 0,
		LeftUp,
		RightUp,
		RightDown,
		LeftDown,
		Main
	};
	
	Selection();
	Selection(sf::FloatRect rect);

	bool visible = false;
	bool debug_draw = false;

	//void move(const sf::Vector2f& offset);

	Contact contactTest(sf::Vector2f pos) const;

	void setRect(sf::FloatRect);
	sf::FloatRect getRect() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (visible)
		{
			states.transform *= getTransform();
			target.draw(main, states);
			target.draw(lup, states);
			target.draw(rup, states);
			target.draw(ldn, states);
			target.draw(rdn, states);
			target.draw(stripes, states);

			/*if (debug_draw)
			{
				//target.draw(lup, states);
			}*/
		}
	}

	sf::FloatRect processBounds(const sf::VertexArray&) const;

	sf::Color color = sf::Color(200, 200, 200);
	float margin = 15.f;

	sf::VertexArray stripes, main, lup, rup, ldn, rdn;
};