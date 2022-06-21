#pragma once
#include "Frame.h"
#include "Board.h"


class h2hChess : public Frame
{
	std::optional<sf::Vector2i> selected;
	sf::Color movement_color, selected_color;
	sf::CircleShape cs;
	Board board;

public:
	h2hChess(Engine& engine);

	// Унаследовано через Frame
	virtual void process_event(sf::Event event) override;
	virtual void render() override;
};

