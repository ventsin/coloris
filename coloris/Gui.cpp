#include "Gui.hpp"

#include <Windows.h>

#include <nana/gui/filebox.hpp>
#include <nana/gui/msgbox.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::experimental::filesystem;

#include "Selection.hpp"
#include "Actions.hpp"

#include "RichText.hpp"
#include "MenuRenderer.hpp"

#include "Files.hpp"
#include "Helper.hpp"

#include "ImageWrite.hpp"

#include <cppcodec/base64_rfc4648.hpp>
using base64 = cppcodec::base64_rfc4648;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

VColor generateColors();

ui::Coloris::Coloris(unsigned int _x, unsigned int _y, std::string _title)
{
	//Configure window
	caption(_title);
	enable_dropfiles(true);
	size(nana::size(_x, _y));
	bgcolor(nana::color(37, 37, 37));
	nana::API::track_window_size(*this, { 800, 600 }, false);

	attemptDebugMode();

	//Load our Bilinear Interpolation shader
	if (!shader.loadFromFile("vert.glsl", "frag.glsl"))
		return;

	//Generate a new icon every run
	generateIcon();

	//Create Widgets
	menubar.create(*this);
	statusbar.create(*this, nana::rectangle(0, size().height - 20, size().width, 20), true);
	panel.create(*this, nana::rectangle(size().width - 200, size().height - 200 - statusbar.size().height, 200, 200), { 0, 0, 200, 200 });
	list.create(panel, nana::rectangle(0, 0, 200, 200));
	group.create(*this, size().width - 200, size().height - 225 - statusbar.size().height);//size().width - 200, panel.pos().y - 25);
	sliders.create(*this, size().width - 200, menubar.size().height);
	adjusts.create(*this, size().width - 200, menubar.size().height + 250);

	setupInterface();
	setupEvents();

	//Create SFML work
	work.create(CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		0,
		menubar.size().height,
		size().width - 200,
		size().height - statusbar.size().height - menubar.size().height,
		(reinterpret_cast<HWND>(native_handle())), NULL, GetModuleHandle(NULL), NULL));

	//work.setFramerateLimit(0);
	selection.debug_draw = true;
	//Make sure our transparency background has some good colors
	background.setColor(sf::Color(120, 120, 120), sf::Color(75, 75, 75));

	//nana timer
	timer.elapse([&] () { draw(); });
	timer.interval(std::chrono::milliseconds{ 1 });
	timer.start();
}

ui::Coloris::~Coloris()
{
}

void ui::Coloris::attemptDebugMode()
{
	std::ifstream f("debug");
	if (f.is_open())
		debug = true;
}

void ui::Coloris::generateIcon()
{
	float x = 32.f, y = 32.f;
	VColor c = generateColors();
	std::vector<sf::Uint8> d;
	sf::RenderTexture t;
	sf::VertexArray v;
	sf::Image i;

	t.create((unsigned)x, (unsigned)y);

	v.setPrimitiveType(sf::Quads);
	v.append(sf::Vertex({ 0.f, 0.f }, c.a, { 0.f, 0.f }));
	v.append(sf::Vertex({ x, 0.f }, c.b, { 1.f, 0.f }));
	v.append(sf::Vertex({ x, y }, c.c, { 1.f, 1.f }));
	v.append(sf::Vertex({ 0.f, y }, c.d, { 0.f, 1.f }));

	shader.setUniform("color0", sf::Glsl::Vec4(c.a));
	shader.setUniform("color1", sf::Glsl::Vec4(c.b));
	shader.setUniform("color3", sf::Glsl::Vec4(c.c));
	shader.setUniform("color2", sf::Glsl::Vec4(c.d));

	t.clear(sf::Color::Green);
	t.draw(v, &shader);
	t.display();
	i = t.getTexture().copyToImage();

	//cl::saveToMemory("bmp", { i.getPixelsPtr(), i.getSize().x, i.getSize().y }, d);
	//icon.open(d.data(), d.size());
	//nana::API::window_icon(this->handle(), icon);

	Icon::get().setIcon(i.getSize().x, i.getSize().y, i.getPixelsPtr());
	Icon::get().applyIcon(this->native_handle());
}

