#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/checkbox.hpp>

class RadioGroup
{
public:
	RadioGroup();
	~RadioGroup();

	void create(nana::window wd, int x, int y);
	void move(int x, int y);

	void enabled(bool);
	bool enabled();

private:
	int x = 0;
	int y = 0;

	bool is_enabled = false;

	nana::checkbox buttons[4];
};