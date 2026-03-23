
#include "button.h"
#include <SFML/Graphics.hpp>

    Button::Button():Button("Push Me!", {300,100}, {30, 10}, sf::Color::White)
    {

        mButton.setScale(1,1);

        unsigned int fontSize = mButton.getGlobalBounds().height/2;
        mText.setCharacterSize(fontSize);

        mText.setOrigin(mText.getGlobalBounds().width/2, mText.getGlobalBounds().height/2);
        mText.setPosition(mPosition.x, mPosition.y-fontSize/4);
    }

    //Constructor that sets button label to s, button position to the position,  button size to size (given in pixels), and button color to color.
    Button::Button(std::string s, sf::Vector2f position, sf::Vector2f size, sf::Color color)
    {
        if (!mTexture.loadFromFile("button.png"))
        {
            std::cout<<"Error opening file\n";
            exit(1);
        }
        mButton.setTexture(mTexture);
        //get size of image
        sf::Vector2u imageSize=mTexture.getSize();
        //change origin to the center of the image (makes rotation easy)
        mButton.setOrigin(imageSize.x/2, imageSize.y/2);
        //set position
        mButton.setPosition(position.x,position.y);
        // choose color
        mButton.setColor(color);
        // set size as a ration of original size
        mButton.setScale(size.x/mButton.getGlobalBounds().width, size.y/mButton.getGlobalBounds().height);
        //Make label
        if (!mFont.loadFromFile("college.ttf"))
        {
            std::cout<<"Error opening file\n";
            exit(2);
        }

        mText.setFont(mFont);
    //choose the font size based on button size (I choose half)
        unsigned int fontSize = mButton.getGlobalBounds().height/2;
        mText.setCharacterSize(fontSize);
        //set label
        mText.setString(s);
        //set origin to the middle
        mText.setOrigin(mText.getGlobalBounds().width/2, mText.getGlobalBounds().height/2);
        //set position at the middle of the button
        mText.setPosition(mButton.getPosition().x, mButton.getPosition().y-fontSize/4);
        //choose colors
        mTextNormal = sf::Color::Green;
        mTextHover = sf::Color::Red;

        mText.setFillColor(mTextNormal);
        mBtnState = state::normal;


    }
    void Button::setPosition(sf::Vector2f position) {
        mPosition = position;
        mButton.setPosition(position);  // Actually move the sprite

        // Recalculate text position relative to the new button position
        unsigned int fontSize = mButton.getGlobalBounds().height / 2;
        mText.setOrigin(mText.getGlobalBounds().width / 2, mText.getGlobalBounds().height / 2);
        mText.setPosition(mButton.getPosition().x, mButton.getPosition().y - fontSize / 4);
    }

    void Button::setSize(sf::Vector2f size) {
        mSize = size;
        mButton.setScale(size.x / mTexture.getSize().x, size.y / mTexture.getSize().y);
    }

    void Button::setColor(sf::Color Color) {
        mButtonColor = Color;
        mButton.setColor(Color);
    }

    void Button::setText(std::string s) {
        mText.setString(s);
        mText.setOrigin(mText.getGlobalBounds().width / 2, mText.getGlobalBounds().height / 2);

        unsigned int fontSize = mButton.getGlobalBounds().height / 2;
        mText.setPosition(mButton.getPosition().x, mButton.getPosition().y - fontSize / 4);
    }

    //This function update the button state and/or look
    void Button::update(sf::Event& e, sf::RenderWindow& window)
    {
            //get position of the mouse
            // sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2i mPos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosition = window.mapPixelToCoords(mPos);
            bool mouseInButton =    mousePosition.x >= mButton.getPosition().x - mButton.getGlobalBounds().width/2
                                    && mousePosition.x <= mButton.getPosition().x + mButton.getGlobalBounds().width/2
                                    && mousePosition.y >= mButton.getPosition().y - mButton.getGlobalBounds().height/2
                                    && mousePosition.y <= mButton.getPosition().y + mButton.getGlobalBounds().height/2;
            if(e.type == sf::Event::MouseMoved)
            {
                if(mouseInButton)
                {
                    mText.setFillColor(mTextHover);
                }
                else
                {
                    mText.setFillColor(mTextNormal);
                }
            }
            if (e.type == sf::Event::MouseButtonPressed)
            {
                if(e.mouseButton.button == sf::Mouse::Left)
                {
                    if(mouseInButton)
                    {
                        mButton.setRotation(180);
                    }
                    else
                    {
                        mButton.setRotation(0);
                    }
                }
            }
            if (e.type == sf::Event::MouseButtonReleased)
            {
                if (e.mouseButton.button == sf::Mouse::Left)
                {
                    if(mouseInButton)
                    {
                        mText.setFillColor(mTextHover);
                        mButton.setRotation(0);
                    }
                    else
                    {
                        mText.setFillColor(mTextNormal);
                        mButton.setRotation(0);
                    }
                }
            }
    };

    void Button::draw(sf::RenderTarget& target,sf::RenderStates states) const
    {target.draw(mButton, states);
    target.draw(mText, states);};