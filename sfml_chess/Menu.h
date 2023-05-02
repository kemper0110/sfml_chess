#pragma once
#include "Frame.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>


class Menu : public Frame
{
	// play from hand to hand
	// play online
	// (play with computer)
	// quit
	sf::Color color = sf::Color::Green;
	sf::RectangleShape rect;

	sf::RectangleShape h2h_button;
	sf::Text h2h_text;

	sf::RectangleShape online_button;
	sf::Text online_text;
public:
	Menu(class Engine& engine);


		
	// Унаследовано через Frame
	virtual void process_event(sf::Event event) override;

	virtual void render() override;
};

