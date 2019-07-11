#include "Workspace.hpp"

#include <Windows.h>

void Workspace::setup(nana::window wd, nana::rectangle r)
{
	form = std::make_shared<nana::nested_form>(nana::nested_form(wd, r));
	//form->activate();
	//auto &f = *form;
	//f["render"]. ;
	//panel.create(wd, r, true);

	//Create SFML window
	/*render.create(CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		r.x,
		r.y,
		r.width,
		r.height,
		(reinterpret_cast<HWND>(form->native_handle())), NULL, GetModuleHandle(NULL), NULL));*/
	render.create(reinterpret_cast<HWND>(form->native_handle()));
}

void Workspace::draw()
{
	render.clear({ 0, 128, 128, 255 });
	render.display();
}
