/*
 * MouseStatus.hpp - Class designed to determine what the current Mouse Operation is.
 * Possible operations include: Nothing, Moving, Panning and Resizing.
 */

#pragma once

#include <SFML/System/Vector2.hpp>

#include <utility>

class Selection;

namespace cl
{

	class MouseStatus
	{
	public:
		enum Status
		{
			Nothing = 0,
			Pan,
			Move,
			Resize
		};
		
		Status beginMovement(sf::Vector2f pos, Selection* sel);
		void endMovement(sf::Vector2f pos);
		
		sf::Vector2f getMovement() const;

		std::pair<sf::Vector2f, sf::Vector2f> getPositions() const;

		short getContact() const
		{
			return contact;
		}

		Status getOperation() const
		{
			return status;
		};

	private:
		Status status = Nothing;

		short contact = 0;

		sf::Vector2f origin;
		sf::Vector2f result;
	};
}