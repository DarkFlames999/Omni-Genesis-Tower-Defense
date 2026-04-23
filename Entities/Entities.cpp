#include "Entities.h"
#include <iostream>
#include <cmath>

//ENTITY CLASS FUNCTIONS
/**
 * @brief Loads all possible textures for the different entities and sets the sprite to the texture,
 * which is used for drawing the entity in the window.    
 * 
 * @param filename 
 * @param texture 
 * @return true 
 * @return false 
 */
bool Entity::loadTextureFromFile(const std::string& filename, sf::Texture& texture)
{
    if(!texture.loadFromFile(filename))
    {
        std::cerr <<"Error opening \"" << filename << "\"!" << std::endl;
        return false;
    }
    mSprite.setTexture(texture);
    return true;
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
    loadTextureFromFile("Sprites/TowerBase.png", mTexture);
    loadTextureFromFile("Sprites/Cannon.png", mCannonTexture);

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
 * @brief Loads the sprite sheet texture and hardcodes the Juvenile enemy with the 
 * correct position, size, and hurtbox.
 * 
 * @param position 
 * @param size 
 * @return true 
 * @return false 
 */
bool Juvenile::createJuvenile(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize)
{
    mFrameCount = 12;
    mSpeed = 10;
    mHealth = 50;
    loadTextureFromFile("Sprites/J_Walking.png", mJuveniles);

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
 * @brief Loads the sprite sheet texture and hardcodes the Matured enemy with the 
 * correct position, size, and hurtbox.
 * 
 * @param window 
 * @param position 
 * @param size 
 * @param spriteSize 
 * @return true 
 * @return false 
 */
bool Matured::createMatured(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize)
{
    mFrameCount = 8;
    mSpeed = 20;
    mHealth = 100;
    loadTextureFromFile("Sprites/M_Walking.png", mMatured);

    mSprite.setTexture(mMatured);
    sf::Vector2u textureSize = mMatured.getSize();
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
 * @brief Loads the sprite sheet texture and hardcodes the Overgrown enemy with the 
 * correct position, size, and hurtbox.
 * 
 * @param window 
 * @param position 
 * @param size 
 * @param spriteSize 
 * @return true 
 * @return false 
 */
// bool Overgrown::createOvergrown(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize)
// {
//     mFrameCount = 7;
//     mSpeed = 30;
//     mHealth = 200;
//     loadTextureFromFile("Sprites/O_Walking.png", mOvergrown);

//     mSprite.setTexture(mOvergrown);
//     sf::Vector2u textureSize = mOvergrown.getSize();
//     float frameWidth  = static_cast<float>(textureSize.x) / mFrameCount;
//     float frameHeight = static_cast<float>(textureSize.y);

//     float scaleX = spriteSize.x / frameWidth;
//     float scaleY = spriteSize.y / frameHeight;
//     mSprite.setScale(scaleX*3, scaleY);

//     // Position in bottom right
//     sf::Vector2u windowSize = window.getSize();
//     mPosition = {
//         windowSize.x - spriteSize.x - 70.f,
//         windowSize.y - spriteSize.y - 40.f
//     };
//     mSprite.setPosition(mPosition);


//     mSize = {(size.x/2)+60.f, (size.y/2)-40.f};
//     mOrigin = {(size.x/2.f)-20.f, (size.y/2.f)-130.f};

//     //Juvenile Hurtbox
//     mHurtbox.setSize(mSize);
//     mHurtbox.setOrigin(mOrigin);
//     mHurtbox.setPosition(mPosition);
//     mHurtbox.setFillColor(sf::Color::Transparent);
//     mHurtbox.setOutlineColor(sf::Color::Red);
//     mHurtbox.setOutlineThickness(1.f);

//     return true;
// }

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
    sf::Vector2u textureSize = mSprite.getTexture()->getSize();
    int frameWidth = textureSize.x / mFrameCount;
    int frameHeight = textureSize.y;
    if(mAnimClock.getElapsedTime().asSeconds() >= mFrameTime)
    {
        mCurrentFrame = (mCurrentFrame+1)%mFrameCount;
        mSprite.setTextureRect(sf::IntRect(mCurrentFrame * frameWidth,0,frameWidth,frameHeight));
        mAnimClock.restart();
    }

    mPosition.x -= mSpeed * mMovement.restart().asSeconds();
    mSprite.setPosition(mPosition);
    mHurtbox.setPosition(mPosition);
}
//ALL BULLET/MAGIC FUNCTIONS