void ui::Coloris::setupInterface()
{
	//Configure schemes and behaviors
	menubar.scheme().text_fgcolor = nana::color(230, 230, 230);
	menubar.scheme().body_highlight = nana::color(100, 100, 100);
	menubar.scheme().body_selected = nana::color(50, 50, 50);
	menubar.scheme().background = nana::color(70, 70, 70);
	menubar.scheme().border_highlight = nana::color(120, 120, 120);
	menubar.scheme().border_selected = nana::color(218, 165, 32);

	filemenu = &menubar.push_back("File");
	filemenu->renderer(renderer(filemenu->renderer()));
	filemenu->append("New", [&](nana::menu::item_proxy& ip) { menuNew(); });
	filemenu->append("Open", [&](nana::menu::item_proxy& ip) { menuOpen(); });
	filemenu->append("Save", [&](nana::menu::item_proxy& ip) { menuSave(); });
	filemenu->append("Save As", [&](nana::menu::item_proxy& ip) { menuSaveAs(); });
	filemenu->append("Export As", [&](nana::menu::item_proxy& ip) { menuExportAs(); });
	filemenu->append("Close", [&](nana::menu::item_proxy& ip) { menuClose(); });
	filemenu->append_splitter();
	filemenu->append("Exit", [&](nana::menu::item_proxy& ip) { menuExit(); });

	filemenu->enabled(2, false);
	filemenu->enabled(3, false);
	filemenu->enabled(4, false);
	filemenu->enabled(5, false);

	statusbar.format(true);
	statusbar.caption(" <size=10>Teste</>");
	statusbar.bgcolor(nana::color(47, 47, 47));
	statusbar.fgcolor(nana::color(230, 230, 230));

	/*combo.scheme().activated = nana::color(218, 165, 32);
	combo.scheme().background = nana::color(30, 30, 30);
	combo.scheme().foreground = nana::color(230, 230, 230);
	//combo.scheme().selection_unfocused = nana::colors::red;
	combo.push_back("Top-Left");
	combo.push_back("Top-Right");
	combo.push_back("Bottom-Right");
	combo.push_back("Bottom-Left");
	*/

	list.scheme().header_bgcolor = nana::color(47, 47, 47);
	list.scheme().header_fgcolor = nana::color(230, 230, 230);
	list.scheme().background = nana::color(34, 34, 34);
	list.scheme().foreground = nana::color(230, 230, 230);
	list.scheme().item_height_ex = 20;
	list.scheme().item_highlighted = nana::color(42, 42, 42);
	list.scheme().item_selected = nana::color(50, 50, 50);
	list.scheme().selection_box = nana::color(0, 0, 0);
	list.scheme().suspension_width = 20;
	list.scheme().text_margin = 10;
	list.scheme().min_column_width = 50;
	list.scheme().header_splitter_area_before = 0;
	list.show_header(false);
	list.borderless(true);
	list.enable_single(true, false);
	list.append_header("Layers");
	list.column_at(0).width(list.size().width - 15);
	resizeList();
}

