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
	sprite.setPosition(board_offset + sf::Vector2f(56 * pos));
}

Movement Figure::canMove(sf::Vector2i newpos) {
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


	const auto king = FigureMoves::findKingOnBoard(board, color);

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

void Figure::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//target.draw(sprite, states);
}

bool Figure::loadTexture() {
	constexpr auto filename = "figures.png";
	return texture.loadFromFile(filename);
}

bool Figure::loadSprite()
{
	sprite.setTexture(texture);
	return true;
}
