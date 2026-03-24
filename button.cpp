#include "button.h"

/**
 * @brief Construct a new Button:: Button object
 * 
 * @param s 
 * @param position 
 * @param size 
 * @param color 
 */
Button::Button(std::string s, sf::Vector2f position, sf::Vector2f size, sf::Color color)
{
    if (!mTexture.loadFromFile("ButtonS.png"))
    {
        std::cout<<"Error opening file\n";
        exit(1);
    }   
    
    //set texture
    mButton.setTexture(mTexture);
    //get size of image
    sf::Vector2u imageSize=mTexture.getSize();
    //set size as a ration of original size
    mButton.setScale((size.x/imageSize.x)-4.5, (size.y/imageSize.y)-3.5);

    //change origin to the center of the image (makes rotation easy)
    mButton.setOrigin(imageSize.x/2, imageSize.y/2);
    //set position
    mPosition = position;
    mButton.setPosition(mPosition.x, mPosition.y);

    //Make Base Rectangle
    sf::Vector2f scaledSize(mButton.getGlobalBounds().width-250, mButton.getGlobalBounds().height-370);
    mBase.setSize(scaledSize);
    mBase.setOrigin(scaledSize.x / 2, scaledSize.y / 2);
    mBase.setPosition(mPosition.x, mPosition.y);
    mBase.setFillColor(color);

    //Make label
    if (!mFont.loadFromFile("Norse.ttf"))
    {
        std::cout<<"Error opening file\n";
        exit(2);
    }
    mText.setFont(mFont);
    //choose the font size based on button size (I choose half)
    unsigned int fontSize = mButton.getGlobalBounds().height/14;
    mText.setCharacterSize(fontSize);
    //set label
    mText.setString(s);
    //set origin to the middle
    mText.setOrigin(mText.getGlobalBounds().width/2, mText.getGlobalBounds().height/2);
    //set position at the middle of the button
    mText.setPosition(mPosition.x, mPosition.y-fontSize/4);
    //choose colors
    mButtonNormal = sf::Color::Red;
    mButtonClick = sf::Color(55, 0, 106, 255);
    mText.setFillColor(sf::Color::White);
    mBtnState=state::normal;
}

/**
 * @brief Draws Each individul part of the button
 * 
 * @param target 
 * @param states 
 */
void Button::draw(sf:: RenderTarget& target, sf:: RenderStates states) const
{
    setColorButtonFade();
    target.draw(mBase, states);
    target.draw(mButton, states);
    target.draw(mText, states);
}

/**
 * @brief Updates the button state and look so that it shifts through all colors of the rainbow before
 * finally settling on purple
 * 
 * @param e 
 * @param window 
 */
void Button::update(sf::Event& e, sf::RenderWindow& window)
{
    //get position of the mouse

    sf::Vector2i mPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosition = window.mapPixelToCoords(mPos);
    bool mouseInButton =    mousePosition.x >= mBase.getPosition().x - mBase.getGlobalBounds().width/2
                            && mousePosition.x <= mBase.getPosition().x + mBase.getGlobalBounds().width/2
                            && mousePosition.y >= mBase.getPosition().y - mBase.getGlobalBounds().height/2
                            && mousePosition.y <= mBase.getPosition().y + mBase.getGlobalBounds().height/2;
    if (e.type == sf::Event::MouseButtonPressed)
    {
        if(e.mouseButton.button==sf::Mouse::Left)
        {
            if(mouseInButton)
            {
                mFading = true;
            }
        }
    }
}

/**
 * @brief This is where the fading animation is so that the base button cycles through all 
 * of the colors of the rainbow using the vector of colors
 * 
 */
void Button::setColorButtonFade() const
{
    if (mFading && mColorIndex < rainbow.size()-1)
    {
        mFadingProgress += 0.01f;  // adjust for fade speed

        if(mFadingProgress >= 1.0f)
        {
            mFadingProgress = 0.0f;
            mColorIndex++;
        }

        sf::Color from = rainbow[mColorIndex];
        sf::Color to = rainbow[mColorIndex+1];

        sf::Color fadedColor;
        fadedColor.r = from.r + (to.r - from.r) * mFadingProgress;
        fadedColor.g = from.g + (to.g - from.g) * mFadingProgress;
        fadedColor.b = from.b + (to.b - from.b) * mFadingProgress;
        fadedColor.a = 255;

        mBase.setFillColor(fadedColor);
    }
}