void ui::Coloris::setupEvents()
{
	//Configure events
	group.getElement(0)->events().click([&](const nana::arg_click& arg)
		{
			if (isLoaded())
				addLayer(++lcount);
			focus();
		});

	group.getElement(1)->events().click([&](const nana::arg_click& arg)
		{
			auto selected = list.selected();
			if (selected.empty())
				return;

			rotateList(selected[0].item, selected[0].item + 1);
			focus();
		});

	group.getElement(2)->events().click([&](const nana::arg_click& arg)
		{
			auto selected = list.selected();
			if (selected.empty())
				return;

			rotateList(selected[0].item, selected[0].item - 1);
			focus();
		});

	group.getElement(3)->events().click([&](const nana::arg_click& arg)
		{
			auto selected = list.selected();
			if (selected.empty())
				return;

			cloneLayer(&layers[selected[0].item]);
			focus();
		});

	group.getElement(4)->events().click([&](const nana::arg_click& arg)
		{
			auto selected = list.selected();
			if (!selected.empty())
				removeLayer(selected[0].item);
			focus();
		});

	group.getElement(5)->events().click([&](const nana::arg_click& arg)
		{
			auto selected = list.selected();
			if (!selected.empty())
				flipLayer(selected[0].item, false);
			focus();
		});

	group.getElement(6)->events().click([&](const nana::arg_click& arg)
		{
			auto selected = list.selected();
			if (!selected.empty())
				flipLayer(selected[0].item, true);
			focus();
		});

	group.getElement(7)->events().click([&](const nana::arg_click& arg)
		{
			auto selected = list.selected();
			if (!selected.empty())
				layers[selected[0].item].rasterize(&resources, &shader);
			focus();
		});

	list.events().key_release([&](const nana::arg_keyboard& arg)
		{
			if (arg.key == nana::keyboard::backspace)
			{
				if (isLoaded())
					addLayer(++lcount);
			}
		});

	list.events().selected([&](const nana::arg_listbox& arg)
		{
			if (arg.item.selected())
			{
				setSelection(arg.item.pos().item);
				adjusts.enabled(true);
				adjusts.setLayer(&layers[arg.item.pos().item]);
				if (layers[arg.item.pos().item].getType() == "vertex")
				{
					sliders.enabled(true);
					sliders.setLayer(&layers[arg.item.pos().item]);
				}
			}
			else
			{
				selection.visible = false;
				sliders.enabled(false);
			}

			focus();
		});

	listdrop = std::make_unique<nana::simple_dragdrop>(list);

	listdrop->condition([&]()
		{
			auto hov = list.hovered(false);
			if (!hov.empty())
				list.at(hov).select(true);
			return !hov.empty();
		});

	listdrop->make_drop(list, [&]()
		{
			reorderList();
		});

	events().key_release([&](const nana::arg_keyboard& arg)
		{
			if (arg.key == nana::keyboard::os_shift)
				shiftPressed = false;
		});

	events().key_press([&](const nana::arg_keyboard& arg)
		{
			if (arg.key == nana::keyboard::os_shift)
			{
				shiftPressed = true;
			}
			else if (arg.key == nana::keyboard::del)
			{
				auto selected = list.selected();
				if (!selected.empty())
					removeLayer(selected[0].item);
			}
		});

	events().resized([&](const nana::arg_resized& arg)
		{
			statusbar.move(nana::rectangle(0, arg.height - 20, arg.width, 20));
			panel.move(nana::rectangle(size().width - 200, size().height - 200 - statusbar.size().height, 200, 200));
			work.setSize({ arg.width - 200, arg.height - statusbar.size().height - menubar.size().height });
			group.move(size().width - 200, size().height - 225 - statusbar.size().height);
			sliders.move(size().width - 200, menubar.size().height);
		});

	events().key_char.connect_front([&](const nana::arg_keyboard& arg)
		{
			if (arg.key == nana::keyboard::undo)
			{
				history.undo();
				if (!list.selected().empty())
					setSelection(list.selected()[0].item);
			}
			else if (arg.key == nana::keyboard::redo)
			{
				history.redo();
				if (!list.selected().empty())
					setSelection(list.selected()[0].item);
			}
		});

	events().mouse_down([&](const nana::arg_mouse& arg) { mouseClick(arg.is_left_button()); });

	events().mouse_up([&](const nana::arg_mouse& arg)
		{
			if (arg.is_left_button())
			{
				auto op = mouse.getOperation();
				if (op == cl::MouseStatus::Move && vLayerOrigin != vLayerResult)
				{
					auto sel = list.selected();
					if (!sel.empty()) //This should always be true
					{
						auto mv = std::make_shared<ActionMove>(ActionMove());
						mv->id = layers[sel[0].item].getId();
						//mv->origin = vLayerOrigin;
						//mv->result = vLayerResult;
						mv->offset = vLayerResult - vLayerOrigin;
						mv->name = "Move Layer " + layers[sel[0].item].name;//std::to_string(sel[0].item);
						mv->run = false;
						history.addAction(mv);
					}
				}
				else if (op == cl::MouseStatus::Resize)
				{
					auto sel = list.selected();
					if (!sel.empty()) //This should always be true
					{
						auto ar = std::make_shared<ActionResize>(ActionResize());
						ar->id = layers[sel[0].item].getId();
						ar->corner = mouse.getContact();
						ar->offset = posi - mouse.getPositions().first;
						ar->name = "Resize Layer " + layers[sel[0].item].name;
						ar->run = false;
						history.addAction(ar);
					}
				}

				mouse.endMovement(posi);

				mousePos.x = 0;
				mousePos.y = 0;
			}
		});

	events().mouse_move([&](const nana::arg_mouse& arg) { mouseMove(cl::Vector2i(arg.pos.x, arg.pos.y)); });

	events().mouse_wheel([&](const nana::arg_wheel& arg)
		{
			auto p = work.getPosition();
			auto s = work.getSize();
			sf::IntRect rect(p.x, p.y, s.x, s.y);

			if (rect.contains(sf::Mouse::getPosition()))
			{
				auto v = work.getView();
				if (arg.upwards)
					v.zoom(0.9f);
				else
					v.zoom(1.1f);
				work.setView(v);
			}
		});

	events().mouse_dropfiles([&](const nana::arg_dropfiles& arg)
		{
			if (isLoaded())
			{
				for (auto& f : arg.files)
				{
					//Add image layer
					auto mv = std::make_shared<ActionAddImageLayer>(ActionAddImageLayer());
					//mv->make = std::bind([](Coloris* c, std::string f) { c->addLayer(f); }, this, f.string());
					//mv->unmake = std::bind([](Coloris* c, Layer* l) { t->setPosition(v); }, &layers[sel[0].item], vLayerOrigin);
					//mv->l = addLayer(f.string(), ++lcount);
					mv->id = ++lcount;
					//mv->file = f.string();
					mv->name = "Add Image Layer " + f.filename().string();//std::to_string(sel[0].item);
					//mv->run = false;
					history.addAction(mv);
				}
			}
			else
			{
				//new project based on arg.files[0] :D
			}
		});
}

void ui::Coloris::menuNew()
{
	//remember to prompt user to save if another, unsaved project is open
	//savePromt();
	//promt for new canvas info
	nana::inputbox input(*this, "Please fill some template information...", "New File");

	nana::inputbox::text name("Name", "New Project");//"<bold blue>Name</>"
	//nana::inputbox::text gender("Gender", std::vector<std::string>{ "Male", "Female" });
	//nana::inputbox::date birth("Date of birth");
	//nana::inputbox::real height("Height(cm)", 100, 1, 300, 1);
	nana::inputbox::integer width("Width(px)", 1920, 50, 10000, 1);
	nana::inputbox::integer height("Height(px)", 1080, 50, 10000, 1);

	//Sets a verifier
	input.verify([&name](nana::window handle)
		{
			if (name.value().empty())
			{
				nana::msgbox mb(handle, "Invalid Input");
				mb << "Please add a Project Name.";
				mb.show();
				return false; //verification failed
			}
			return true; //verified successfully
		});

	//Call the show_modal() method to show the inputbox.
	//This method blocks the application execution until the inputbox gets closed.
	//To allow user to continue interacting with parent and other windows, use show() instead.
	//It returns true if the user clicks OK button, or false otherwise.
	if (input.show(name, width, height))
	{
		pName = name.value();
		pSize.x = width.value();
		pSize.y = height.value();

		background.setSize(pSize);
		background.visible = true;

		caption(pName + " - Coloris");

		//enable controls!
		filemenu->enabled(2, true);
		filemenu->enabled(3, true);
		filemenu->enabled(4, true);
		filemenu->enabled(5, true);
	}
}

