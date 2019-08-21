#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/checkbox.hpp>

class SliderGroup
{
public:
	SliderGroup();
	~SliderGroup();

	void create(nana::window wd, int x, int y);
	void move(int x, int y);

	void enabled(bool);
	bool enabled();

	//nana::picture* getElement(unsigned short index);

private:
	void updateFromText(const nana::arg_textbox& arg);
	void updateFromSlider(const nana::arg_slider& arg);
	void updateColor();

	int x = 0;
	int y = 0;

	bool is_enabled = false;
	bool is_busy = false;

	nana::color color;
	std::unique_ptr<nana::drawing> drawing;

	nana::label red_label;
	nana::label green_label;
	nana::label blue_label;
	nana::label alpha_label;

	nana::slider red_slider;
	nana::slider green_slider;
	nana::slider blue_slider;
	nana::slider alpha_slider;

	nana::textbox red_text;
	nana::textbox green_text;
	nana::textbox blue_text;
	nana::textbox alpha_text;
};