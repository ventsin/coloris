/*
 * This file is meant to be an implementation of a simple Icon class
 * to spare ourselves some work with WINDOWS icons.
 * THIS IMPLEMENTATION IS HEAVILY BASED ON SFML'S SETICON
 */

#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include <nana/gui.hpp>

class Icon
{
public:
	Icon(Icon const&) = delete;
	void operator=(Icon const&) = delete;
	~Icon();
	
	static Icon& get()
	{
		static Icon m_icon;
		return m_icon;
	};

	bool setIcon(unsigned width, unsigned height, const unsigned char* pixels);
	void applyIcon(nana::native_window_type win);

private:
	Icon() {};

	HICON icon = 0;
};