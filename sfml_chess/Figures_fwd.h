#pragma once

class Board;
class Figure;
class Pawn;
class Queen;
class Bishop;
class Rook;
class King;
class Knight;
class FigureMoves;

namespace Movements {
	struct Illegal;
	struct Common;
	struct Castling;
	struct EnPassan;
}

//using Movement = std::variant<Movements::Illegal, Movements::Common, Movements::Castling, Movements::EnPassan>;
