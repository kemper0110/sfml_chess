#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

#include "Figures_fwd.h"


class Figure
	:
	public sf::Drawable
{
public:
	enum class Type {
		// Пешка, Слон,  Конь, Ладья, Король, Королева(Ферзь)
		Null, Pawn, Bishop, Knight, Rook, King, Queen
	}; 
	enum class Color 
	{
		Null, White, Black
	};

	Figure(Type type, Color color, sf::Vector2i pos, Board& board);

	virtual std::unique_ptr<Figure> clone() const = 0;
	Type getType() const;
	Color getColor() const;
	sf::Vector2i getPosition() const;
	void move(sf::Vector2i newpos);
	virtual bool canMove(sf::Vector2i newpos) const;
	virtual bool canAttack(sf::Vector2i newpos) const;

private:
	Type type;
	Color color;
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Vector2i pos;
	sf::Sprite sprite;
	Board& board;

private:
	static bool loadTexture();
public:
	static inline sf::Texture texture = sf::Texture();
	const static inline bool isTextureLoaded = loadTexture();
	const static inline sf::Vector2f board_offset = sf::Vector2f(28, 28);
};

