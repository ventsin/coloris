#include "SliderGroup.hpp"

SliderGroup::SliderGroup()
{
}

SliderGroup::~SliderGroup()
{
}

void SliderGroup::create(nana::window wd, int _x, int _y)
{
	x = _x;
	y = _y;
	drawing = std::make_unique<nana::drawing>(wd);

	red_slider.create(wd, { x + 10, y + 10, 20, 200 });
	red_slider.maximum(255);
	red_slider.vertical(true);
	red_slider.transparent(true);
	red_slider.scheme().color_adorn = nana::color(180, 0, 0);
	red_slider.scheme().color_slider_highlighted = nana::color(180, 0, 0);

	red_text.create(wd, { x + 190 - 23, y + 110, 23, 20 });
	red_text.fgcolor({ 230, 230, 230 });
	red_text.bgcolor({ 30, 30, 30 });
	red_text.multi_lines(false);
	red_text.set_accept([](wchar_t wc)
		{
			return iswdigit(wc) || wc == nana::keyboard::backspace;
		});

	red_label.create(wd, { x + 190 - 60, y + 110 + 1, 32, 20 });
	red_label.caption("Red");
	red_label.scheme().foreground = nana::color(230, 230, 230);
	red_label.transparent(true);

	green_slider.create(wd, { x + 30 + 1, y + 10, 20, 200 });
	green_slider.maximum(255);
	green_slider.vertical(true);
	green_slider.transparent(true);
	green_slider.scheme().color_adorn = nana::color(0, 180, 0);
	green_slider.scheme().color_slider_highlighted = nana::color(0, 180, 0);

	green_text.create(wd, { x + 190 - 23, y + 130 + 1, 23, 20 });
	green_text.fgcolor({ 230, 230, 230 });
	green_text.bgcolor({ 30, 30, 30 });
	green_text.multi_lines(false);
	green_text.set_accept([](wchar_t wc)
		{
			return iswdigit(wc) || wc == nana::keyboard::backspace;
		});

	green_label.create(wd, { x + 190 - 60, y + 130 + 1 + 1, 32, 20 });
	green_label.caption("Green");
	green_label.scheme().foreground = nana::color(230, 230, 230);
	green_label.transparent(true);

	blue_slider.create(wd, { x + 50 + 2, y + 10, 20, 200 });
	blue_slider.maximum(255);
	blue_slider.vertical(true);
	blue_slider.transparent(true);
	blue_slider.scheme().color_adorn = nana::color(0, 0, 180);
	blue_slider.scheme().color_slider_highlighted = nana::color(0, 0, 180);

	blue_text.create(wd, { x + 190 - 23, y + 150 + 2, 23, 20 });
	blue_text.fgcolor({ 230, 230, 230 });
	blue_text.bgcolor({ 30, 30, 30 });
	blue_text.multi_lines(false);
	blue_text.set_accept([](wchar_t wc)
		{
			return iswdigit(wc) || wc == nana::keyboard::backspace;
		});

	blue_label.create(wd, { x + 190 - 60, y + 150 + 2 + 1, 32, 20 });
	blue_label.caption("Blue");
	blue_label.scheme().foreground = nana::color(230, 230, 230);
	blue_label.transparent(true);

	alpha_slider.create(wd, { x + 70 + 3, y + 10, 20, 200 });
	alpha_slider.maximum(255);
	alpha_slider.vertical(true);
	alpha_slider.transparent(true);
	alpha_slider.scheme().color_adorn = nana::color(180, 180, 180);
	alpha_slider.scheme().color_slider_highlighted = nana::color(180, 180, 180);

	alpha_text.create(wd, { x + 190 - 23, y + 170 + 3, 23, 20 });
	alpha_text.fgcolor({ 230, 230, 230 });
	alpha_text.bgcolor({ 30, 30, 30 });
	alpha_text.multi_lines(false);
	alpha_text.set_accept([](wchar_t wc)
		{
			return iswdigit(wc) || wc == nana::keyboard::backspace;
		});

	alpha_label.create(wd, { x + 190 - 60, y + 170 + 3 + 1, 32, 20 });
	alpha_label.caption("Alpha");
	alpha_label.scheme().foreground = nana::color(230, 230, 230);
	alpha_label.transparent(true);

	red_slider.events().value_changed(std::bind(&SliderGroup::updateFromSlider, this, std::placeholders::_1));
	green_slider.events().value_changed(std::bind(&SliderGroup::updateFromSlider, this, std::placeholders::_1));
	blue_slider.events().value_changed(std::bind(&SliderGroup::updateFromSlider, this, std::placeholders::_1));
	alpha_slider.events().value_changed(std::bind(&SliderGroup::updateFromSlider, this, std::placeholders::_1));

	red_text.events().text_changed(std::bind(&SliderGroup::updateFromText, this, std::placeholders::_1));
	green_text.events().text_changed(std::bind(&SliderGroup::updateFromText, this, std::placeholders::_1));
	blue_text.events().text_changed(std::bind(&SliderGroup::updateFromText, this, std::placeholders::_1));
	alpha_text.events().text_changed(std::bind(&SliderGroup::updateFromText, this, std::placeholders::_1));

	drawing->draw([&](nana::paint::graphics& graph) {
			graph.rectangle({ x + 100, y + 10, 90, 90 }, true, color);
		});
}

void SliderGroup::move(int x, int y)
{
}

void SliderGroup::updateFromText(const nana::arg_textbox& arg)
{
	if (is_busy)
		return;

	is_busy = true;
	red_slider.value(red_text.to_int());
	green_slider.value(green_text.to_int());
	blue_slider.value(blue_text.to_int());
	alpha_slider.value(alpha_text.to_int());

	updateColor();
	is_busy = false;
}

void SliderGroup::updateFromSlider(const nana::arg_slider& arg)
{
	if (is_busy)
		return;

	is_busy = true;
	red_text.caption(std::to_string(red_slider.value()));
	green_text.caption(std::to_string(green_slider.value()));
	blue_text.caption(std::to_string(blue_slider.value()));
	alpha_text.caption(std::to_string(alpha_slider.value()));

	updateColor();
	is_busy = false;
}

void SliderGroup::updateColor()
{
	color = nana::color(red_slider.value(), green_slider.value(), blue_slider.value(), alpha_slider.value() / 255.0);
	drawing->update();
}
