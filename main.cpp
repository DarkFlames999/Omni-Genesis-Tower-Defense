/**
 * @file main.cpp
 * @author Keali Lake
 * @brief  This program testing class buton
 * @version 0.1
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * @aknolegment https://code.markrichards.ninja/sfml/how-to-create-simple-buttons-for-your-sfml-game
 * 
 * 
 */
#include <SFML/Graphics.hpp>
#include <iostream>
#include "button.h"
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 500), "Omni-Genesis/TowerDefense");
    Button play("Play",{200,200}, {450, 250}, sf::Color(112, 3, 0, 255));
    Button custom("Custom", {200,350}, {450, 250}, sf::Color(112, 3, 0, 255));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            custom.update(event, window);
            play.update(event, window);
        }

        window.clear();
        window.draw(play);
        window.draw(custom);
        window.display();
    }

    return 0;
}