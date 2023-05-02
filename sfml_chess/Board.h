#pragma once
#include <array>
#include <list>
#include <iostream>
#include <optional>
#include <fmt/format.h>

#include "Movement.h"
#include "Figure.h"
#include "Engine.h"


class Board
	:
	public sf::Drawable
{


	std::array<std::array<std::unique_ptr<Figure>, 8>, 8> data;
	// not a vector because there are
	// no random access
	// only appending to end
	// heavy strings
	std::list<std::string> history;
	Engine& engine;
public:
	Figure::Color who_moves = Figure::Color::White;
	Figure::Color playing_as;
	std::optional<sf::Vector2i> promotion;

	Board(Engine& engine, Figure::Color playing_as);
	const std::unique_ptr<Figure>& at(sf::Vector2i pos) const;
	std::unique_ptr<Figure>& at(sf::Vector2i pos);
	void move(sf::Vector2i src, sf::Vector2i dst);
	const std::list<std::string>& getHistory() const;
	const std::array<std::array<std::unique_ptr<Figure>, 8>, 8>& getData() const;
	std::array<std::array<std::unique_ptr<Figure>, 8>, 8>& getData();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

