#pragma once
#include "Frame.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class Endgame :
    public Frame
{

    bool win;

    sf::RectangleShape rect;

    sf::RectangleShape menu_button;
    sf::Text menu_text;

    sf::Text sentence_text;
public:
    Endgame(class Engine& engine, bool win);



    // Унаследовано через Frame
    virtual void process_event(sf::Event event) override;

    virtual void render() override;

};

