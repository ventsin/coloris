#include "Layer.hpp"

Layer::Layer(int _id)
{
	id = _id;
}

Layer::~Layer()
{
}

void Layer::clear()
{
	if (image)
		image.reset();
	if (vex)
		vex.reset();
	if (text)
		text.reset();
}

sf::Vector2f Layer::getSize() const
{
	if (m_type == LayerType::Vertex)
		return getVPos(2) - getVPos(0);
	else if (m_type == LayerType::Image)
	{
		auto rect = image->sprite.getTextureRect();
		return sf::Vector2f((float)rect.width, (float)rect.height);
	}
	return { 0.f, 0.f };
}

sf::Vector2f Layer::getVPos(size_t index) const
{
	//size_t is an unsigned type...
	if (index < 0 || index > 3)
		return sf::Vector2f();
	return vex->vertex[index].position;
}

std::string Layer::getType() const
{
	//This needs to be upgraded in the future, especially when we add text support
	return m_type == LayerType::Vertex ? "vertex" : "image";
}

int Layer::getId() const
{
	return id;
}

/*void Layer::move(const sf::Vector2f& offset)
{
	m_vex[0].position += offset;
	m_vex[1].position += offset;
	m_vex[2].position += offset;
	m_vex[3].position += offset;

	//Transformable::move(offset);
}*/

std::shared_ptr<sf::Texture> Layer::getTexture() const
{
	return image->texture;
}

void Layer::resize(short corner, sf::Vector2f offset)
{
	if (corner >= 1 and corner <= 4)
	{
		if (corner == 1)
		{
			vex->vertex[2].position -= offset;
			vex->vertex[1].position.x -= offset.x;
			vex->vertex[3].position.y -= offset.y;
			move(offset);
		}
		else if (corner == 2)
		{
			vex->vertex[1].position.x += offset.x;
			vex->vertex[2].position.x += offset.x;
			vex->vertex[2].position.y -= offset.y;
			vex->vertex[3].position.y -= offset.y;
			move(0.f, offset.y);
		}
		else if (corner == 3)
		{
			vex->vertex[2].position += offset;
			vex->vertex[1].position.x += offset.x;
			vex->vertex[3].position.y += offset.y;
		}
		else if (corner == 4)
		{
			vex->vertex[1].position.x -= offset.x;
			vex->vertex[3].position.y += offset.y;
			vex->vertex[2].position.y += offset.y;
			vex->vertex[2].position.x -= offset.x;
			move(offset.x, 0.f);
		}
	}
}

void Layer::setColors(VColor vc)
{
	vex->vertex[0].color = vc.a;
	vex->vertex[1].color = vc.b;
	vex->vertex[2].color = vc.c;
	vex->vertex[3].color = vc.d;
}

VColor Layer::getColors()
{
	VColor vc;
	vc.a = vex->vertex[0].color;
	vc.b = vex->vertex[1].color;
	vc.c = vex->vertex[2].color;
	vc.d = vex->vertex[3].color;
	return vc;
}

void Layer::setup(sf::Vector2u size, VColor colors, std::string _name)
{
	clear();

	name = _name;

	m_type = LayerType::Vertex;
	vex = std::make_shared<VertexData>(VertexData());
	vex->vertex.setPrimitiveType(sf::Quads);
	vex->vertex.append(sf::Vertex({ 0.f, 0.f }, colors.a, { 0.f, 0.f }));
	vex->vertex.append(sf::Vertex({ (float)size.x, 0.f }, colors.b, { 1.f, 0.f }));
	vex->vertex.append(sf::Vertex({ (float)size.x, (float)size.y }, colors.c, { 1.f, 1.f }));
	vex->vertex.append(sf::Vertex({ 0.f, (float)size.y }, colors.d, { 0.f, 1.f }));
}

void Layer::setup(std::shared_ptr<sf::Texture> tex, std::string _name)
{
	clear();

	name = _name;

	m_type = LayerType::Image;
	image = std::make_shared<ImageData>(ImageData());
	image->texture = tex;
	image->sprite.setTexture(*image->texture);
}