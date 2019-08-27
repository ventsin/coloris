#include "TransformAdjusts.hpp"

#include "Layer.hpp"

TransformAdjusts::TransformAdjusts()
{
}

TransformAdjusts::~TransformAdjusts()
{
}

void TransformAdjusts::create(nana::window wd, int _x, int _y)
{
	x = _x;
	y = _y;

	x_label.create(wd, { x + 10, y + 10, 32, 20 });
	x_label.caption("X:");
	x_label.scheme().foreground = nana::color(230, 230, 230);
	x_label.transparent(true);

	y_label.create(wd, { x + 110, y + 10, 32, 20 });
	y_label.caption("Y:");
	y_label.scheme().foreground = nana::color(230, 230, 230);
	y_label.transparent(true);

	width_label.create(wd, { x + 10, y + 40, 32, 20 });
	width_label.caption("W:");
	width_label.scheme().foreground = nana::color(230, 230, 230);
	width_label.transparent(true);

	height_label.create(wd, { x + 110, y + 40, 32, 20 });
	height_label.caption("H:");
	height_label.scheme().foreground = nana::color(230, 230, 230);
	height_label.transparent(true);

	x_text.create(wd, { x + 25, y + 10 - 2, 60, 20 });
	x_text.scheme().activated = nana::color(218, 165, 32);
	x_text.fgcolor({ 230, 230, 230 });
	x_text.bgcolor({ 30, 30, 30 });
	x_text.multi_lines(false);
	x_text.set_accept([&](wchar_t wc)
		{
			if (!x_text.caption().empty())
				return iswdigit(wc) || wc == nana::keyboard::backspace;
			else
				return iswdigit(wc) || wc == nana::keyboard::backspace || wc == L'-';
		});
		
	y_text.create(wd, { x + 125, y + 10 - 2, 60, 20 });
	y_text.scheme().activated = nana::color(218, 165, 32);
	y_text.fgcolor({ 230, 230, 230 });
	y_text.bgcolor({ 30, 30, 30 });
	y_text.multi_lines(false);
	y_text.set_accept([&](wchar_t wc)
		{
			if (!y_text.caption().empty())
				return iswdigit(wc) || wc == nana::keyboard::backspace;
			else
				return iswdigit(wc) || wc == nana::keyboard::backspace || wc == L'-';
		});

	width_text.create(wd, { x + 25, y + 40 - 2, 60, 20 });
	width_text.scheme().activated = nana::color(218, 165, 32);
	width_text.fgcolor({ 230, 230, 230 });
	width_text.bgcolor({ 30, 30, 30 });
	width_text.multi_lines(false);
	width_text.set_accept([&](wchar_t wc)
		{
			return iswdigit(wc) || wc == nana::keyboard::backspace;
		});

	height_text.create(wd, { x + 125, y + 40 - 2, 60, 20 });
	height_text.scheme().activated = nana::color(218, 165, 32);
	height_text.fgcolor({ 230, 230, 230 });
	height_text.bgcolor({ 30, 30, 30 });
	height_text.multi_lines(false);
	height_text.set_accept([&](wchar_t wc)
		{
			return iswdigit(wc) || wc == nana::keyboard::backspace;
		});

	x_text.events().text_changed(std::bind(&TransformAdjusts::updateLayer, this, std::placeholders::_1));
	y_text.events().text_changed(std::bind(&TransformAdjusts::updateLayer, this, std::placeholders::_1));
	width_text.events().text_changed(std::bind(&TransformAdjusts::updateLayer, this, std::placeholders::_1));
	height_text.events().text_changed(std::bind(&TransformAdjusts::updateLayer, this, std::placeholders::_1));

	enabled(false);
}

void TransformAdjusts::move(int _x, int _y)
{
	x = _x;
	y = _y;

	x_label.move({ x + 10, y + 10, 32, 20 });
	y_label.move({ x + 110, y + 10, 32, 20 });
	width_label.move({ x + 10, y + 40, 32, 20 });
	height_label.move({ x + 110, y + 40, 32, 20 });
	x_text.move({ x + 25, y + 10 - 2, 60, 20 });
	y_text.move({ x + 125, y + 10 - 2, 60, 20 });
	width_text.move({ x + 25, y + 40 - 2, 60, 20 });
	height_text.move({ x + 125, y + 40 - 2, 60, 20 });
}

void TransformAdjusts::enabled(bool enabled)
{
	if (enabled)
	{
		x_label.show();
		y_label.show();
		width_label.show();
		height_label.show();
		x_text.show();
		y_text.show();
		width_text.show();
		height_text.show();
	}
	else
	{
		x_label.hide();
		y_label.hide();
		width_label.hide();
		height_label.hide();
		x_text.hide();
		y_text.hide();
		width_text.hide();
		height_text.hide();
	}
}

bool TransformAdjusts::enabled() const
{
	return is_enabled;
}

void TransformAdjusts::setLayer(Layer* _l)
{
	if (_l)
	{
		layer = _l;
		updateValues();
	}
}

void TransformAdjusts::updateValues()
{
	cl::Vector2i pos = layer->getPosition();
	auto size = layer->getSize();
	
	x_text.caption(std::to_string(pos.x));
	y_text.caption(std::to_string(pos.y));
	width_text.caption(std::to_string(size.x));
	height_text.caption(std::to_string(size.y));
}

void TransformAdjusts::updateLayer(const nana::arg_textbox&)
{
	try
	{
		int x = x_text.to_int();
		int y = y_text.to_int();
		int w = width_text.to_int();
		int h = height_text.to_int();
		layer->resizeTo({ x, y, w, h });
		layer->update_selection = true;
	}
	catch (...) {}
}