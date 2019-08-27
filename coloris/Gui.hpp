#pragma once

#include <nana/gui.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/dragdrop.hpp>
#include <nana/gui/widgets/label.hpp>
#include "scrollpanel.hpp"
//#include "scroll.hpp"
#include <nana/gui/widgets/scroll.hpp>
//#include "listbox.hpp"
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/spinbox.hpp>

#include <SFML/Graphics.hpp>

#include "Icon.hpp"
#include "Layer.hpp"
#include "History.hpp"
#include "Workspace.hpp"
#include "Selection.hpp"
#include "Background.hpp"
#include "ButtonGroup.hpp"
#include "VertexControls.hpp"
#include "TransformAdjusts.hpp"
#include "MouseStatus.hpp"
#include "ResourceManager.hpp"

namespace ui
{
	class Coloris : public nana::form
	{
	public:
		Coloris(unsigned int, unsigned int, std::string);
		~Coloris();

		void attemptDebugMode();

		void generateIcon();

		void setupInterface();
		void setupEvents();

		//Menu functions
		void menuNew();
		void menuOpen();
		void menuSave();
		void menuSaveAs();
		void menuExportAs();
		void menuClose();
		void menuExit();

		void rotateList(size_t oldPos, size_t newPos);
		void reorderList();
		void resizeList();

		void mouseClick(bool leftclick);
		void mouseMove(cl::Vector2i moved);

		void draw();

		bool loadFromFile(std::string file);
		bool saveToFile(std::string file);
		void closeProject();

		double getZoomFactor();
		double getInverseZoomFactor();

		Layer* addLayer(int id = -1); //Default Random Vertex
		Layer* addLayer(VColor, sf::Vector2u size, int id = -1); //Colored Vertex
		Layer* addLayer(std::string, int id = -1); //Image
		Layer* addLayer(std::shared_ptr<sf::Texture>, int id = -1); //Image
		Layer* addLayer(const sf::Image&, int id = -1); //Image
		Layer* addLayer(std::string text, std::string font, int id = -1); //Text
		Layer* cloneLayer(Layer*);
		bool removeLayer(size_t, bool withAction = true);
		void flipLayer(size_t index, bool vertical);
		size_t getLayerIndex(Layer*);
		Layer* getLayerByIndex(size_t);
		Layer* getLayerById(int);
		
		T getTextureByLayerId(int id);

		void updateList();
		void updatePreviews();

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
		nana::menu* filemenu = nullptr;
		nana::listbox list;
		nana::label statusbar;
		nana::timer timer;
		nana::combox combo;

		sf::Vector2i mousePos;
		sf::Vector2f posi, posi_last;

		cl::Vector2f vLayerSize;
		sf::Vector2f vLayerOrigin;
		sf::Vector2f vLayerResult;

		float rotation = 0.f;

		size_t lcount = 0;

		History history = History(this);
		//Workspace workspace;
		VertexControls sliders;
		ButtonGroup group;
		Selection selection;
		Background background;
		TransformAdjusts adjusts;
		cl::MouseStatus mouse;
		ResourceManager resources;

		bool mouseDown = false;
		bool shiftPressed = false;

		bool debug = false;
	};

	void run();
}