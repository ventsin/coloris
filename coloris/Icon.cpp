#include "Icon.hpp"

Icon::~Icon()
{
	if (icon)
		DestroyIcon(icon);
}

bool Icon::setIcon(unsigned width, unsigned height, const unsigned char* pixels)
{
	// First destroy the previous one
	if (icon)
		DestroyIcon(icon);

	// Windows wants BGRA pixels: swap red and blue channels
	std::vector<unsigned char> iconPixels(width * height * 4);
	for (std::size_t i = 0; i < iconPixels.size() / 4; ++i)
	{
		iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
		iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
		iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
		iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
	}

	// Create the icon from the pixel array
	icon = CreateIcon(GetModuleHandleW(NULL), width, height, 1, 32, NULL, &iconPixels[0]);

	return !!icon;
}

void Icon::applyIcon(nana::native_window_type win)
{
	// Set it as both big and small icon of the window
	if (icon)
	{
		SendMessageW(reinterpret_cast<HWND>(win), WM_SETICON, ICON_BIG, (LPARAM)icon);
		SendMessageW(reinterpret_cast<HWND>(win), WM_SETICON, ICON_SMALL, (LPARAM)icon);
	}
}
