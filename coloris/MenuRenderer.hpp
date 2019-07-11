#pragma once

#include <string>

#include <nana/gui.hpp>
#include <nana/system/platform.hpp>
#include <nana/gui/widgets/menu.hpp>
#include <nana/paint/text_renderer.hpp>

class renderer : public nana::menu::renderer_interface
{
	using color_rgb = ::nana::color_rgb;
public:
	renderer(const nana::pat::cloneable<renderer_interface>& rd)
		: reuse_(rd) //Reuses the current renderer
	{}
private:
	void background(graph_reference graph, nana::window wd) override
	{
		graph.rectangle(true, nana::color::color(50, 50, 50));
		graph.rectangle(false, nana::color::color(218, 165, 32));
	}

	void item(graph_reference graph, const nana::rectangle& r, const attr & atr) override
	{
		if (state::active == atr.item_state)
			graph.rectangle(r, true, nana::color::color(90, 90, 90));
	}

	void item_image(graph_reference graph, const nana::point& pos, unsigned image_px, const nana::paint::image& img) override
	{
		reuse_->item_image(graph, pos, image_px, img);
	}

	void item_text(graph_reference graph, const nana::point& pos, const std::string& text, unsigned pixels, const attr& atr) override
	{
		//reuse_->item_text(graph, pos, text, pixels, atr);
		graph.palette(true, atr.enabled ? nana::color::color(230, 230, 230) : nana::colors::gray_border);
		nana::paint::text_renderer tr(graph);

		auto wstr = std::wstring(::nana::charset(text, ::nana::unicode::utf8));//nana::to_wstring(text);
		tr.render(pos, wstr.c_str(), wstr.length(), pixels, true);
	}

	void sub_arrow(graph_reference graph, const nana::point& pos, unsigned pixels, const attr & atr) override
	{
		reuse_->sub_arrow(graph, pos, pixels, atr);
	}
private:
	nana::pat::cloneable<renderer_interface> reuse_;
};

/*
#include <Windows.h>

class renderer : public nana::menu::renderer_interface
{
	using color_rgb = ::nana::color_rgb;
public:
	renderer(const nana::pat::cloneable<renderer_interface>& rd)
		: reuse_(rd) //Reuses the current renderer
	{}
private:
	void background(graph_reference graph, nana::window wd) override
	{
		graph.rectangle(true, nana::colors::white);
		graph.rectangle(false, static_cast<color_rgb>(0x5DC1AC));

		//Makes the menu transparent, it only works under Windows with #include <windows.h>
		HWND native = reinterpret_cast<HWND>(nana::API::root(wd));
		DWORD ex_style = ::GetWindowLong(native, GWL_EXSTYLE);
		::SetWindowLong(native, GWL_EXSTYLE, ex_style | 0x00080000 /*WS_EX_LAYERED);

		using slwa_t = BOOL(WINAPI*)(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
		slwa_t slwa = reinterpret_cast<slwa_t>(::GetProcAddress(::GetModuleHandleA("User32.DLL"), "SetLayeredWindowAttributes"));
		if (slwa)
			slwa(native, 0, 220, 0x00000002/*LWA_ALPHA);
	}

	void item(graph_reference graph, const nana::rectangle& r, const attr & atr) override
	{
		if (state::active == atr.item_state)
			graph.rectangle(r, true, static_cast<color_rgb>(0x9ADCCA));
	}

	void item_image(graph_reference graph, const nana::point& pos, unsigned image_px, const nana::paint::image& img) override
	{
		reuse_->item_image(graph, pos, image_px, img);
	}

	void item_text(graph_reference graph, const nana::point& pos, const std::string& text, unsigned pixels, const attr& atr) override
	{
		reuse_->item_text(graph, pos, text, pixels, atr);
	}

	void sub_arrow(graph_reference graph, const nana::point& pos, unsigned pixels, const attr & atr) override
	{
		reuse_->sub_arrow(graph, pos, pixels, atr);
	}
private:
	nana::pat::cloneable<renderer_interface> reuse_;
};
*/