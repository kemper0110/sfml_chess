#include "Figure.h"
#include "Board.h"
#include "FigureMoves.h"

Figure::Figure(Type type, Color color, sf::Vector2i pos, char mark, Board& board) : type(type), color(color), pos(pos), mark(mark), board(board){}
Figure::~Figure(){}
char Figure::getMark() const
{
	return mark;
}
Figure::Type Figure::getType() const {
	return type;
}
Figure::Color Figure::getColor() const {
	return color;
}
sf::Vector2i Figure::getPosition() const {
	return pos;
}
void Figure::move(sf::Vector2i newpos) {
	pos = newpos;
}

Movement Figure::canMove(sf::Vector2i newpos) {
	if (pos == newpos)
		return Movements::Illegal{};

	// переход на эту позицию не должен подставлять короля под шах
	// if(FigureMoves::isKingShah(board, newpos))
	// findKingOnBoard


	// add conditions 
	// target figure exists
	// target is not a king
	// target color is not same

	const auto& figure = board.at(newpos);
	if (not Figure::canAttack(newpos))
		return Movements::Illegal{};

	auto& cell = board.at({ this->getPosition() });

	// if on next step king stands under attack then it's illegal step
	auto king = FigureMoves::findKingOnBoard(board, color);

	// moving figure
	const auto prev_pos = cell->getPosition();
	cell->move(newpos);
	auto& target = board.at(newpos);
	auto prev_cell = std::move(target);
	target = std::move(cell);

	Figure::Color opposite_color;
	switch (color) {
	case Figure::Color::Black: opposite_color = Figure::Color::White; break;
	case Figure::Color::White: opposite_color = Figure::Color::Black; break;
	default: throw;
	}

	if(this->type == Figure::Type::King)
		king = FigureMoves::findKingOnBoard(board, color);

	const auto bad_move = FigureMoves::isUnderAttackOf(board, king, opposite_color);

	// returning figure to previous pos
	cell = std::move(target);
	target = std::move(prev_cell);
	cell->move(prev_pos);


	if (bad_move)
		return Movements::Illegal{};

	if (figure) {
		if (figure->getType() == Figure::Type::King)
			return Movements::Illegal{};
		if (figure->getColor() == this->getColor())
			return Movements::Illegal{};
	}
	return Movements::Common{};
}

bool Figure::canAttack(sf::Vector2i newpos) const
{
	if (newpos.x < 0 or newpos.y < 0 or newpos.x > 7 or newpos.y > 7)
		return false;
	return true;
}
