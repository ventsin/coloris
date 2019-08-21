#include "ButtonGroup.hpp"

ButtonGroup::ButtonGroup()
{
}

ButtonGroup::~ButtonGroup()
{
}

void ButtonGroup::create(nana::window wd, int x, int y)
{
	nana::rectangle rect{ x, y, 25, 25 };
	for (auto& p : pics)
	{
		p.create(wd, rect);
		p.transparent(true);
		rect.x += 25;
	}

	pics[0].load(nana::paint::image("resources/icons/enabled/newLayer.png"));
	pics[1].load(nana::paint::image("resources/icons/enabled/descend.png"));
	pics[2].load(nana::paint::image("resources/icons/enabled/ascend.png"));
	pics[3].load(nana::paint::image("resources/icons/enabled/duplicate.png"));
	pics[4].load(nana::paint::image("resources/icons/enabled/delete.png"));
	pics[5].load(nana::paint::image("resources/icons/enabled/flipHorizontal.png"));
	pics[6].load(nana::paint::image("resources/icons/enabled/flipVertical.png"));
	pics[7].load(nana::paint::image("resources/icons/enabled/rasterize.png"));
}

void ButtonGroup::move(int x, int y)
{
	nana::rectangle rect{ x, y, 25, 25 };
	for (auto& p : pics)
	{
		p.move(rect);
		rect.x += 25;
	}
}

nana::picture* ButtonGroup::getElement(unsigned short index)
{
	if (index > 7)
		return nullptr;
	else
		return &pics[index];
}
