#include "Entities.h"
#include <iostream>
#include <cmath>

//ENTITY CLASS FUNCTIONS
    void draw(sf::RenderWindow& window)
    {
        
    }
    
    void update(sf::RenderWindow& window)
    {
        
    }

//ALL TOWER FUNCTIONS
/**
 * @brief Loads all of the tower textures and creates the base and cannon of the tower as 
 separate sprite pieces
 * 
 * @param position 
 * @return true 
 * @return false 
 */
bool Tower::createTower(sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize)
{
    //Loading textures for the cannon andthe tower spearately
    if(!mTexture.loadFromFile("Sprites/TowerBase.png"))
    {
        std::cerr <<"Error opening \"TowerBase.png\"!" << std::endl;
        return false;
    }
    if(!mCannonTexture.loadFromFile("Sprites/Cannon.png"))
    {
        std::cerr <<"Error opening \"Cannon.png\"!" << std::endl;
        return false;
    }

    mSprite.setTexture(mTexture);
    sf::FloatRect baseBounds = mSprite.getLocalBounds();

    //Create the base Towers origin and rotation
    mOrigin = {baseBounds.width/2.0f, baseBounds.height/2.0f};
    mSprite.setOrigin(mOrigin);
    mSprite.setPosition(position.x, position.y-50);
    mSprite.setScale(spriteSize.x/6, spriteSize.y/6);

    //Create the cannons origin and rotation in the top center of the tower base
    mCannon.setTexture(mCannonTexture);
    sf::FloatRect cannonBounds = mCannon.getLocalBounds();
    mCannon.setOrigin(cannonBounds.width/2.0f, (cannonBounds.height/2.0f)-45.0f);
    mCannon.setPosition(position.x+5.f, position.y-115.f);
    mCannon.setScale(spriteSize.x/6, spriteSize.y/6);

    //Shared data
    mPosition = position;
    mSize = size;

    //Hutbox is only on the base of the tower
    mHurtbox.setSize(mSize);
    mHurtbox.setOrigin(size.x/2-8, size.y/2);
    mHurtbox.setPosition(position.x, position.y-35);
    mHurtbox.setFillColor(sf::Color::Transparent);
    mHurtbox.setOutlineColor(sf::Color::Red);
    mHurtbox.setOutlineThickness(1.f);

    return true;    
}

/**
 * @brief Rotates the cannon on the tower bassed on the position of the cursor
 * 
 * @param target 
 */
void Tower::aim(sf::Vector2f target)
{
    //Follow the mouse cursor around the window
    sf::Vector2f cannonPos = mCannon.getPosition();
    float angle = atan2(target.y - cannonPos.y, target.x - cannonPos.x) * 180 / 3.14159f;
    mCannon.setRotation(angle + 180.0f);

    //Set boundaries so that it the cannon doesn't clip into the tower
    if(mCannon.getRotation() >= 210.0f && mCannon.getRotation() < 270.0f)
    {
        mCannon.setRotation(210.0f);
    }
    if(mCannon.getRotation() >= 270.0f && mCannon.getRotation() < 330.0f)
    {
        mCannon.setRotation(330.0f);
    }
}

/**
 * @brief Where the different magic bullet functions will be implemented 
 * 
 * @param window 
 */
void Tower::shoot(sf::RenderWindow& window)
{
    //Default bullets to test with
    //Call the function for kindness magic
    //Call the function for bravery magic
}

/**
 * @brief Literally just drawing the tower, cannon, and the hutbox in the window.
 * 
 * @param target 
 * @param states 
 */
void Tower::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mCannon, states);
    target.draw(mSprite, states);
    target.draw(mHurtbox, states);
}

/**
 * @brief Just updates the cursor position in the world
 * 
 * @param window 
 */
void Tower::update(sf::RenderWindow& window)
{
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window); //Where the mouse is in pixel coordinates, which is important for checking if the mouse is hovering over the tower and for clicking on the tower to select it
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel); //Where the mouse is in the world, not just the window, which is important for aiming and shooting projectiles towards the mouse position
    aim(mouseWorld);
}

//ALL ENEMY FUNCTIONS
/**
 * @brief Loads the sprite sheet texture frame-by-frame and creates the base and cannon of the tower as 
 separate sprite pieces
 * 
 * @param position 
 * @param size 
 * @return true 
 * @return false 
 */
bool Juvenile::createJuvenile(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize)
{
    if(!mJuveniles.loadFromFile("Sprites/J_Walking.png"))
    {
        std::cerr <<"Error opening \"J_Walking.png\"!" << std::endl;
        return false;
    }

    mSprite.setTexture(mJuveniles);
    sf::Vector2u textureSize = mJuveniles.getSize();
    float frameWidth  = static_cast<float>(textureSize.x) / mFrameCount;
    float frameHeight = static_cast<float>(textureSize.y);

    float scaleX = spriteSize.x / frameWidth;
    float scaleY = spriteSize.y / frameHeight;
    mSprite.setScale(scaleX*3, scaleY);

    // Position in bottom right
    sf::Vector2u windowSize = window.getSize();
    mPosition = {
        windowSize.x - spriteSize.x - 70.f,
        windowSize.y - spriteSize.y - 40.f
    };
    mSprite.setPosition(mPosition);


    mSize = {(size.x/2)+60.f, (size.y/2)-40.f};
    mOrigin = {(size.x/2.f)-20.f, (size.y/2.f)-130.f};

    //Juvenile Hurtbox
    mHurtbox.setSize(mSize);
    mHurtbox.setOrigin(mOrigin);
    mHurtbox.setPosition(mPosition);
    mHurtbox.setFillColor(sf::Color::Transparent);
    mHurtbox.setOutlineColor(sf::Color::Red);
    mHurtbox.setOutlineThickness(1.f);

    return true;
}

/**
 * @brief Literally just drawing the juvenile enemy, and the hurtbox in the window.
 * 
 * @param target 
 * @param states 
 */
void Enemies::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mHurtbox, states);
}

void Enemies::update(sf::RenderWindow& window)
{
    //All enemy walking animations
    sf::Vector2u textureSize = mJuveniles.getSize();
    int frameWidth = textureSize.x / mFrameCount;
    int frameHeight = textureSize.y;
    if(mAnimClock.getElapsedTime().asSeconds() >= mFrameTime)
    {
        mCurrentFrame = (mCurrentFrame+1)%mFrameCount;
        mSprite.setTextureRect(sf::IntRect(mCurrentFrame * frameWidth,0,frameWidth,frameHeight));
        mAnimClock.restart();
    }
}
//ALL BULLET/MAGIC FUNCTIONS
