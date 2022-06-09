#include "Figure.h"
#include "Board.h"

#include "FigureMoves.h"

Figure::Figure(Type type, Color color, sf::Vector2i pos, Board& board) : type(type), color(color), pos(pos), board(board) {
	sprite.setPosition(board_offset + sf::Vector2f(56 * pos));
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

bool Figure::canMove(sf::Vector2i newpos) const {
	// переход на эту позицию не должен подставлять короля под шах
	// if(FigureMoves::isKingShah(board, newpos))
	// findKingOnBoard


	// add conditions 
	// target figure exists
	// target is not a king
	// target color is not same

	const auto& figure = board.at(newpos);
	return Figure::canAttack(newpos) and figure? figure->getType() != Figure::Type::King and figure->getColor() != this->getColor() : true;
}

bool Figure::canAttack(sf::Vector2i newpos) const
{
	if (newpos.x < 0 or newpos.y < 0 or newpos.x > 7 or newpos.y > 7)
		return false;
	return true;
}

void Figure::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite, states);
}

bool Figure::loadTexture() {
	constexpr auto filename = "figures.png";
	return texture.loadFromFile(filename);
}