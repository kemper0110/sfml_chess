#pragma once
#include "Figure.h"

#include "Figures_fwd.h"
#include "Movement.h"



class Rook :
    public Figure
{
public:
    Rook(Figure::Color color, sf::Vector2i pos, Board& board);
    std::unique_ptr<Figure> clone() const override;
    Movement canMove(sf::Vector2i newpos) override;
    bool canAttack(sf::Vector2i newpos) const override;
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

