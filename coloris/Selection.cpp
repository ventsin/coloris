#include "Selection.hpp"

Selection::Selection(sf::FloatRect rect)
{
	setRect(rect);
}

Selection::Selection()
{

}

/*void Selection::move(const sf::Vector2f& offset)
{
	for (size_t i = 0; i < main.getVertexCount(); ++i)
		main[i].position += offset;

	for (size_t i = 0; i < lup.getVertexCount(); ++i)
		lup[i].position += offset;

	for (size_t i = 0; i < rup.getVertexCount(); ++i)
		rup[i].position += offset;

	for (size_t i = 0; i < rdn.getVertexCount(); ++i)
		rdn[i].position += offset;

	for (size_t i = 0; i < ldn.getVertexCount(); ++i)
		ldn[i].position += offset;

	for (size_t i = 0; i < stripes.getVertexCount(); ++i)
		stripes[i].position += offset;
	
	//don't forget to move the stripes, too!
}*/

Selection::Contact Selection::contactTest(sf::Vector2f pos) const
{
	Contact c = Nothing;
	if (processBounds(lup).contains(pos))
		c = LeftUp;
	else if (processBounds(rup).contains(pos))
		c = RightUp;
	else if (processBounds(rdn).contains(pos))
		c = RightDown;
	else if (processBounds(ldn).contains(pos))
		c = LeftDown;
	else if (processBounds(main).contains(pos))
		c = Main;

	return c;
}

void Selection::setRect(sf::FloatRect rect)
{
	float x = 0; //rect.left;
	float y = 0; //rect.top;
	float w = rect.width; //rect.left + rect.width;
	float h = rect.height; //rect.top + rect.height;

	setPosition(rect.left, rect.top);

	main.clear();
	main.setPrimitiveType(sf::LineStrip);
	main.append({ {x, y}, color });
	main.append({ {w, y}, color });
	main.append({ {w, h}, color });
	main.append({ {x, h}, color });
	main.append({ {x, y}, color });

	lup.clear();
	lup.setPrimitiveType(sf::LineStrip);
	lup.append({ {x - margin, y - margin}, color });
	lup.append({ {x + margin, y - margin}, color });
	lup.append({ {x + margin, y + margin}, color });
	lup.append({ {x - margin, y + margin}, color });
	lup.append({ {x - margin, y - margin}, color });

	rup.clear();
	rup.setPrimitiveType(sf::LineStrip);
	rup.append({ {w - margin, y - margin}, color });
	rup.append({ {w + margin, y - margin}, color });
	rup.append({ {w + margin, y + margin}, color });
	rup.append({ {w - margin, y + margin}, color });
	rup.append({ {w - margin, y - margin}, color });

	rdn.clear();
	rdn.setPrimitiveType(sf::LineStrip);
	rdn.append({ { w - margin, h - margin }, color });
	rdn.append({ { w + margin, h - margin }, color });
	rdn.append({ { w + margin, h + margin }, color });
	rdn.append({ { w - margin, h + margin }, color });
	rdn.append({ { w - margin, h - margin }, color });

	ldn.clear();
	ldn.setPrimitiveType(sf::LineStrip);
	ldn.append({ {x - margin, h - margin}, color });
	ldn.append({ {x + margin, h - margin}, color });
	ldn.append({ {x + margin, h + margin}, color });
	ldn.append({ {x - margin, h + margin}, color });
	ldn.append({ {x - margin, h - margin}, color });

	stripes.clear();
	stripes.setPrimitiveType(sf::Lines);

	/*stripes.append(sf::Vertex(sf::Vector2f(rect.left, rect.top), sf::Color::Color(150, 150, 150)));
	stripes.append(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top), sf::Color::Color(150, 150, 150)));
	stripes.append(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), sf::Color::Color(150, 150, 150)));
	stripes.append(sf::Vertex(sf::Vector2f(rect.left, rect.top + rect.height), sf::Color::Color(150, 150, 150)));
	stripes.append(sf::Vertex(sf::Vector2f(rect.left, rect.top), sf::Color::Color(150, 150, 150)));

	stripes.append(sf::Vertex(sf::Vector2f(rect.left - 20.f, rect.top - 20.f), sf::Color::Color(150, 150, 150)));
	stripes.append(sf::Vertex(sf::Vector2f(rect.left + 20.f, rect.top - 20.f), sf::Color::Color(150, 150, 150)));
	stripes.append(sf::Vertex(sf::Vector2f(rect.left + 20.f, rect.top + 20.f), sf::Color::Color(150, 150, 150)));
	stripes.append(sf::Vertex(sf::Vector2f(rect.left - 20.f, rect.top + 20.f), sf::Color::Color(150, 150, 150)));
	stripes.append(sf::Vertex(sf::Vector2f(rect.left - 20.f, rect.top - 20.f), sf::Color::Color(150, 150, 150)));*/


	float step = 6.f;
	//stripes.append(sf::Vertex());
}

sf::FloatRect Selection::getRect() const
{
	return processBounds(main);
}

sf::FloatRect Selection::processBounds(const sf::VertexArray& arr) const
{
	auto bounds = arr.getBounds();
	return sf::FloatRect(getPosition() + sf::Vector2f(bounds.left, bounds.top), { bounds.width, bounds.height });
}
