#include "Pawn.h"
#include "Board.h"


Pawn::Pawn(Figure::Color color, sf::Vector2i pos, Board& board) : Figure(Figure::Type::Pawn, color, pos, board) {
	sprite.setTexture(Figure::texture);
	switch (color) {
	case Figure::Color::Black:
		sprite.setTextureRect(sf::IntRect(56 * 5, 0, 56, 56));
		break;
	case Figure::Color::White:
		sprite.setTextureRect(sf::IntRect(56 * 5, 56, 56, 56));
		break;
	}
}

std::unique_ptr<Figure> Pawn::clone() const {
	return std::make_unique<Pawn>(getColor(), getPosition(), board);
}


bool Pawn::canMove(sf::Vector2i newpos) const {
	if (not Figure::canMove(newpos))
		return false;

	bool colorFlag;
	switch (getColor()) {
	case Figure::Color::Black: colorFlag = 0; break;
	case Figure::Color::White: colorFlag = 1; break;
	default: throw;
	}
	// сделать взятие на проходе
	// пешка на 5(4) горизонтали 
	// другая пешка должна сделать шаг на 2 клетки, пересекая бой текущей пешки
	// взятие можно совершать только следующим ходом
	// if(board.getHistory().back() == "" and )
	const auto moves = std::array{
		//		          one step                double step			  en passant 
		std::array{  sf::Vector2i{  0, 1  }, sf::Vector2i{  0, 2  },   /*   empty    */   },
		std::array{  sf::Vector2i{  0, -1 }, sf::Vector2i{  0, -2 },   /*   empty    */   }
	};
	// forward is clear
	if (newpos == pos + moves[colorFlag][0] and not board.at(newpos))
		return true;

	// first double step with clear forward
	if (pos.y == (colorFlag == 0 ? 1 : 6) and newpos == pos + moves[colorFlag][1] and not board.at(newpos))
		return true;

	return board.at(newpos) and canAttack(newpos);
}

bool Pawn::canAttack(sf::Vector2i newpos) const
{
	if (not Figure::canAttack(newpos))
		return false;
	bool colorFlag;
	switch (getColor()) {
	case Figure::Color::Black: colorFlag = 0; break;
	case Figure::Color::White: colorFlag = 1; break;
	default: throw;
	}

	const auto attacks = std::array{
		//		        diagonal attack          diagonal attack2 
		std::array{  sf::Vector2i{ 1, 1  }, sf::Vector2i{ -1, 1  } },		// black moves
		std::array{  sf::Vector2i{ 1, -1 }, sf::Vector2i{ -1, -1 } },		// white moves
	};

	// kill over diagonal
	if (newpos == pos + attacks[colorFlag][0] or newpos == pos + attacks[colorFlag][1])
		//if(board.at(newpos))
		return true;


	return false;
}

void Pawn::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}