#pragma once
#include <variant>

namespace Movements {
	struct Illegal {};
	struct Common {};
	struct Castling {};
	struct EnPassant {};
	struct Promotion {};
}
using Movement = std::variant<Movements::Illegal, Movements::Common, Movements::Castling, Movements::EnPassant, Movements::Promotion>;