void ui::Coloris::menuOpen()
{
	//if (isLoaded())
		//savePrompt();

	nana::filebox picker(*this, true);
	picker.add_filter("JavaScript Object Notation (*.json)", "*.json");
	picker.allow_multi_select(false);
	
	auto paths = picker.show();
	if (!paths.empty())
	{
		if (loadFromFile(paths[0].generic_string()))
		{
			pFile = paths[0].generic_string();

			//enable controls!
			filemenu->enabled(2, true);
			filemenu->enabled(3, true);
			filemenu->enabled(4, true);
			filemenu->enabled(5, true);
		}
		else
			closeProject();
		//filemenu->enabled()
	}
}

void ui::Coloris::menuSave()
{
	//if no pFile, open file save dialog
	if (pFile.empty())
	{
		nana::filebox picker(*this, false);
		picker.add_filter("JavaScript Object Notation (*.json)", "*.json");
		picker.allow_multi_select(false);

		auto paths = picker.show();
		if (!paths.empty())
		{
			saveToFile(paths[0].generic_string());
			pFile = paths[0].generic_string();
			filemenu->enabled(2, false);
		}
	}
	else
	{
		saveToFile(pFile);
	}
}

void ui::Coloris::menuSaveAs()
{
	//open file save dialog
	nana::filebox picker(*this, false);
	picker.add_filter("JavaScript Object Notation (*.json)", "*.json");
	picker.allow_multi_select(false);

	auto paths = picker.show();
	if (!paths.empty())
	{
		saveToFile(paths[0].generic_string());
		filemenu->enabled(2, false);
	}
}

void ui::Coloris::menuExportAs()
{
	//open file save dialog
	nana::filebox picker(*this, false);
	picker.add_filter("Portable Network Graphics (*.png)", "*.png");
	picker.add_filter("JPEG (*.jpg)", "*.jpg;*.jpeg");
	picker.add_filter("Bitmap (*.bmp)", "*.bmp");
	picker.allow_multi_select(false);

	auto paths = picker.show();
	if (!paths.empty())
	{
		sf::RenderTexture texture;
		texture.create(pSize.x, pSize.y);
		texture.clear(sf::Color::Black);
		for (auto& layer : layers)
			if (layer.getType() == "vertex")
			{
				sf::RenderStates states;
				//reconfigure the shader with the layer data
				VColor vc = layer.getColors();
				shader.setUniform("color0", sf::Glsl::Vec4(vc.a));
				shader.setUniform("color1", sf::Glsl::Vec4(vc.b));
				shader.setUniform("color3", sf::Glsl::Vec4(vc.c));
				shader.setUniform("color2", sf::Glsl::Vec4(vc.d));
				states.shader = &shader;
				texture.draw(layer, states);
			}
			else
				texture.draw(layer);
		texture.display();
		texture.getTexture().copyToImage().saveToFile(paths[0].generic_string());
	}
}

void ui::Coloris::menuClose()
{
	//remember to prompt user to save if another, unsaved project is open
	//savePromt();

	//close the current canvas and block tools
	closeProject();

	filemenu->enabled(2, false);
	filemenu->enabled(3, false);
	filemenu->enabled(4, false);
	filemenu->enabled(5, false);
}

void ui::Coloris::menuExit()
{
	//remember to prompt user to save if another, unsaved project is open
	//savePromt();

	//exit software
	nana::API::exit();
}

void ui::Coloris::rotateList(size_t oldPos, size_t newPos)
{
	//Make sure both positions are valid. This is useful for when multiple things are changing the list frequently.
	auto s = layers.size();
	if (newPos < s && oldPos < s)
	{
		if (oldPos < newPos)
		{
			auto its = layers.begin() + oldPos;
			auto ith = layers.begin() + newPos + 1;
			auto fit = std::rotate(its, its + 1, ith);

			updateList();
			list.at(0).at(newPos).select(true);
		}
		else if (oldPos > newPos)
		{
			auto rits = layers.rbegin() + (layers.size() - oldPos - 1);
			auto rith = layers.rbegin() + (layers.size() - newPos);
			auto fit = std::rotate(rits, rits + 1, rith);

			updateList();
			list.at(0).at(newPos).select(true);
		}
		else //The user dragged but there was no actual movement
		{
			//We do nothing on this scenario, this is just a reminder
		}
	}
}

void ui::Coloris::reorderList()
{
	//First, we must determine where our new position is.

	//Check to see if we have a destination item. If we don't, return.
	auto hovered = list.hovered(false);
	if (hovered.empty())
		return;

	//Then, get the selected position.
	if (list.selected().empty())
		return;
	auto selected = list.selected()[0];

	rotateList(selected.item, hovered.item);
}

void ui::Coloris::resizeList()
{
	//So apparently the item height is set to 35px (text_height + item_height_ex), don't ask me how I know this because I just measured it
	unsigned s = static_cast<unsigned>(35 * list.at(0).size());
	list.size(nana::size(200, s > 200 ? s : 200));
	panel.size({ list.size().width, list.size().height + 100 });
}

