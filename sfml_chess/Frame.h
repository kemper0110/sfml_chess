#pragma once
#include <SFML/Graphics.hpp>

class Engine;

#include "Engine.h"


class Frame
{
protected:
	Engine& engine;
public:
	Frame(Engine& engine);
	virtual void process_event(sf::Event event) = 0;
	virtual void render() = 0;
	virtual ~Frame() = default;
};

