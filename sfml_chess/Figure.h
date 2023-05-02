#pragma once
#include <memory>
#include <SFML/System/Vector2.hpp>
#include "Movement.h"




class Figure
{
public:
	enum class Type : short
	{
		// Пешка, Слон,  Конь, Ладья, Король, Королева(Ферзь)
		Null, Pawn, Bishop, Knight, Rook, King, Queen
	}; 
	enum class Color : short
	{
		Null, White, Black
	};

	Figure(Type type, Color color, sf::Vector2i pos, char mark, class Board& board);
	virtual ~Figure();
	char getMark() const;

	virtual std::unique_ptr<Figure> clone() const = 0;
	Type getType() const;
	Color getColor() const;
	sf::Vector2i getPosition() const;
	void move(sf::Vector2i newpos);
	virtual Movement canMove(sf::Vector2i newpos);
	virtual bool canAttack(sf::Vector2i newpos) const;

private:
	Type type;
	Color color;
	char mark;
protected:
	sf::Vector2i pos;
	//sf::Sprite sprite;
	class Board& board;

};

