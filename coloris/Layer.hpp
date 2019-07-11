#pragma once

#include <nana/paint/image.hpp>
#include <SFML/Graphics.hpp>
#include <string>

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
	sf::Sprite sprite;
};

struct TextData
{
	std::shared_ptr<sf::Font> font;
	sf::Text text;
};

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

	sf::Vector2f getSize() const;
	sf::Vector2f getVPos(size_t) const;
	std::string getType() const;

	int getId() const;

	//void move(const sf::Vector2f& offset);

	std::shared_ptr<sf::Texture> getTexture() const;

	void resize(short corner, sf::Vector2f offset);

	void setColors(VColor);
	VColor getColors();

	void setup(sf::Vector2u size, VColor colors, std::string name);
	void setup(std::shared_ptr<sf::Texture> tex, std::string name);

	nana::paint::image preview;

	std::string name = "";

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
			target.draw(image->sprite, states);
		else //text
			target.draw(text->text, states);
	}

	int id = -1;

	LayerType m_type = Vertex;

	std::shared_ptr<VertexData> vex;
	std::shared_ptr<ImageData> image;
	std::shared_ptr<TextData> text;
};