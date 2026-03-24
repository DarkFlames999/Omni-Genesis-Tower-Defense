/**
 * @file main.cpp
 * @author Ekaterina Miller
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
#include "GUI-Components/button.h"
#include "Handlers/InputHandler/InputHandler.h"

int main()
{
   sf::RenderWindow window(sf::VideoMode({800, 500}), "Omni-Genesis-Tower-Defense", sf::Style::Titlebar | sf::Style::Close);
   InputHandler inputHandler;
   inputHandler.ImplementHeldKey(sf::Keyboard::W, 1);
   inputHandler.ImplementDoubleTap(sf::Keyboard::Num1, 0.5);

    sf::RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
    window.setFramerateLimit(60);

   while (window.isOpen())
   {
       sf::Event event;
       
       inputHandler.update(event);

       while (window.pollEvent(event))
       {
           if (event.type == sf::Event::Closed){
               window.close();
            };

            if(inputHandler.isHeldKey(inputHandler.mHeldKeyBindings[0], event) == true)
            {
                
                rectangle.setFillColor(sf::Color::Red);
                rectangle.setPosition(350.f, 200.f);
            }

            if(inputHandler.isDoubleTap(inputHandler.mDoubleTapBindings[0], event) == true)
            {
                rectangle.setFillColor(sf::Color::Blue);
                rectangle.setPosition(200.f, 400.f);
            }

       }

       window.clear();

       window.draw(rectangle);

       window.display();
   }

   return 0;
}