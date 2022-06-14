#include "King.h"
#include "FigureMoves.h"
#include "Board.h"


King::King(Figure::Color color, sf::Vector2i pos, Board& board) : Figure(Figure::Type::King, color, pos, 'K', board) {}

std::unique_ptr<Figure> King::clone() const {
	return std::make_unique<King>(this->getColor(), getPosition(), board);
}
Movement King::canMove(sf::Vector2i newpos) {
	bool color;
	Figure::Color opposite_color;
	switch (getColor()) {
	case Figure::Color::White: color = 0; opposite_color = Figure::Color::Black; break;
	case Figure::Color::Black: color = 1; opposite_color = Figure::Color::White; break;
	default: throw;
	}

	//const auto can_castle = std::invoke([this, newpos, color, opposite_color] {
	//	// 4 + 1, 4 + 2 must be not under attack
	//	const sf::Vector2i openning_king[2]{ {4, 7}, {4, 0} };
	//	const sf::Vector2i openning_rook[2][2]{
	//		{ {0, 7}, {0, 0} },	// long castling 
	//		{ {7, 7}, {7, 0} }	// short castling
	//	};

	//	// check king and rook are on places
	//	if (getPosition() != openning_king[color]
	//		or
	//		newpos != openning_rook[0][color] and newpos != openning_rook[1][color])
	//		return false;
	//	if (not FigureMoves::isUnderAttackOf(board, getPosition(), opposite_color))
	//		return false;

	//	// check history of king and rook moves
	//	const auto& history = board.getHistory();
	//	for (const auto& step : history) {
	//		const auto pos = color ?
	//			step.find('|') + 1 :		// take ending for black steps
	//			0;							// take openning for white steps
	//		if (step[pos] == 'K' or step[pos] == 'R')
	//			return false;
	//	}

	//	// long castling rook position
	//	if (newpos == openning_rook[0][color]) {
	//		const sf::Vector2i long_castling_king[2]{ {2, 7}, {2, 0} };	// x + 1
	//		// check if path is under attack

	//	}

	//	// short castling rook position
	//	else if (newpos == openning_rook[1][color]) {
	//		const sf::Vector2i short_castling_king[2]{ {6, 7}, {6, 0} };	// x - 1
	//		// check if path is under attack

	//	}
	//	});
	//if (can_castle)
	//	return true;


	if (not std::holds_alternative<Movements::Common>(Figure::canMove(newpos)) or not canAttack(newpos))
		return Movements::Illegal{};

	if (FigureMoves::isUnderAttackOf(board, newpos, opposite_color))
		return Movements::Illegal{};

	return Movements::Common{};
}
bool King::canAttack(sf::Vector2i newpos) const
{
	if (not Figure::canAttack(newpos))
		return false;
	if (
		newpos.y == pos.y and std::abs(newpos.x - pos.x) == 1	// vertical step
		or
		newpos.x == pos.x and std::abs(newpos.y - pos.y) == 1	// horizontal step
		or
		std::abs(newpos.x - pos.x) == 1 and std::abs(newpos.y - pos.y) == 1		// diagonal step
		)
	{
		return true;
	}
	return false;
}
void King::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sprite.setPosition(board_offset + sf::Vector2f(56 * pos));

	switch (getColor()) {
	case Figure::Color::Black:
		sprite.setTextureRect(sf::IntRect(56 * 4, 0, 56, 56));
		break;
	case Figure::Color::White:
		sprite.setTextureRect(sf::IntRect(56 * 4, 56, 56, 56));
		break;
	}
	target.draw(sprite, states);
}