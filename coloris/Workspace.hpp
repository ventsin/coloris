#pragma once

#include <nana/gui.hpp>
#include <SFML/Graphics.hpp>

class Workspace
{
public:
	void setup(nana::window, nana::rectangle r);

	void draw();

private:
	sf::RenderWindow render;
	std::shared_ptr<nana::nested_form> form;
};