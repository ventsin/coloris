#pragma once

#include <nana/gui.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/dragdrop.hpp>
#include <nana/gui/widgets/label.hpp>
#include "scrollpanel.hpp"
//#include "scroll.hpp"
#include <nana/gui/widgets/scroll.hpp>
#include <nana/gui/widgets/button.hpp>
//#include "listbox.hpp"
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/menubar.hpp>

#include <SFML/Graphics.hpp>

#include "Icon.hpp"
#include "Layer.hpp"
#include "History.hpp"
#include "Workspace.hpp"
#include "Selection.hpp"
#include "Background.hpp"
#include "MouseStatus.hpp"
#include "ResourceManager.hpp"

namespace ui
{
	class Coloris : public nana::form
	{
	public:
		Coloris(unsigned int, unsigned int, std::string);
		~Coloris();

		void generateIcon();

		void setupInterface();
		void setupEvents();

		//Menu functions
		void menuNew();
		void menuOpen();
		void menuSave();
		void menuSaveAs();
		void menuClose();
		void menuExit();

		void reorderList();
		void resizeList();

		void mouseClick(bool leftclick);
		void mouseMove(sf::Vector2i moved);

		void draw();

		bool saveToFile(std::string file);
		void closeProject();

		double getZoomFactor();
		double getInverseZoomFactor();

		Layer* addLayer(int id = -1); //Default Random Vertex
		Layer* addLayer(VColor, int id = -1); //Colored Vertex
		Layer* addLayer(std::string, int id = -1); //Image
		Layer* addLayer(std::string text, std::string font, int id = -1); //Text
		bool removeLayer(size_t);
		size_t getLayerIndex(Layer*);
		Layer* getLayerByIndex(size_t);
		Layer* getLayerById(int);

		void updateList();

		void setSelection(size_t index);

		bool isLoaded() const { return !pName.empty(); };

		void messageBox(std::string title, std::string message);

	private:
		sf::RenderWindow work;

		std::string pFile = "";
		std::string pName = "";
		sf::Vector2u pSize;
		std::vector<Layer> layers;

		//TEMPORARY
		F font;
		sf::Shader shader;
		
		//nana::paint::image icon;
		std::unique_ptr<nana::simple_dragdrop> listdrop;
		nana::scrollpanel panel;
		nana::menubar menubar;
		nana::menu* filemenu;
		nana::listbox list;
		nana::label statusbar;
		nana::timer timer;
		nana::button newLayer;

		sf::Vector2i mousePos;
		sf::Vector2f posi, posi_last;

		sf::Vector2f vLayerOrigin;
		sf::Vector2f vLayerResult;

		float rotation = 0.f;

		size_t lcount = 0;

		History history = History(this);
		//Workspace workspace;
		Selection selection;
		Background background;
		cl::MouseStatus mouse;
		ResourceManager resources;

		bool mouseDown = false;
	};

	void run();
}