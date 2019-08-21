#pragma once

#include <nana/paint/image.hpp>
#include <SFML/Graphics.hpp>
#include <string>

#include "Vector.hpp"

struct VColor
{
	sf::Color a, b, c, d;
};

struct VertexData
{
	sf::VertexArray vertex;
};

struct ImageData
{
	std::shared_ptr<sf::Texture> texture;
	//sf::Sprite sprite;
	sf::VertexArray vertex;
};

struct TextData
{
	std::shared_ptr<sf::Font> font;
	sf::Text text;
};

class ResourceManager;

class Layer : public sf::Drawable, public sf::Transformable
{
public:
	enum LayerType
	{
		Vertex,
		Image,
		Text
	};

	Layer(int _id);
	~Layer();

	void clear();


	cl::Vector2u getSize() const;
	cl::Vector2f getCenter() const;
	cl::Vector2f getVPos(size_t) const;
	std::string getType() const;

	int getId() const;

	//void move(const sf::Vector2f& offset);

	std::shared_ptr<sf::Texture> getTexture() const;

	void resize(short corner, cl::Vector2f offset);
	void resizeTo(sf::IntRect rect);
	void resizeAspect(short corner, cl::Vector2f centroid, std::pair<cl::Vector2f, cl::Vector2f> mouse);

	void flip(bool vertical);

	void rasterize(ResourceManager*, sf::Shader*);

	void setColors(VColor);
	VColor getColors();

	void setup(cl::Vector2u size, VColor colors, std::string name);
	void setup(std::shared_ptr<sf::Texture> tex, std::string name);

	nana::paint::image preview;

	std::string name = "";

	bool debug_draw = false;
	cl::Vector2f debug_end;
	cl::Vector2f debug_size;
	cl::Vector2f debug_start;
	cl::Vector2f debug_center;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the entity's transform -- combine it with the one that was passed by the caller
		states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

		// apply the texture
		//states.texture = &m_texture;

		// draw the vertex array
		if (m_type == Vertex)
			target.draw(vex->vertex, states);
		else if (m_type == Image)
		{
			states.texture = &*image->texture;
			target.draw(image->vertex, states);
			if (debug_draw)
			{
				sf::RectangleShape debug(debug_size);
				debug.setFillColor(sf::Color::Transparent);
				debug.setOutlineColor(sf::Color::Magenta);
				debug.setOutlineThickness(3.f);
				states.texture = nullptr;
				target.draw(debug, states);

				cl::Vector2f true_center = debug_center + getPosition();

				sf::Vertex source_line[] = {
					{true_center, sf::Color::Green},
					{debug_start, sf::Color::Green},
				};
				target.draw(source_line, 2, sf::Lines);

				sf::Vertex target_line[] = {
					{true_center, sf::Color::Red},
					{debug_end, sf::Color::Red},
				};
				target.draw(target_line, 2, sf::Lines);
			}
		}
		else //text
			target.draw(text->text, states);
	}

	int id = -1;

	LayerType m_type = Vertex;

	std::shared_ptr<VertexData> vex;
	std::shared_ptr<ImageData> image;
	std::shared_ptr<TextData> text;
};