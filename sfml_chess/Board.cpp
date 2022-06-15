#include "Board.h"


template<typename... Lambdas>
struct Overload : Lambdas... {
	using Lambdas::operator()...;
};

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


Board::Board(Figure::Color playing_as) : Board()
{
	this->playing_as = playing_as;
}

const std::unique_ptr<Figure>& Board::at(sf::Vector2i pos) const {
	return data[pos.y][pos.x];
}

std::unique_ptr<Figure>& Board::at(sf::Vector2i pos)
{
	return data[pos.y][pos.x];
}

void Board::move(sf::Vector2i src, sf::Vector2i dst) {
	auto& figure = data[src.y][src.x];
	if (not figure)
		return;

	const auto move_strategy = figure->canMove(dst);
	const auto move = Overload{
		[&figure, &src, &dst, this](Movements::Common) {
			figure->move(dst);
			auto& target = data[dst.y][dst.x];
			target = std::move(figure);
			// append to history this movement (Pawn is not marked)
			switch (target->getColor()) {
			case Figure::Color::White:
				if (target->getType() == Figure::Type::Pawn)
					history.push_back(fmt::format("{}{}-{}{}|", src.x, 7 - src.y, dst.x, 7 - dst.y));
				else
					history.push_back(fmt::format("{}{}{}-{}{}|", target->getMark(), src.x, 7 - src.y, dst.x, 7 - dst.y));
				break;
			case Figure::Color::Black:
				if (target->getType() == Figure::Type::Pawn)
					history.back() += fmt::format("{}{}-{}{}", src.x, 7 - src.y, dst.x, 7 - dst.y);
				else
					history.back() += fmt::format("{}{}{}-{}{}", target->getMark(), src.x, 7 - src.y, dst.x, 7 - dst.y);
				std::cout << history.back() << '\n';
				break;
			}
		},
		[&figure](Movements::Castling) { throw; },
		[&figure](Movements::EnPassan) { throw; },
		[&figure](Movements::Illegal) { throw; }
	};

	std::visit(move, move_strategy);
}

const std::list<std::string>& Board::getHistory() const
{
	return history;
}

const std::array<std::array<std::unique_ptr<Figure>, 8>, 8>& Board::getData() const
{
	return data;
}

std::array<std::array<std::unique_ptr<Figure>, 8>, 8>& Board::getData()
{
	return data;
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
	//sf::RenderTexture rtex;
	//rtex.create()
	for (const auto& row : data) {
		for (const auto& cell : row) {
			if (cell) {
				const auto& figure = *cell;
				const auto pos = figure.getPosition();

				int y_offset;
				switch (figure.getColor()) {
				case Figure::Color::Black: y_offset = 0; break;
				case Figure::Color::White: y_offset = 56; break;
				default: throw "null color";
				}
				int idx;
				switch (figure.getType()) {
				case Figure::Type::Pawn: idx = 5; break;
				case Figure::Type::Bishop: idx = 2; break;
				case Figure::Type::Knight: idx = 1; break;
				case Figure::Type::Rook: idx = 0; break;
				case Figure::Type::Queen: idx = 3; break;
				case Figure::Type::King: idx = 4; break;
				default: throw "who are you?";
				}
				figure.sprite.setTextureRect(sf::IntRect(56 * idx, y_offset, 56, 56));

				switch (playing_as) {
				case Figure::Color::White:
					figure.sprite.setPosition(sf::Vector2f{ pos * 56 });
					break;
				case Figure::Color::Black:
					figure.sprite.setPosition(sf::Vector2f( (sf::Vector2i{7, 7} - pos ) * 56 ));
					break;
				default:
					throw "kwo are you?";
				}

				//sprite.setPosition(sf::Vector2f(56 * pos));
				
				target.draw(figure.sprite, states);
				//target.draw(*cell, states);

			}
		}
	}
}
bool Board::load_texture() {
	constexpr auto filename = "board0.png";
	return texture.loadFromFile(filename);
}