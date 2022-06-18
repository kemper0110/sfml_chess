#pragma once
#include "Figure.h"

#include "Figures_fwd.h"

#include <ranges>

class King :
    public Figure
{
public:
    King(Figure::Color color, sf::Vector2i pos, Board& board);
    std::unique_ptr<Figure> clone() const override;
    Movement canMove(sf::Vector2i newpos) override;
    bool canAttack(sf::Vector2i newpos) const override;
};

