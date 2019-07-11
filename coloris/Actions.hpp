#pragma once

#include "Gui.hpp"

struct ActionMove : public IAction
{
	void make()
	{
		coloris->getLayerById(id)->move(offset);
	};
	void unmake()
	{
		coloris->getLayerById(id)->move(-offset);
	};

	bool validate()
	{
		return true; //movable&& origin != result;
	};

	int id = -1;
	sf::Vector2f offset;
	//sf::Vector2f origin;
	//sf::Vector2f result;
};

struct ActionResize : public IAction
{
	void make()
	{
		coloris->getLayerById(id)->resize(corner, offset);
	};

	void unmake()
	{
		coloris->getLayerById(id)->resize(corner, -offset);
	};

	bool validate()
	{
		return true;
	};

	int id = -1;
	short corner = 0;
	sf::Vector2f offset;
};

struct ActionAddVertexLayer : public IAction
{
	void make()
	{
		id = coloris->addLayer(color, id)->getId();
	};

	void unmake()
	{
		coloris->removeLayer(coloris->getLayerIndex(coloris->getLayerById(id)));
	};

	bool validate()
	{
		return true;
	};

	int id = -1;
	VColor color;
};

struct ActionAddImageLayer : public IAction
{
	void make()
	{
		id = coloris->addLayer(file, id)->getId();
	};

	void unmake()
	{
		coloris->removeLayer(coloris->getLayerIndex(coloris->getLayerById(id)));
	}

	bool validate()
	{
		return true;
	}

	int id = -1;
	std::string file;
};