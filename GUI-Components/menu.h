/**
 * @file menu.h
 * @author Isaiah Watkins, Konner Knoll, and Keali Lake
 * @brief Menu declarations - buttons, titles, animations, etc.
 * @version 0.1
 * @date 2026-05-05
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <iostream>

enum state {normal, hovered, clicked};

class Button: public sf::Drawable
{
public:
    //Default constructor that loads texture from file “ButtonS.png” and font from “Norse.ttf” file.
    Button();
    //Constructor that sets button label to s, button position to the position,  button size to size (given in pixels), and button color to color.
    Button(std::string s, sf::Vector2f position, sf::Vector2f size, sf::Color color);
    ~Button(){};
    //change button color to color (what else needs to be changed?)
    void setColorButtonNormal(sf::Color buttonNormalColor){mButtonNormal = buttonNormalColor;};
    void setColorButtonClick(sf::Color buttonClickColor){mButtonClick = buttonClickColor;};

    sf::Vector2f getPosition(){return mPosition;};
    sf::Vector2f getDimensions(){return sf::Vector2f(mButton.getGlobalBounds().width, mButton.getGlobalBounds().height);};
    sf::Uint32 getState(){return mBtnState;};

    //Button Color fading at a consistent rate unconditionally (constant rate)
    mutable bool mFading = false;
    mutable float mFadingProgress = 0.0f;
    void setColorButtonFade() const;

    //Vector of colors the buttons must cycle through
    mutable unsigned int mColorIndex = 0;
    mutable std::vector<sf::Color> rainbow =
    {
        sf::Color::Red,
        sf::Color(255, 140, 0, 255),
        sf::Color(255, 255, 0, 255),
        sf::Color::Green,
        sf::Color(0, 255, 255, 255),
        sf::Color::Blue,
        sf::Color(128, 0, 128, 255)
    };

    //This function update the button state and/or look
    void update(sf::Event& e, sf::RenderWindow& window);
    virtual void draw(sf::RenderTarget& target,  sf::RenderStates states) const;

private:
    mutable sf::RectangleShape mBase;
    sf::Sprite mButton;
    sf::Texture mTexture;
    sf::Color mButtonNormal;
    sf::Color mButtonClick;
    sf::Vector2f mPosition;
    sf::Uint32 mBtnState;
    
    //text
    sf::Text mText;
    sf::Font mFont;

};
 
struct Title
{
    sf::Font font;
    sf::Texture titleTexture;
    sf::Clock textClock;
    sf::Clock shrinkClock;
    std::vector<std::string> parts;
    std::string displayedFirst = "";
    std::string displayedSecond = "";
    float titleX = 170.0f;
    float titleY = 170.0f;
    float titleSize = 50.0f;
    bool titleComplete = false;
    bool startPosition = false;
    
    Title(const std::string title);
    void update(sf::RenderWindow& window, float dt);
    void draw(sf::RenderWindow& window);
};

void starAnimation(sf::RenderWindow& window);
#endif