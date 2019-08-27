#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>

class Layer;

class TransformAdjusts
{
public:
	TransformAdjusts();
	~TransformAdjusts();

	void create(nana::window wd, int x, int y);
	void move(int x, int y);

	void enabled(bool);
	bool enabled() const;

	void setLayer(Layer*);

private:
	void updateValues();
	void updateLayer(const nana::arg_textbox&);

	int x = 0;
	int y = 0;

	bool is_enabled = false;
	
	Layer* layer = nullptr;

	nana::label x_label;
	nana::label y_label;
	nana::label width_label;
	nana::label height_label;
	
	nana::textbox x_text;
	nana::textbox y_text;
	nana::textbox width_text;
	nana::textbox height_text;
};