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
		auto l = coloris->addLayer(color, size, id);
		id = l->getId();
		l->resizeTo({ (cl::Vector2i)pos, (cl::Vector2i)size });
		l->name = lname;
		coloris->updateList();
	};

	void unmake()
	{
		auto l = coloris->getLayerById(id);
		size = l->getSize();
		pos = l->getPosition();
		color = l->getColors();
		lname = l->name;
		coloris->removeLayer(coloris->getLayerIndex(coloris->getLayerById(id)), false);
	};

	bool validate()
	{
		return true;
	};

	int id = -1;
	std::string lname;
	sf::Vector2u size;
	sf::Vector2f pos;
	VColor color;
};

struct ActionRemoveVertexLayer : public IAction
{
	void make()
	{
		auto l = coloris->getLayerById(id);
		size = l->getSize();
		pos = l->getPosition();
		color = l->getColors();
		lname = l->name;
		coloris->removeLayer(coloris->getLayerIndex(coloris->getLayerById(id)), false);
	};

	void unmake()
	{
		auto l = coloris->addLayer(color, size, id);
		id = l->getId();
		l->resizeTo({ (cl::Vector2i)pos, (cl::Vector2i)size });
		l->name = lname;
		coloris->updateList();
	};

	bool validate()
	{
		return true;
	};

	int id = -1;
	std::string lname;
	sf::Vector2u size;
	sf::Vector2f pos;
	VColor color;
};

struct ActionAddImageLayer : public IAction
{
	void make()
	{
		auto l = coloris->addLayer(recovery, id);
		if (l)
		{
			id = l->getId();
			l->resizeTo({ (cl::Vector2i) pos, (cl::Vector2i) size });
			l->name = lname;
			coloris->updateList();
		}
	};

	void unmake()
	{
		auto l = coloris->getLayerById(id);
		recovery = l->getTexture();
		size = l->getSize();
		pos = l->getPosition();
		lname = l->name;
		coloris->removeLayer(coloris->getLayerIndex(l), false);
	}

	bool validate()
	{
		return true;
	}

	int id = -1;
	sf::Vector2u size;
	sf::Vector2f pos;
	std::string lname;
	std::shared_ptr<sf::Texture> recovery;
};

//WARNING: if 'run' is false, then recovery needs to be set manually as well!!!
struct ActionRemoveImageLayer : public IAction
{
	void make()
	{
		auto l = coloris->getLayerById(id);
		recovery = l->getTexture();
		size = l->getSize();
		pos = l->getPosition();
		lname = l->name;
		coloris->removeLayer(coloris->getLayerIndex(l), false);
	}

	void unmake()
	{
		auto l = coloris->addLayer(recovery, id);
		if (l)
		{
			id = l->getId();
			l->resizeTo({ (cl::Vector2i) pos, (cl::Vector2i) size });
			l->name = lname;
			coloris->updateList();
		}
	}

	bool validate()
	{
		return true;
	}

	int id = -1;
	std::string lname;
	sf::Vector2u size;
	sf::Vector2f pos;
	std::shared_ptr<sf::Texture> recovery;
};