/**
 * @file menu.cpp
 * @author Isaiah Watkins, Konner Knoll, and Keali Lake
 * @brief Menu definitions - buttons, titles, animations, etc.
 * @version 0.1
 * @date 2026-05-05
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "menu.h"
#include <sstream>

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
    if (!mTexture.loadFromFile("Sprites/ButtonS.png"))
    {
        std::cerr<<"Error opening file\n";
        exit(1);
    }
    
    //set texture
    mButton.setTexture(mTexture);
    //get size of image
    sf::Vector2u imageSize=mTexture.getSize();
    //set size as a ration of original size
    mButton.setScale(size.x/imageSize.x, size.y/imageSize.y);

    //change origin to the center of the image (makes rotation easy)
    mButton.setOrigin(imageSize.x/2.0f, imageSize.y/2.0f);
    //set position
    mPosition = position;
    mButton.setPosition(mPosition.x, mPosition.y);

    //Make Base Rectangle
    sf::FloatRect bounds = mButton.getGlobalBounds();
    sf::Vector2f scaledSize(bounds.width-40.0f, bounds.height-40.0f);
    mBase.setSize(scaledSize);
    mBase.setOrigin(scaledSize.x / 2.0f, scaledSize.y / 2.0f);
    mBase.setPosition(mPosition.x, mPosition.y);
    mBase.setFillColor(color);

    //Make label
    if (!mFont.loadFromFile("Fonts/Norse.ttf"))
    {
        std::cerr<<"Error opening file\n";
        exit(2);
    }
    mText.setFont(mFont);
    //choose the font size based on button size
    unsigned int fontSize = bounds.height / 4.0f;
    mText.setCharacterSize(fontSize);
    //set label
    mText.setString(s);
    //set origin to the middle
    mText.setOrigin(mText.getGlobalBounds().width/2.0f, mText.getGlobalBounds().height/2.0f);
    //set position at the middle of the button
    mText.setPosition(mPosition.x, mPosition.y-fontSize/4.0f);
    //choose colors
    mButtonNormal = sf::Color::Red;
    mButtonClick = sf::Color(55, 0, 106, 255);
    mText.setFillColor(sf::Color::White);
    mBtnState=state::normal;
}

/**
 * @brief Draws Each individul part of the button (Base, Texture, and Text)
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
        mFadingProgress += 0.25f;  // adjust for fade speed

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

/**
 * @brief Spinning star animation that plays when the window is first created
 * 
 * @param window 
 */
void starAnimation(sf::RenderWindow& window)
{
    //Loading the star texture
    sf::Texture star;
    if(!star.loadFromFile("Sprites/Star.png"))
    {
        std::cerr<<"Error opening \"Star.png\" file" << std::endl;
        exit(1);
    }

    //Create a sprite for the star
    //Origin should be set to the center of the screen so that it scales and rotates from the fixed 
    //point at the center of the screen instead of the top left corner of the star texture
    sf::Sprite starSprite;
    sf::Vector2u windowSize = window.getSize();
    //Make the star bigger than the origina texture
    starSprite.setScale(10.0f, 10.0f);
    starSprite.setTexture(star);
    starSprite.setOrigin(star.getSize().x/2.0f, star.getSize().y/2.0f);
    starSprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    //Make the star take up the entire window and spin clockwise while scaling down to the center of the screen
    //until a minimum size is reached
    for(float scale = 15.0f; scale >= 2.0f; scale -= 0.1f)
    {
        starSprite.setScale(scale, scale);
        starSprite.setRotation(starSprite.getRotation() + 10.0f);
        window.clear(sf::Color::Black);
        window.draw(starSprite);
        window.display();
    }

    // Keep the star displayed for a pre-determined amount of time after the animation is complete
    sf::Clock displayClock;
    while(displayClock.getElapsedTime().asSeconds() < 3.0f)
    {
        window.clear(sf::Color::Black);
        window.draw(starSprite);
        window.display();
    }

    //Set up a white rectangle overlay
    // After the display clock, set up a white overlay rectangle
    sf::RectangleShape whiteOverlay(sf::Vector2f(star.getSize().x*2, star.getSize().y*2));
    whiteOverlay.setOrigin(star.getSize().x / 2.0f, star.getSize().y / 2.0f);
    whiteOverlay.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    whiteOverlay.setFillColor(sf::Color(255, 255, 255, 0)); // start transparent

    //While the star is scaling up and spinning counterclockwise, gradually increase the brightness 
    //of the star by increasing the RGB values of the star's color
    for(float scale = 1.0f; scale <= 15.0f; scale += 0.1f)
    {
        starSprite.setScale(scale, scale);
        starSprite.setRotation(starSprite.getRotation() - 10.0f);
        whiteOverlay.setScale(scale, scale);
        whiteOverlay.setRotation(whiteOverlay.getRotation() - 10.0f);

        sf::Color color = whiteOverlay.getFillColor();
        color.a = std::min(255, color.a + 1);
        whiteOverlay.setFillColor(color);

        window.clear(sf::Color::Black);
        window.draw(starSprite);
        window.draw(whiteOverlay);
        window.display();
    }
}

