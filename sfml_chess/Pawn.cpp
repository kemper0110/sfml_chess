#include "Pawn.h"
#include "FigureMoves.h"
#include "Board.h"


Pawn::Pawn(Figure::Color color, sf::Vector2i pos, class Board& board) : Figure(Figure::Type::Pawn, color, pos, ' ', board) {}

std::unique_ptr<Figure> Pawn::clone() const {
	return std::make_unique<Pawn>(getColor(), getPosition(), board);
}


Movement Pawn::canMove(sf::Vector2i newpos) {
	if (std::holds_alternative<Movements::Illegal>(Figure::canMove(newpos)))
		return Movements::Illegal{};


	//						let's assume that						BLACK = 0		WHITE = 1
	bool color_flag;
	switch (getColor()) {
	case Figure::Color::Black: color_flag = 0; break;
	case Figure::Color::White: color_flag = 1; break;
	default: throw;
	}


	const auto moves = std::array{
		//		          one step                double step		
		std::array{  sf::Vector2i{  0, 1  }, sf::Vector2i{  0, 2  } },	// black
		std::array{  sf::Vector2i{  0, -1 }, sf::Vector2i{  0, -2 } },	// white
	};

	const auto promotion_pos = std::array{ 7, 0 };

	// forward is clear
	if (newpos == pos + moves[color_flag][0] and not board.at(newpos))
		if (newpos.y == promotion_pos[color_flag])
			return Movements::Promotion{};
		else
			return Movements::Common{};

	// first double step with empty forward
	if (pos.y == (color_flag == 0 ? 1 : 6) and newpos == pos + moves[color_flag][1] and
		not board.at(newpos) and not board.at(pos + moves[color_flag][0]))	// two cells are empty
		return Movements::Common{};



	const auto can_en_passant = std::invoke([this, color_flag, newpos] {
		//								don't forget							BLACK = 0		WHITE = 1

		// start positions of pawns for black/white
		constexpr auto pos_y = std::array{ 4, 3 };
		if (pos.y != pos_y[color_flag])
			return false;

		// corrections for black/white directions
		constexpr auto dir = std::array{ 1, -1 };

		if (
			newpos != pos + sf::Vector2i(1, 1) * dir[color_flag] and	// left diagonal
			newpos != pos + sf::Vector2i(-1, 1) * dir[color_flag]		// right diagonal
			)
			return false;

		// checking history
		if (board.getHistory().empty())
			return false;
		const auto& last_move = board.getHistory().back();

		const auto color_move = color_flag ?
			std::string_view(last_move.begin() + last_move.find('|') + 1, last_move.end()) :		// take part of black move
			std::string_view(last_move.begin(), last_move.begin() + last_move.find('|'));			// take part white move


		constexpr auto coords = std::array{
			std::array{ '1', '3' },		// black
			std::array{ '6', '4' }		// white
		};
		const auto dx = newpos.x - pos.x;

		//	double-cell step forward 
		if (
			color_move[0] == color_move[3] and				// x-coord is not changed
			color_move[1] == coords[color_flag][0] and		// y-coord is base position
			color_move[3] == pos.x + dx + '0' and			// x-coord is left/right to (*this) Pawn 
			color_move[4] == coords[color_flag][1]			// y-coord changed to (base + 2)
			)
			return true;
		return false;
		}
	);

	if (can_en_passant)
		return Movements::EnPassant{};

	if (board.at(newpos) and canAttack(newpos))
		if (newpos.y == promotion_pos[color_flag])
			return Movements::Promotion{};
		else
			return Movements::Common{};
	return Movements::Illegal{};
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

