#pragma once

#include <SFML/System/Vector2.hpp>

namespace cl
{
	class Vector2f : public sf::Vector2f
	{
	public:
		Vector2f() {};
		Vector2f(const sf::Vector2f& _v) { x = _v.x; y = _v.y; };
		Vector2f(const sf::Vector2u& _v) { x = (float)_v.x; y = (float)_v.y; };
		Vector2f(const sf::Vector2i& _v) { x = (float)_v.x; y = (float)_v.y; };
		Vector2f(const float& _x, const float& _y) { x = _x; y = _y; };
		Vector2f(const float& _f) { x = y = _f; };
		//operator cl::Vector2u() const { return sf::Vector2u((unsigned)x, (unsigned)y); };
		//operator cl::Vector2i() const { return sf::Vector2i((int)x, (int)y); };
	};

	class Vector2u : public sf::Vector2u
	{
	public:
		Vector2u() {};
		Vector2u(const sf::Vector2u& _v) { x = _v.x; y = _v.y; };
		Vector2u(const sf::Vector2f& _v) { x = (unsigned)_v.x; y = (unsigned)_v.y; };
		Vector2u(const sf::Vector2i& _v) { x = (unsigned)_v.x; y = (unsigned)_v.y; };
		Vector2u(const unsigned& _x, const unsigned& _y) { x = _x; y = _y; };
		Vector2u(const unsigned& _f) { x = y = _f; };
		//operator cl::Vector2f() const { return sf::Vector2f((float)x, (float)y); };
		//operator cl::Vector2i() const { return sf::Vector2i((int)x, (int)y); };
	};

	class Vector2i : public sf::Vector2i
	{
	public:
		Vector2i() {};
		Vector2i(const sf::Vector2i& _v) { x = _v.x; y = _v.y; };
		Vector2i(const sf::Vector2f& _v) { x = (int)_v.x; y = (int)_v.y; };
		Vector2i(const sf::Vector2u& _v) { x = (int)_v.x; y = (int)_v.y; };
		Vector2i(const unsigned& _x, const unsigned& _y) { x = _x; y = _y; };
		Vector2i(const unsigned& _f) { x = y = _f; };
		//operator cl::Vector2f() const { return sf::Vector2f((float)x, (float)y); };
		//operator cl::Vector2u() const { return sf::Vector2u((unsigned)x, (unsigned)y); };
	};
}