/**
 * @brief Title appears after a certain amount of time once the window is created and the Star animation is played
 * The title will appear character by character every 100 milliseconds.
 * Once fully displayed, it will shrink and move to the top left corner of the window
 * 
 * @param window 
 * @param textClock 
 * @param T 
 * @param displayedFirst 
 * @param displayedSecond 
 */
Title::Title(const std::string title)
{
//Loading the Font
    if(!font.loadFromFile("Fonts/Norse.ttf"))
    {
        std::cerr<<"Error opening \"Norse.ttf\" file" << std::endl;
        exit(1);
    }

    //Load the Title screenshot and set up the sprite for it
    if(!titleTexture.loadFromFile("Sprites/Title.png"))
    {
        std::cerr<<"Error opening \"Title.png\" file" << std::endl;
        exit(1);
    }

    std::stringstream ss(title);
    std::string token;
    //Split the Title before the Main Loop
    while(std::getline(ss, token, '/'))
    {
        parts.push_back(token); 
    }
}

/**
 * @brief Update the Title's appearance
 * 
 */
void Title::update(sf::RenderWindow& window, float dt)
{
    if (!titleComplete && textClock.getElapsedTime().asMilliseconds() >= 100)
    {
        if (displayedFirst.size() < parts[0].size()) {
            displayedFirst += parts[0][displayedFirst.size()];
        } else if (displayedSecond.size() < parts[1].size()) {
            displayedSecond += parts[1][displayedSecond.size()];
        } else {
            titleComplete = true;
        }
        textClock.restart();
    }

    if (titleComplete)
    {
        if (!startPosition)
        {
            titleX = window.getSize().x / 2.0f;
            titleY = window.getSize().y / 2.0f;
            startPosition = true;
        }

        const float targetX = 100.0f;
        const float targetY = 40.0f;
        const float targetSize = 20.0f;
        const float speed = 5.0f;

        titleX += (targetX - titleX) * speed * dt;
        titleY += (targetY - titleY) * speed * dt;
        titleSize += (targetSize - titleSize) * speed * dt;

        // Snap to target when close enough
        if (std::abs(titleX - targetX) < 0.5f) titleX = targetX;
        if (std::abs(titleY - targetY) < 0.5f) titleY = targetY;
        if (std::abs(titleSize - targetSize) < 0.1f) titleSize = targetSize;
    }
}

/**
 * @brief Draw the Title on the window  
 * 
 * @param window 
 */
void Title::draw(sf::RenderWindow& window)
{
    if (titleComplete)
    {
        sf::Sprite titleSprite;
        titleSprite.setTexture(titleTexture);
        titleSprite.setOrigin(titleTexture.getSize().x / 2.0f, titleTexture.getSize().y / 2.0f);
        titleSprite.setPosition(titleX, titleY);
        titleSprite.setScale(titleSize / 50.0f, titleSize / 50.0f);
        window.draw(titleSprite);
    }
    else
    {
        sf::Text text1;
        text1.setStyle(sf::Text::Bold);
        text1.setFont(font);
        text1.setCharacterSize(50);
        text1.setString(displayedFirst);
        text1.setFillColor(sf::Color::White);

        sf::Text text2;
        text2.setStyle(sf::Text::Bold);
        text2.setFont(font);
        text2.setCharacterSize(50);
        text2.setString(displayedSecond);
        text2.setFillColor(sf::Color::White);

        // Center both lines horizontally, stack them vertically
        sf::FloatRect b1 = text1.getLocalBounds();
        sf::FloatRect b2 = text2.getLocalBounds();
        float cx = window.getSize().x / 2.0f;
        float cy = window.getSize().y / 2.0f;

        text1.setOrigin(b1.width / 2.0f, b1.height / 2.0f);
        text1.setPosition(cx, cy - 40.0f);

        text2.setOrigin(b2.width / 2.0f, b2.height / 2.0f);
        text2.setPosition(cx, cy + 40.0f);

        window.draw(text1);
        window.draw(text2);
    }
}