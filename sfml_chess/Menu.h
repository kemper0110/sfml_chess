#pragma once



#include "Frame.h"



class Menu : public Frame
{
	// play from hand to hand
	// play online
	// (play with computer)
	// quit
	sf::Color color = sf::Color::Green;
	sf::RectangleShape rect;
	sf::RectangleShape button1;
	sf::Text text;
public:
	Menu(Engine& engine, std::shared_ptr<sf::Font> font);


		
	// Унаследовано через Frame
	virtual void process_event(sf::Event event) override;

	virtual void render() override;
};

