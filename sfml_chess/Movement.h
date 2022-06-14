#pragma once
#include <variant>

namespace Movements {
	struct Illegal {};
	struct Common {};
	struct Castling {};
	struct EnPassan {};
}
using Movement = std::variant<Movements::Illegal, Movements::Common, Movements::Castling, Movements::EnPassan>;