void ui::Coloris::mouseClick(bool leftclick)
{
	if (leftclick)
	{
		auto p = work.getPosition();
		auto s = work.getSize();
		sf::IntRect rect(p.x, p.y, s.x, s.y);
		auto pos = sf::Mouse::getPosition();

		if (rect.contains(pos))
		{
			auto status = mouse.beginMovement(posi, !list.selected().empty() ? &selection : nullptr);

			switch (status)
			{
			case cl::MouseStatus::Pan:
				break;
			case cl::MouseStatus::Move:
				vLayerOrigin = layers[list.selected()[0].item].getPosition();
				break;
			case cl::MouseStatus::Resize:
				vLayerSize = layers[list.selected()[0].item].getSize();
				break;
			default:
				break;
			}

			mousePos = pos;
			/*if (selection.isResize(posi))
			dragSel = selection.getRect().contains(posi);
			if (dragSel && !list.selected().empty())
				vLayerOrigin = layers[list.selected()[0].item].getPosition();
			mousePos = pos;*/
		}
	}
	else
	{
		updatePreviews();
	}
}

void ui::Coloris::mouseMove(cl::Vector2i moved)
{
	sf::Vector2i pipi;
	pipi.x = moved.x;
	pipi.y = moved.y - menubar.size().height;
	posi_last = posi;
	posi = work.mapPixelToCoords(pipi);

	if (mousePos.x != 0 && mousePos.y != 0)
	{
		auto p = work.getPosition();
		auto s = work.getSize();
		sf::IntRect rect(p.x, p.y, s.x, s.y);
		auto curPos = sf::Mouse::getPosition();
		auto tempresult = mousePos - curPos;//work.mapCoordsToPixel(posi_last) - work.mapCoordsToPixel(posi);
		auto tempPosi = posi_last - posi;
		sf::Vector2f resultC;
		//try something dumb
		resultC.x = std::round(tempPosi.x);
		resultC.y = std::round(tempPosi.y);
		auto resultP = sf::Vector2f({ (float)tempresult.x, (float)tempresult.y });
		mouse.update(posi);

		if (rect.contains(curPos))
		{
			auto sel = list.selected();
			if (!sel.empty())
			{
				auto op = mouse.getOperation();
				if (op == cl::MouseStatus::Move)
				{
					layers[sel[0].item].move(-resultC);
					selection.move(-resultC);
					vLayerResult = layers[sel[0].item].getPosition();
				}
				else if (op == cl::MouseStatus::Resize)
				{
					if (shiftPressed)
						layers[sel[0].item].resizeAspect(mouse.getContact(), vLayerSize, mouse.getPositions());//std::make_pair(posi_clicked, posi)); //mouse.getPositions());
					else
						layers[sel[0].item].resize(mouse.getContact(), -resultC);
					//we should have a selection.resize() here... But we're lazy...
					setSelection(sel[0].item);
				}
				else //This is MouseState::Pan
				{
					auto v = work.getView();
					v.move(resultP * (float)getZoomFactor());
					work.setView(v);
				}
			}
			else
			{
				auto v = work.getView();
				v.move(resultP * (float)getZoomFactor());
				work.setView(v);
			}
		}
		mousePos = curPos;
	}
}

void ui::Coloris::draw()
{
	sf::Event event;
	while (work.pollEvent(event))
	{
		if (event.type == sf::Event::Resized)
		{
			auto factor = getInverseZoomFactor();
			sf::View v(sf::Vector2f(((float)event.size.width) / 2.f, ((float)event.size.height) / 2.f), sf::Vector2f((float)event.size.width, (float)event.size.height));
			v.zoom((float)factor);
			work.setView(v);
		}
	}

	resources.update();

	sf::CircleShape shape;
	shape.setFillColor(sf::Color::Red);
	shape.setRadius(50.f);
	shape.setPosition(10.f, 10.f);
	rotation += 5.f;
	shape.setRotation(rotation);

	sfe::RichText text;
	if (debug)
	{
		text.setFont(*resources.loadFont("arial.ttf"));
		text.setCharacterSize(14);
		text << sfe::TextStroke{ sf::Color::White, sf::Color::Black, 1.f }
			<< "Current Zoom Factor: " << sf::Color::Blue << std::to_string(getZoomFactor()) << "\n"
			<< sf::Color::White << "Mouse: (" << sf::Color::Green << std::to_string(posi.x) << sf::Color::White
			<< ", " << sf::Color::Green << std::to_string(posi.y) << sf::Color::White << " (" << sf::Color::Cyan
			<< std::to_string(menubar.size().height) << sf::Color::White << "))\n";
		if (!layers.empty())
			text << "Layer 1 Pos: (" << sf::Color::Yellow << std::to_string(layers[0].getPosition().x) << sf::Color::White
			<< ", " << sf::Color::Yellow << std::to_string(layers[0].getPosition().y) << sf::Color::White << ")\n"
			<< "Layer 1 Size: (" << sf::Color::Yellow << std::to_string(layers[0].getSize().x) << sf::Color::White
			<< ", " << sf::Color::Yellow << std::to_string(layers[0].getSize().y) << sf::Color::White << ")\n";
		text << "History: " << sf::Color::Magenta << history.toString() << sf::Color::White;
		text << "Resources: " << sf::Color::Magenta << resources.toString() << sf::Color::White;
	}

	background.setScale((float)getZoomFactor(), (float)getZoomFactor());

	work.clear(sf::Color(30, 30, 30));
	work.draw(background);
	
	if (debug)
		work.draw(shape);

	if (!list.selected().empty() and layers[list.selected()[0].item].update_selection)
	{
		setSelection(list.selected()[0].item);
		layers[list.selected()[0].item].update_selection = false;
	}

	for (auto& layer : layers)
	{
		if (layer.getType() == "vertex")
		{
			sf::RenderStates states;
			//reconfigure the shader with the layer data
			VColor vc = layer.getColors();
			shader.setUniform("color0", sf::Glsl::Vec4(vc.a));
			shader.setUniform("color1", sf::Glsl::Vec4(vc.b));
			shader.setUniform("color3", sf::Glsl::Vec4(vc.c));
			shader.setUniform("color2", sf::Glsl::Vec4(vc.d));
			states.shader = &shader;
			work.draw(layer, states);
		}
		else
			work.draw(layer);
	}

	work.draw(selection);

	if (debug)
	{
		auto temp = work.getView();
		work.setView(sf::View({ (float)work.getSize().x / 2.f, (float)work.getSize().y / 2.f }, { (float)work.getSize().x, (float)work.getSize().y }));
		work.draw(text);
		work.setView(temp);
	}
	work.display();
}

