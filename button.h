#ifndef BUTTON_H
#define BUTTON_H
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

#endif
