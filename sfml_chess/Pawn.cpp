#include "Pawn.h"
#include "Board.h"


Pawn::Pawn(Figure::Color color, sf::Vector2i pos, Board& board) : Figure(Figure::Type::Pawn, color, pos, ' ', board) {}

std::unique_ptr<Figure> Pawn::clone() const {
	return std::make_unique<Pawn>(getColor(), getPosition(), board);
}


bool Pawn::canMove(sf::Vector2i newpos) {
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
		//		          one step                double step			  en passant1		en passant2 
		std::array{  sf::Vector2i{  0, 1  }, sf::Vector2i{  0, 2  },  sf::Vector2i{  },  sf::Vector2i{ }  },
		std::array{  sf::Vector2i{  0, -1 }, sf::Vector2i{  0, -2 },  sf::Vector2i{  },  sf::Vector2i{ }  }
	};
	// forward is clear
	if (newpos == pos + moves[colorFlag][0] and not board.at(newpos))
		return true;

	// first double step with clear forward
	if (pos.y == (colorFlag == 0 ? 1 : 6) and newpos == pos + moves[colorFlag][1] and not board.at(newpos))
		return true;

	//// en passant not done yet
	//if (getColor() == Figure::Color::White) {
	//	const auto row2 = '1' or '6';	// BW	
	//	const auto row4 = '3' or '4';	// BW
	//	if (const auto& last_move = board.getHistory().back();
	//		last_move.size() == 5 and last_move[1] == row2 and last_move[3] == row4)
	//		if (last_move[0] == last_move[4] and last_move[0] == pos.x) {
	//			//if()
	//		}
	//			
	//}
	//if (getColor() == Figure::Color::Black) {
	//}
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
	sprite.setPosition(board_offset + sf::Vector2f(56 * pos));

	switch (getColor()) {
	case Figure::Color::Black:
		sprite.setTextureRect(sf::IntRect(56 * 5, 0, 56, 56));
		break;
	case Figure::Color::White:
		sprite.setTextureRect(sf::IntRect(56 * 5, 56, 56, 56));
		break;
	}
	target.draw(sprite, states);
}