bool ui::Coloris::loadFromFile(std::string file)
{
	try
	{
		std::ifstream f(file);
		if (f.is_open())
		{
			json j;
			{
				std::ostringstream ss;
				ss << f.rdbuf();
				j = json::parse(ss.str(), nullptr, false);
			}

			if (j == json::value_t::discarded)
				return false;

			pName = j["name"].get<std::string>();
			pSize.x = j["size"]["width"];
			pSize.y = j["size"]["height"];

			background.setSize(pSize);
			background.visible = true;
			caption(pName + " - Coloris");

			json data = j["layers"];
			for (auto& d : data)
			{
				std::string type = d["type"];
				if (type == "vertex")
				{
					VColor vc;
					vc.a = sf::Color(d["colors"]["a"]["r"], d["colors"]["a"]["g"], d["colors"]["a"]["b"], d["colors"]["a"]["a"]);
					vc.b = sf::Color(d["colors"]["b"]["r"], d["colors"]["b"]["g"], d["colors"]["b"]["b"], d["colors"]["b"]["a"]);
					vc.c = sf::Color(d["colors"]["c"]["r"], d["colors"]["c"]["g"], d["colors"]["c"]["b"], d["colors"]["c"]["a"]);
					vc.d = sf::Color(d["colors"]["d"]["r"], d["colors"]["d"]["g"], d["colors"]["d"]["b"], d["colors"]["d"]["a"]);

					auto l = addLayer(vc, { d["size"]["width"], d["size"]["height"] });
					l->name = d["name"].get<std::string>();
					l->setPosition(d["position"]["x"], d["position"]["y"]);
					l->setRotation(d["rotation"]);
				}
				else if (type == "image")
				{
					std::vector<sf::Uint8> vec = base64::decode(d["data"].get<std::string>());
					sf::Image img;
					img.loadFromMemory(vec.data(), vec.size());
					auto l = addLayer(img);
					l->name = d["name"].get<std::string>();
					l->setPosition(d["position"]["x"], d["position"]["y"]);
					l->setRotation(d["rotation"]);
				}
				else if (type == "text")
				{

				}
				else
					return false;
			}
			
			return true;
		}
		return false;
	}
	catch (json::exception& e)
	{
		//output exception information
		messageBox("Error loading project", "The loading operation has failed. The project file is likely corrupt");
		return false;
	}
}

bool ui::Coloris::saveToFile(std::string file)
{
	json j;
	json data = json::array();
	for (auto& l : layers)
	{
		json obj;

		auto type = l.getType();
		//turn into Vector2i later
		sf::Vector2f pos = l.getPosition();
		sf::Vector2u size = l.getSize();

		obj["type"] = type;
		obj["name"] = l.name;

		obj["position"] = { {"x", pos.x}, {"y", pos.y} };
		obj["size"] = { {"width", size.x}, {"height", size.y} };
		obj["rotation"] = l.getRotation();

		if (type == "vertex")
		{
			VColor colors = l.getColors();
			obj["colors"] = {
				{"a", {{"r", colors.a.r}, {"g", colors.a.g}, {"b", colors.a.b}, {"a", colors.a.a}}},
				{"b", {{"r", colors.b.r}, {"g", colors.b.g}, {"b", colors.b.b}, {"a", colors.b.a}}},
				{"c", {{"r", colors.c.r}, {"g", colors.c.g}, {"b", colors.c.b}, {"a", colors.c.a}}},
				{"d", {{"r", colors.d.r}, {"g", colors.d.g}, {"b", colors.d.b}, {"a", colors.d.a}}}
			};
		}
		else if (type == "image")
		{
			/* Consider using the code below as a secondary option in case the file cannot be loaded
			 * from the original path.
			 */
			//Encode layer as png and store it
			std::vector<sf::Uint8> content;
			auto image = l.getTexture()->copyToImage();
			if (cl::saveToMemory("png", { image.getPixelsPtr(), image.getSize().x, image.getSize().y }, content))
			{
				obj["format"] = "png";//path.extension().string();
				obj["data"] = base64::encode(content.data(), content.size());

				//std::string thing = obj["data"];
				//setFileBinary(".tmp/" + l.name + "_encode.png", content);
				//setFileBinary(".tmp/" + l.name + "_decode.png", base64::decode(thing));
			}
			else
			{
				messageBox("Failed to save project", "Some of the content in this project is no longer available,"
					" and our attempts to repair this issue have failed. The project file remains usable,"
					" but the content will not be available.");
			}
			//setFileBinary("test.png", content);
			/*fs::path path(resources.getPath(l.getTexture()));

			//Attempt to load file from existing source
			std::string file = getFileBinary(path.string());
			if (file.empty())
			{
				//If the file was moved, deleted, etc, use the available buffer.
				//If something fails here, the resulting project file will be damaged
				auto newpath = ".tmp/.img" + path.extension().string();
				bool success = l.getTexture()->copyToImage().saveToFile(newpath);
				file = getFileBinary(newpath);

				//This error should, in theory, never happen
				if (!success or file.empty())
					messageBox("Failed to save project", "Some of the content in this project is no longer available,"
						" and our attempts to repair this issue have failed. The project file remains usable,"
						" but the content will not be available.");
			}*/

		}
		else if (type == "text")
		{

		}
		else
			return false;

		data.push_back(obj);
	}

	j["layers"] = data;
	j["name"] = pName;
	j["size"] = { {"width", pSize.x}, {"height", pSize.y} };

	std::ofstream s(file);
	if (s.is_open())
	{
		s << j.dump(4);
		return true;
	}

	return false;
}

