#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "Figures_fwd.h"
//#include "FigureMoves.h"
#include "Movement.h"


class Figure
{
public:
	enum class Type : short
	{
		// �����, ����,  ����, �����, ������, ��������(�����)
		Null, Pawn, Bishop, Knight, Rook, King, Queen
	}; 
	enum class Color : short
	{
		Null, White, Black
	};

	Figure(Type type, Color color, sf::Vector2i pos, char mark, Board& board);
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
	Board& board;

private:
	static bool loadTexture();
	static bool loadSprite();
public:
	static inline sf::Texture texture = sf::Texture();
	const static inline bool isTextureLoaded = loadTexture();
	static inline sf::Sprite sprite = sf::Sprite();
	const static inline bool isSpriteLoaded = loadSprite();
};

