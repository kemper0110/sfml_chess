#pragma once
#include "Figure.h"



class Bishop :
    public Figure
{
public:
    Bishop(Figure::Color color, sf::Vector2i pos, Board& board);
    std::unique_ptr<Figure> clone() const override;
    Movement canMove(sf::Vector2i newpos) override;
    bool canAttack(sf::Vector2i newpos) const override;
protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

