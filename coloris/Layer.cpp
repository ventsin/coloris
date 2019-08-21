#include "Layer.hpp"

#include "ResourceManager.hpp"

float magnitude(cl::Vector2f center, cl::Vector2f point)
{
	return sqrt((point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y));
}

float magnitude(cl::Vector2f a)
{
	return std::sqrt(a.x * a.x + a.y * a.y);
}

cl::Vector2i round(cl::Vector2f a)
{
	a.x = std::round(a.x);
	a.y = std::round(a.y);
	return a;
}

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

cl::Vector2u Layer::getSize() const
{
	if (m_type == LayerType::Text)
		return { 0, 0 };//Do something with this
	else
		return getVPos(2) - getVPos(0);
}

cl::Vector2f Layer::getCenter() const
{
	if (m_type == LayerType::Text)
		return { 0, 0 };//Do something with this
	else
		return { (getVPos(2).x - getVPos(0).x) / 2.f, (getVPos(2).y - getVPos(0).y) / 2.f };
}

cl::Vector2f Layer::getVPos(size_t index) const
{
	if (m_type == LayerType::Text)
		return { 0.f, 0.f };
	//size_t is an unsigned type...
	if (index < 0 || index > 3)
		return { 0.f, 0.f };
	sf::VertexArray* v = m_type == LayerType::Vertex ? &vex->vertex : &image->vertex;
	return (*v)[index].position;
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

void Layer::resize(short corner, cl::Vector2f offset)
{
	if (corner >= 1 and corner <= 4)
	{
		if (m_type == LayerType::Text)
		{
		}
		else
		{
			sf::VertexArray* v = m_type == LayerType::Vertex ? &vex->vertex : &image->vertex;
			if (corner == 1)
			{
				sf::Vector2f resolved = offset;
				if ((*v)[2].position.x - offset.x <= (*v)[0].position.x)
				{
					resolved.x = (*v)[2].position.x - 1.f;
					(*v)[2].position.x = 1.f;
				}
				else
					(*v)[2].position.x -= offset.x;

				if ((*v)[2].position.y - offset.y <= (*v)[0].position.y)
				{
					resolved.y = (*v)[2].position.y - 1.f;
					(*v)[2].position.y = 1.f;
				}
				else
					(*v)[2].position.y -= offset.y;

				if ((*v)[1].position.x - offset.x <= (*v)[0].position.x)
				{
					resolved.x = (*v)[1].position.x - 1.f;
					(*v)[1].position.x = 1.f;
				}
				else
					(*v)[1].position.x -= offset.x;

				if ((*v)[3].position.y - offset.y <= (*v)[0].position.y)
				{
					resolved.y = (*v)[3].position.y - 1.f;
					(*v)[3].position.y = 1.f;
				}
				else
					(*v)[3].position.y -= offset.y;

				move(resolved);
			}
			else if (corner == 2)
			{
				sf::Vector2f resolved = offset;
				if ((*v)[1].position.x + offset.x <= (*v)[0].position.x)
				{
					resolved.x = (*v)[1].position.x - 1.f;
					(*v)[1].position.x = 1.f;
				}
				else
					(*v)[1].position.x += offset.x;

				if ((*v)[2].position.x + offset.x <= (*v)[0].position.x)
				{
					resolved.x = (*v)[2].position.x - 1.f;
					(*v)[2].position.x = 1.f;
				}
				else
					(*v)[2].position.x += offset.x;

				if ((*v)[2].position.y - offset.y <= (*v)[0].position.y)
				{
					resolved.y = (*v)[2].position.y - 1.f;
					(*v)[2].position.y = 1.f;
				}
				else
					(*v)[2].position.y -= offset.y;

				if ((*v)[3].position.y - offset.y <= (*v)[0].position.y)
				{
					resolved.y = (*v)[3].position.y - 1.f;
					(*v)[3].position.y = 1.f;
				}
				else
					(*v)[3].position.y -= offset.y;

				move(0.f, resolved.y);
			}
			else if (corner == 3)
			{
				if ((*v)[2].position.x + offset.x <= (*v)[0].position.x)
					(*v)[2].position.x = 1.f;
				else
					(*v)[2].position.x += offset.x;

				if ((*v)[2].position.y + offset.y <= (*v)[0].position.y)
					(*v)[2].position.y = 1.f;
				else
					(*v)[2].position.y += offset.y;

				if ((*v)[1].position.x + offset.x <= (*v)[0].position.x)
					(*v)[1].position.x = 1.f;
				else
					(*v)[1].position.x += offset.x;

				if ((*v)[3].position.y + offset.y <= (*v)[0].position.y)
					(*v)[3].position.y = 1.f;
				else
					(*v)[3].position.y += offset.y;
			}
			else if (corner == 4)
			{
				sf::Vector2f resolved = offset;
				if ((*v)[1].position.x - offset.x <= (*v)[0].position.x)
				{
					resolved.x = (*v)[1].position.x - 1.f;
					(*v)[1].position.x = 1.f;
				}
				else
					(*v)[1].position.x -= offset.x;

				if ((*v)[3].position.y + offset.y <= (*v)[0].position.y)
				{
					resolved.y = (*v)[3].position.y - 1.f;
					(*v)[3].position.y = 1.f;
				}
				else
					(*v)[3].position.y += offset.y;

				if ((*v)[2].position.y + offset.y <= (*v)[0].position.y)
				{
					resolved.y = (*v)[2].position.y - 1.f;
					(*v)[2].position.y = 1.f;
				}
				else
					(*v)[2].position.y += offset.y;

				if ((*v)[2].position.x - offset.x <= (*v)[0].position.x)
				{
					resolved.x = (*v)[2].position.x - 1.f;
					(*v)[2].position.x = 1.f;
				}
				else
					(*v)[2].position.x -= offset.x;

				move(resolved.x, 0.f);
			}
		}
	}
}

void Layer::resizeTo(sf::IntRect rect)
{
	if (m_type == LayerType::Text)
	{
	}
	else
	{
		sf::VertexArray* v = m_type == LayerType::Vertex ? &vex->vertex : &image->vertex;
		setPosition((float)rect.left, (float)rect.top);
		(*v)[0].position = { 0.f, 0.f };
		(*v)[1].position = { (float)rect.width, 0.f };
		(*v)[2].position = { (float)rect.width, (float)rect.height };
		(*v)[3].position = { 0.f, (float)rect.height };
	}
}

void Layer::resizeAspect(short corner, cl::Vector2f original, std::pair<cl::Vector2f, cl::Vector2f> mouse)
{
	if (corner == 1)
	{
		float d1 = magnitude(mouse.first - getPosition() - (cl::Vector2f)getSize());
		float d2 = magnitude(mouse.second - getPosition() - (cl::Vector2f)getSize());
		float factor = d2 / d1;

		cl::Vector2f oSize = original;
		cl::Vector2f nSize = round(oSize * factor);
		cl::Vector2f dSize = nSize - (cl::Vector2f)getSize();

		debug_size = nSize;
		debug_center = getCenter();
		debug_start = mouse.first;
		debug_end = mouse.second;

		resizeTo({ cl::Vector2i(getPosition() - dSize), (cl::Vector2i)nSize });
	}
	else if (corner == 2)
	{
		cl::Vector2f first, second;
		first.x = mouse.first.x - getPosition().x;
		first.x = mouse.first.y - getPosition().y - (float)getSize().y;
		second.x = mouse.second.x - getPosition().x;
		second.x = mouse.second.y - getPosition().y - (float)getSize().y;
		float d1 = magnitude(first);
		float d2 = magnitude(second);
		float factor = d2 / d1;

		cl::Vector2f oSize = original;
		cl::Vector2i nSize = round(oSize * factor);
		cl::Vector2i dSize = nSize - (cl::Vector2i)getSize();
		cl::Vector2i nPosi = cl::Vector2f(getPosition().x, getPosition().y - dSize.y);

		debug_size = nSize;
		debug_center = getCenter();
		debug_start = mouse.first;
		debug_end = mouse.second;

		resizeTo({ nPosi, nSize });
	}
	else if (corner == 3)
	{
		float d1 = magnitude(mouse.first - getPosition());
		float d2 = magnitude(mouse.second - getPosition());
		float factor = d2 / d1;

		cl::Vector2f oSize = original;
		cl::Vector2f nSize = round(oSize * factor);

		debug_size = nSize;
		debug_center = getCenter();
		debug_start = mouse.first;
		debug_end = mouse.second;

		resizeTo({ (cl::Vector2i)getPosition(), (cl::Vector2i)nSize });
	}
	else if (corner == 4)
	{
		cl::Vector2f first, second;
		first.x = mouse.first.x - getPosition().x - (float)getSize().x;
		first.x = mouse.first.y - getPosition().y;
		second.x = mouse.second.x - getPosition().x - (float)getSize().x;
		second.x = mouse.second.y - getPosition().y;
		float d1 = magnitude(first);
		float d2 = magnitude(second);
		float factor = d2 / d1;

		cl::Vector2f oSize = original;
		cl::Vector2i nSize = round(oSize * factor);
		cl::Vector2i dSize = nSize - (cl::Vector2i)getSize();
		cl::Vector2i nPosi = cl::Vector2f(getPosition().x - dSize.x, getPosition().y);

		debug_size = nSize;
		debug_center = getCenter();
		debug_start = mouse.first;
		debug_end = mouse.second;

		resizeTo({ nPosi, nSize });
	}
}

void Layer::flip(bool vertical)
{
	if (m_type == LayerType::Vertex)
	{
		if (vertical)
		{
			std::swap(vex->vertex[0].color, vex->vertex[3].color);
			std::swap(vex->vertex[1].color, vex->vertex[2].color);
		}
		else
		{
			std::swap(vex->vertex[0].color, vex->vertex[1].color);
			std::swap(vex->vertex[2].color, vex->vertex[3].color);
		}
	}
	else if (m_type == LayerType::Image)
	{
		if (vertical)
		{
			std::swap(image->vertex[0].texCoords, image->vertex[3].texCoords);
			std::swap(image->vertex[1].texCoords, image->vertex[2].texCoords);
		}
		else
		{
			std::swap(image->vertex[0].texCoords, image->vertex[1].texCoords);
			std::swap(image->vertex[2].texCoords, image->vertex[3].texCoords);
		}
	}
}

void Layer::rasterize(ResourceManager* rm, sf::Shader* shader)
{
	if (m_type == LayerType::Vertex)
	{
		auto sz = getSize();

		VColor vc = getColors();
		shader->setUniform("color0", sf::Glsl::Vec4(vc.a));
		shader->setUniform("color1", sf::Glsl::Vec4(vc.b));
		shader->setUniform("color3", sf::Glsl::Vec4(vc.c));
		shader->setUniform("color2", sf::Glsl::Vec4(vc.d));

		sf::RenderTexture rt;
		rt.create(sz.x, sz.y);
		rt.clear();
		rt.draw(vex->vertex, shader);
		rt.display();
		
		auto img = rt.getTexture().copyToImage();
		name.replace(name.find("Vertex"), 6, "Image");
		setup(rm->loadTextureFromImage(img), name);
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

void Layer::setup(cl::Vector2u size, VColor colors, std::string _name)
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

	cl::Vector2u size = tex->getSize();
	image->vertex.setPrimitiveType(sf::Quads);
	image->vertex.append(sf::Vertex({ 0.f, 0.f }, { 0.f, 0.f }));
	image->vertex.append(sf::Vertex({ (float)size.x, 0.f }, { (float)size.x, 0.f }));
	image->vertex.append(sf::Vertex({ (float)size.x, (float)size.y }, { (float)size.x, (float)size.y }));
	image->vertex.append(sf::Vertex({ 0.f, (float)size.y }, { 0.f, (float)size.y }));
}