void ui::Coloris::closeProject()
{
	list.clear();
	layers.clear();
	history.clear();
	pName = "";
	pSize.x = 0;
	pSize.y = 0;
	background.visible = false;
	selection.visible = false;
	sliders.enabled(false);

	caption("Coloris");
}

double ui::Coloris::getZoomFactor()
{
	auto vsize = work.getView().getSize();
	auto wsize = work.getSize();

	return vsize.x / wsize.x;
}

double ui::Coloris::getInverseZoomFactor()
{
	auto vsize = work.getView().getSize();
	auto wsize = work.getSize();

	return wsize.x / vsize.x;
}

Layer* ui::Coloris::addLayer(int id)
{
	if (isLoaded())
	{
		layers.push_back(Layer(id > 0 ? id : ++lcount));

		VColor color;
		color.a = generateColor();
		color.b = generateColor();
		color.c = generateColor();
		color.d = generateColor();

		layers.back().setup(sf::Vector2u(1920, 1080), color, "Layer " + std::to_string(layers.size()) + " (Vertex)");
		list.at(0).push_back(layers.back().name);
		resizeList();
		return &layers.back();
	}
	return nullptr;
}

Layer* ui::Coloris::addLayer(VColor vc, sf::Vector2u size, int id)
{
	if (isLoaded())
	{
		layers.push_back(Layer(id > 0 ? id : ++lcount));

		layers.back().setup(size, vc, "Layer " + std::to_string(layers.size()) + " (Vertex)");
		layers.back().debug_draw = debug;
		list.at(0).push_back(layers.back().name);
		resizeList();
		return &layers.back();
	}
	return nullptr;
}

Layer* ui::Coloris::addLayer(std::string file, int id)
{
	if (isLoaded())
	{
		auto t = resources.loadTexture(file);
		if (t)
		{
			layers.push_back(Layer(id > 0 ? id : ++lcount));
			layers.back().setup(t, "Layer " + std::to_string(layers.size()) + " (Image)");
			layers.back().debug_draw = debug;
			list.at(0).push_back(layers.back().name);
			resizeList();
			return &layers.back();
		}
		else
		{
			messageBox("Failed to load image...", "File " + file + " could not be loaded!");
		}
	}
	return nullptr;
}

Layer* ui::Coloris::addLayer(std::shared_ptr<sf::Texture> t, int id)
{
	if (isLoaded())
	{
		if (t)
		{
			layers.push_back(Layer(id > 0 ? id : ++lcount));
			layers.back().setup(t, "Layer " + std::to_string(layers.size()) + " (Image)");
			layers.back().debug_draw = debug;
			list.at(0).push_back(layers.back().name);
			resizeList();
			return &layers.back();
		}
		else
		{
			messageBox("Failed to load image...", "Image could not be loaded!");
		}
	}
	return nullptr;
}

Layer* ui::Coloris::addLayer(const sf::Image& img, int id)
{
	if (isLoaded())
	{
		auto t = resources.loadTextureFromImage(img);
		if (t)
		{
			layers.push_back(Layer(id > 0 ? id : ++lcount));
			layers.back().setup(t, "Layer " + std::to_string(layers.size()) + " (Image)");
			layers.back().debug_draw = debug;
			list.at(0).push_back(layers.back().name);
			resizeList();
			return &layers.back();
		}
		else
		{
			messageBox("Failed to load image...", "Image could not be loaded!");
		}
	}
	return nullptr;
}

Layer* ui::Coloris::addLayer(std::string text, std::string font, int id)
{
	if (isLoaded())
	{
		layers.push_back(Layer(id > 0 ? id : ++lcount));

		VColor color;
		color.a = generateColor();
		color.b = generateColor();
		color.c = generateColor();
		color.d = generateColor();

		layers.back().setup(sf::Vector2u(1920, 1080), color, "Layer " + std::to_string(layers.size()) + " (Vertex)");
		layers.back().debug_draw = debug;
		list.at(0).push_back(layers.back().name);
		resizeList();
		return &layers.back();
	}
	return nullptr;
}

