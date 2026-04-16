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
bool Tower::createTower(sf::Vector2f position)
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

    //Create the base Towers origin and rotation
    sf::FloatRect baseBounds = mSprite.getLocalBounds();
    mOrigin = {baseBounds.width/2.0f, baseBounds.height/2.0f};
    mSprite.setOrigin(mOrigin);
    mSprite.setPosition(position);

    //Create the cannons origin and rotation in the top center of the tower base
    sf::FloatRect cannonBounds = mCannon.getLocalBounds();
    mCannon.setOrigin(cannonBounds.width/2.0f, cannonBounds.height/2.0f);
    mCannon.setPosition(position);

    //Shared data
    mPosition = position;
    mSize = {baseBounds.width, baseBounds.height};

    //Hitbox is only on the base of the tower
    mHurtbox.setSize(mSize);
    mHurtbox.setOrigin(mOrigin);
    mHurtbox.setPosition(mPosition);
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
    mCannon.setRotation(angle+90.0f);
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
 * @brief Literally just drawing the tower, cannon, and the hitbox in the window.
 * 
 * @param target 
 * @param states 
 */
void Tower::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mCannon, states);
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

//ALL BULLET/MAGIC FUNCTIONS

//ALL TOWER FUNCTIONS

//ALL ENEMY FUNCTIONS

//ALL BULLET/MAGIC FUNCTIONS
