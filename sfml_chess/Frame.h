#pragma once
#include <SFML/Graphics.hpp>

class Engine;

#include "Engine.h"


class Frame
{
protected:
	Engine& engine;
	std::shared_ptr<sf::Font> font;
public:
	Frame(Engine& engine);
	void set_font(std::shared_ptr<sf::Font>& font);
	virtual void process_event(sf::Event event) = 0;
	virtual void render() = 0;
	virtual ~Frame() = default;
};

