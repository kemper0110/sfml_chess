#include "Board.h"

Board::Board() {
	sprite.setTexture(texture);

	data = {
		std::make_unique<Rook>(Figure::Color::Black, sf::Vector2i{0, 0}, *this), std::make_unique<Knight>(Figure::Color::Black, sf::Vector2i{1, 0}, *this), std::make_unique<Bishop>(Figure::Color::Black, sf::Vector2i{2, 0}, *this), std::make_unique<Queen>(Figure::Color::Black, sf::Vector2i{3, 0}, *this),
		std::make_unique<King>(Figure::Color::Black, sf::Vector2i{4, 0}, *this), std::make_unique<Bishop>(Figure::Color::Black, sf::Vector2i{5, 0}, *this), std::make_unique<Knight>(Figure::Color::Black, sf::Vector2i{6, 0}, *this), std::make_unique<Rook>(Figure::Color::Black, sf::Vector2i{7, 0}, *this),
		std::make_unique<Pawn>(Figure::Color::Black, sf::Vector2i{0, 1}, *this), std::make_unique<Pawn>(Figure::Color::Black, sf::Vector2i{1, 1}, *this), std::make_unique<Pawn>(Figure::Color::Black, sf::Vector2i{2, 1}, *this), std::make_unique<Pawn>(Figure::Color::Black, sf::Vector2i{3, 1}, *this),
		std::make_unique<Pawn>(Figure::Color::Black, sf::Vector2i{4, 1}, *this), std::make_unique<Pawn>(Figure::Color::Black, sf::Vector2i{5, 1}, *this), std::make_unique<Pawn>(Figure::Color::Black, sf::Vector2i{6, 1}, *this), std::make_unique<Pawn>(Figure::Color::Black, sf::Vector2i{7, 1}, *this),

		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,

		std::make_unique<Pawn>(Figure::Color::White, sf::Vector2i{0, 6}, *this), std::make_unique<Pawn>(Figure::Color::White, sf::Vector2i{1, 6}, *this), std::make_unique<Pawn>(Figure::Color::White, sf::Vector2i{2, 6}, *this), std::make_unique<Pawn>(Figure::Color::White, sf::Vector2i{3, 6}, *this),
		std::make_unique<Pawn>(Figure::Color::White, sf::Vector2i{4, 6}, *this), std::make_unique<Pawn>(Figure::Color::White, sf::Vector2i{5, 6}, *this), std::make_unique<Pawn>(Figure::Color::White, sf::Vector2i{6, 6}, *this), std::make_unique<Pawn>(Figure::Color::White, sf::Vector2i{7, 6}, *this),
		std::make_unique<Rook>(Figure::Color::White, sf::Vector2i{0, 7}, *this), std::make_unique<Knight>(Figure::Color::White, sf::Vector2i{1, 7}, *this), std::make_unique<Bishop>(Figure::Color::White, sf::Vector2i{2, 7}, *this), std::make_unique<Queen>(Figure::Color::White, sf::Vector2i{3, 7}, *this),
		std::make_unique<King>(Figure::Color::White, sf::Vector2i{4, 7}, *this), std::make_unique<Bishop>(Figure::Color::White, sf::Vector2i{5, 7}, *this), std::make_unique<Knight>(Figure::Color::White, sf::Vector2i{6, 7}, *this), std::make_unique<Rook>(Figure::Color::White, sf::Vector2i{7, 7}, *this),
	};
}


const std::unique_ptr<Figure>& Board::at(sf::Vector2i pos) const{
	return data[pos.y][pos.x];
}

void Board::move(sf::Vector2i src, sf::Vector2i dst) {
	auto& figure = data[src.y][src.x];
	if (figure && figure->canMove(dst)) {
		figure->move(dst);

		auto& target = data[dst.y][dst.x];
		target = std::move(figure);

		// append to history this move;
	}
}

const std::list<std::string>& Board::getHistory() const
{
	return history;
}

const std::array<std::array<std::unique_ptr<Figure>, 8>, 8>& Board::getData() const
{
	return data;
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
	for (const auto& row : data) {
		for (const auto& cell : row) {
			if (cell)
				target.draw(*cell, states);
		}
	}
}
bool Board::load_texture() {
	constexpr auto filename = "board.png";
	return texture.loadFromFile(filename);
}