Layer* ui::Coloris::cloneLayer(Layer* l)
{
	if (l)
	{
		auto name = l->name;
		auto pos = l->getPosition();
		auto size = l->getSize();
		if (l->getType() == "vertex")
		{
			auto nl = addLayer(l->getColors(), size, ++lcount);
			nl->name = name + " (Copy)";
			nl->setPosition(pos);
			updateList();
			return nl;
		}
		else if (l->getType() == "image")
		{
			auto nl = addLayer(l->getTexture(), ++lcount);
			nl->name = name + " (Copy)";
			//nl->setPosition(pos);
			nl->resizeTo({ (cl::Vector2i)pos, (cl::Vector2i)size });
			updateList();
			return nl;
		}
	}
	return nullptr;
}

bool ui::Coloris::removeLayer(size_t index, bool action)
{
	if (index >= layers.size())
		return false;

	if (action)
	{
		if (layers[index].getType() == "vertex")
		{
			auto action = std::make_shared<ActionRemoveVertexLayer>();
			action->id = layers[index].getId();
			action->size = layers[index].getSize();
			action->color = layers[index].getColors();
			action->lname = layers[index].name;
			action->name = "Remove Vertex Layer";
			action->pos = layers[index].getPosition();
			action->size = layers[index].getSize();
			action->run = false;
			history.addAction(action);
		}
		else if (layers[index].getType() == "image")
		{
			auto action = std::make_shared<ActionRemoveImageLayer>();
			action->id = layers[index].getId();
			//action->file = resources.getPath(layers[index].getTexture());
			action->lname = layers[index].name;
			action->name = "Remove Image Layer";
			action->recovery = layers[index].getTexture();
			action->pos = layers[index].getPosition();
			action->size = layers[index].getSize();
			action->run = false;
			history.addAction(action);
		}
	}

	layers.erase(layers.begin() + index);
	list.erase(list.at(0).at(index));
	resizeList();
	selection.visible = false;

	return true;
}

void ui::Coloris::flipLayer(size_t index, bool vertical)
{
	layers[index].flip(vertical);
}

size_t ui::Coloris::getLayerIndex(Layer* l)
{
	if (l)
	{
		auto it = std::find_if(layers.begin(), layers.end(), [&](const Layer& layer) { return &layer == &*l; });
		if (it != layers.end())
			return it - layers.begin();
	}
	return SIZE_MAX;
}

Layer* ui::Coloris::getLayerByIndex(size_t index)
{
	if (index >= layers.size())
		return nullptr;
	return &layers[index];
}

Layer* ui::Coloris::getLayerById(int id)
{
	auto it = std::find_if(layers.begin(), layers.end(), [&](const Layer& l) { return l.getId() == id; });
	if (it != layers.end())
		return &*it;
	return nullptr;
}

T ui::Coloris::getTextureByLayerId(int id)
{
	auto l = getLayerById(id);
	if (l)
		return l->getTexture();
	return T();
}

void ui::Coloris::updateList()
{
	list.avoid_drawing([&]() {
		list.clear();
		for (auto& l : layers)
		{
			list.at(0).push_back(l.name);
			list.size(nana::size(200, unsigned(35 * list.at(0).size())));
			panel.size({ list.size().width, list.size().height + 100 });
		}
		});
}

void ui::Coloris::updatePreviews()
{
	//update layer icons
	for (size_t i = 0; i < layers.size(); ++i)
	{
		float scale = 0.02f;
		auto v = layers[i].getSize();
		auto vee = v * scale;
		sf::RenderTexture texture;
		sf::RenderStates states;
		if (layers[i].getType() == "vertex")
		{
			VColor vc = layers[i].getColors();
			shader.setUniform("color0", sf::Glsl::Vec4(vc.a));
			shader.setUniform("color1", sf::Glsl::Vec4(vc.b));
			shader.setUniform("color3", sf::Glsl::Vec4(vc.c));
			shader.setUniform("color2", sf::Glsl::Vec4(vc.d));
			states.shader = &shader;
		}
		states.transform.scale({ scale, scale });
		sf::View camera(layers[i].getPosition() * scale + vee / 2.f, vee);
		texture.create((unsigned)vee.x, (unsigned)vee.y);
		texture.setView(camera);
		texture.clear(sf::Color::Black);
		texture.draw(layers[i], states);
		texture.display();

		auto image = texture.getTexture().copyToImage();
		std::vector<sf::Uint8> content;
		if (cl::saveToMemory("bmp", { image.getPixelsPtr(), image.getSize().x, image.getSize().y }, content))
			if (layers[i].preview.open(content.data(), content.size()))
				list.at(0).at(i).icon(layers[i].preview);
	}
}

void ui::Coloris::setSelection(size_t index)
{
	auto& l = layers[index];
	selection.setRect({ l.getPosition(), l.getSize() * 1.f });
	selection.visible = true;
}

void ui::Coloris::messageBox(std::string title, std::string message)
{
	nana::msgbox m(*this, title);
	m.icon(nana::msgbox::icon_warning);
	m << message;
	m.show();
}

VColor generateColors()
{
	VColor c;
	c.a = generateColor();
	c.b = generateColor();
	c.c = generateColor();
	c.d = generateColor();
	return c;
}

void ui::run()
{
	Coloris win(800, 600, "Coloris");
	win.show();
	nana::exec();
}