#include "VertexControls.hpp"

#include "Layer.hpp"

VertexControls::VertexControls()
{
}

VertexControls::~VertexControls()
{
}

void VertexControls::create(nana::window wd, int _x, int _y)
{
	x = _x;
	y = _y;
	drawing = std::make_unique<nana::drawing>(wd);

	spin.create(wd, { x + 10, y + 220, 180, 25 });
	spin.scheme().activated = nana::color(218, 165, 32);
	spin.scheme().background = nana::color(30, 30, 30);
	spin.scheme().foreground = nana::color(230, 230, 230);
	//spin.enable_focus_color(false);
	spin.caption("Vertex");
	spin.range({
		"Top-Left",
		"Top-Right",
		"Bottom-Right",
		"Bottom-Left" });
	spin.events().text_changed([&](const nana::arg_spinbox& arg)
		{
			updateSliders();
		});

	red_slider.create(wd, { x + 10, y + 10, 20, 200 });
	red_slider.maximum(255);
	red_slider.vertical(true);
	red_slider.transparent(true);
	red_slider.scheme().color_adorn = nana::color(180, 0, 0);
	red_slider.scheme().color_slider_highlighted = nana::color(180, 0, 0);

	red_text.create(wd, { x + 190 - 23, y + 110, 23, 20 });
	red_text.scheme().activated = nana::color(218, 165, 32);
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
	green_text.scheme().activated = nana::color(218, 165, 32);
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
	blue_text.scheme().activated = nana::color(218, 165, 32);
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
	alpha_text.scheme().activated = nana::color(218, 165, 32);
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

	red_slider.events().value_changed(std::bind(&VertexControls::updateFromSlider, this, std::placeholders::_1));
	green_slider.events().value_changed(std::bind(&VertexControls::updateFromSlider, this, std::placeholders::_1));
	blue_slider.events().value_changed(std::bind(&VertexControls::updateFromSlider, this, std::placeholders::_1));
	alpha_slider.events().value_changed(std::bind(&VertexControls::updateFromSlider, this, std::placeholders::_1));

	red_text.events().text_changed(std::bind(&VertexControls::updateFromText, this, std::placeholders::_1));
	green_text.events().text_changed(std::bind(&VertexControls::updateFromText, this, std::placeholders::_1));
	blue_text.events().text_changed(std::bind(&VertexControls::updateFromText, this, std::placeholders::_1));
	alpha_text.events().text_changed(std::bind(&VertexControls::updateFromText, this, std::placeholders::_1));

	enabled(false);
}

void VertexControls::move(int _x, int _y)
{
	x = _x;
	y = _y;

	spin.move({ x + 10, y + 220, 180, 25 });

	red_slider.move({ x + 10, y + 10, 20, 200 });
	red_text.move({ x + 190 - 23, y + 110, 23, 20 });
	red_label.move({ x + 190 - 60, y + 110 + 1, 32, 20 });
	green_slider.move({ x + 30 + 1, y + 10, 20, 200 });
	green_text.move({ x + 190 - 23, y + 130 + 1, 23, 20 });
	green_label.move({ x + 190 - 60, y + 130 + 1 + 1, 32, 20 });
	blue_slider.move({ x + 50 + 2, y + 10, 20, 200 });
	blue_text.move({ x + 190 - 23, y + 150 + 2, 23, 20 });
	blue_label.move({ x + 190 - 60, y + 150 + 2 + 1, 32, 20 });
	alpha_slider.move({ x + 70 + 3, y + 10, 20, 200 });
	alpha_text.move({ x + 190 - 23, y + 170 + 3, 23, 20 });
	alpha_label.move({ x + 190 - 60, y + 170 + 3 + 1, 32, 20 });

	updateColor();
}

void VertexControls::enabled(bool enabled)
{
	is_enabled = enabled;
	//alpha_label.transparent(true);

	if (enabled)
	{
		spin.show();

		red_slider.show();
		green_slider.show();
		blue_slider.show();
		alpha_slider.show();

		red_text.show();
		green_text.show();
		blue_text.show();
		alpha_text.show();

		red_label.show();
		green_label.show();
		blue_label.show();
		alpha_label.show();

		drawing->draw([&](nana::paint::graphics& graph) {
			graph.rectangle({ x + 100, y + 10, 90, 90 }, true, color);
			});
		drawing->update();
	}
	else
	{
		spin.hide();

		red_slider.hide();
		green_slider.hide();
		blue_slider.hide();
		alpha_slider.hide();

		red_text.hide();
		green_text.hide();
		blue_text.hide();
		alpha_text.hide();

		red_label.hide();
		green_label.hide();
		blue_label.hide();
		alpha_label.hide();

		drawing->clear();
		drawing->update();
	}
}

bool VertexControls::enabled()
{
	return is_enabled;
}

void VertexControls::setLayer(Layer* _l) 
{
	layer = _l;
	updateSliders();
}

int VertexControls::getCorner() const
{
	const std::string s = spin.value();

	if (s == "Top-Left")
		return 0;
	if (s == "Top-Right")
		return 1;
	if (s == "Bottom-Right")
		return 2;
	if (s == "Bottom-Left")
		return 3;

	return -1;
}

void VertexControls::updateFromText(const nana::arg_textbox& arg)
{
	if (is_busy)
		return;

	is_busy = true;
	red_slider.value(red_text.to_int());
	green_slider.value(green_text.to_int());
	blue_slider.value(blue_text.to_int());
	alpha_slider.value(alpha_text.to_int());

	sf::Color c;
	c.r = red_text.to_int();
	c.g = green_text.to_int();
	c.b = blue_text.to_int();
	c.a = alpha_text.to_int();

	VColor vc = layer->getColors();
	vc[getCorner()] = c;

	layer->setColors(vc);

	updateColor();
	is_busy = false;
}

void VertexControls::updateFromSlider(const nana::arg_slider& arg)
{
	if (is_busy)
		return;

	is_busy = true;
	red_text.caption(std::to_string(red_slider.value()));
	green_text.caption(std::to_string(green_slider.value()));
	blue_text.caption(std::to_string(blue_slider.value()));
	alpha_text.caption(std::to_string(alpha_slider.value()));

	sf::Color c;
	c.r = red_slider.value();
	c.g = green_slider.value();
	c.b = blue_slider.value();
	c.a = alpha_slider.value();

	VColor vc = layer->getColors();
	vc[getCorner()] = c;

	layer->setColors(vc);

	updateColor();
	is_busy = false;
}

void VertexControls::updateColor()
{
	color = nana::color(red_slider.value(), green_slider.value(), blue_slider.value(), alpha_slider.value() / 255.0);
	drawing->update();
}

void VertexControls::updateSliders()
{
	if (!layer)
		return;

	is_busy = true;
	sf::Color color = layer->getColors()[getCorner()];
	red_slider.value(color.r);
	green_slider.value(color.g);
	blue_slider.value(color.b);
	alpha_slider.value(color.a);

	red_text.caption(std::to_string(red_slider.value()));
	green_text.caption(std::to_string(green_slider.value()));
	blue_text.caption(std::to_string(blue_slider.value()));
	alpha_text.caption(std::to_string(alpha_slider.value()));

	updateColor();

	is_busy = false;
}