#include "Frame.h"

Frame::Frame(Engine& engine) : engine(engine)
{
}

void Frame::set_font(std::shared_ptr<sf::Font>& font)
{
	this->font = font;
}

