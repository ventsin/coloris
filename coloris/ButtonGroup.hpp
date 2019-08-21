#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/picture.hpp>

class ButtonGroup
{
public:
	ButtonGroup();
	~ButtonGroup();

	void create(nana::window wd, int x, int y);
	void move(int x, int y);

	nana::picture* getElement(unsigned short index);

private:
	nana::picture pics[8];
};