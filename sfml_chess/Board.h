#pragma once
#include "Figures.h"
#include <array>
#include <list>
#include <iostream>
#include <fmt/format.h>

#include "Figures_fwd.h"


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
public:
	Board();
	const std::unique_ptr<Figure>& at(sf::Vector2i pos) const;
	std::unique_ptr<Figure>& at(sf::Vector2i pos);
	void move(sf::Vector2i src, sf::Vector2i dst);
	const std::list<std::string>& getHistory() const;
	const std::array<std::array<std::unique_ptr<Figure>, 8>, 8>& getData() const;
	std::array<std::array<std::unique_ptr<Figure>, 8>, 8>& getData();
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Sprite sprite;
private:
	static bool load_texture();
public:
	static inline sf::Texture texture = sf::Texture();
	static inline bool isTextureLoaded = load_texture();
};

