#include "MouseStatus.hpp"

#include "Selection.hpp"

cl::MouseStatus::Status cl::MouseStatus::beginMovement(sf::Vector2f pos, Selection* sel)
{
	origin = result = pos;

	if (sel)
	{
		contact = sel->contactTest(pos);

		switch (contact)
		{
		case Selection::Contact::Nothing:
			status = Pan;
			break;
		case Selection::Contact::Main:
			status = Move;
			break;
		default:
			status = Resize;
			break;
		}
	}
	else
		status = Pan;

	return status;
}

void cl::MouseStatus::endMovement(sf::Vector2f pos)
{
	result = pos;
	status = Nothing;
	contact = 0;
}

void cl::MouseStatus::update(cl::Vector2f posi)
{
	result = posi;
}

sf::Vector2f cl::MouseStatus::getMovement() const
{
	return result - origin;
}

std::pair<sf::Vector2f, sf::Vector2f> cl::MouseStatus::getPositions() const
{
	return std::make_pair(origin, result);
}
