#pragma once
#include "Figure.h"




class Queen :
    public Figure
{
public:
    Queen(Figure::Color color, sf::Vector2i pos, class Board& board);
    std::unique_ptr<Figure> clone() const override;
    Movement canMove(sf::Vector2i newpos) override;
    bool canAttack(sf::Vector2i newpos) const override;
};

