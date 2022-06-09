#pragma once
#include "Figure.h"

#include "Figures_fwd.h"


class King :
    public Figure
{
public:
    King(Figure::Color color, sf::Vector2i pos, Board& board);
    std::unique_ptr<Figure> clone() const override;
    bool canMove(sf::Vector2i newpos) const override;
    bool canAttack(sf::Vector2i newpos) const override;
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

