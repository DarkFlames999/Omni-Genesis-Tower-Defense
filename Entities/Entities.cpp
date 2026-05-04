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
bool Tower::createTower(sf::RenderWindow& window, sf::Vector2f size, sf::Vector2f spriteSize)
{
    //Loading textures for the cannon andthe tower spearately
    loadTextureFromFile("Sprites/TowerBase.png", mTexture);
    loadTextureFromFile("Sprites/Cannon.png", mCannonTexture);

    mSprite.setTexture(mTexture);
    sf::FloatRect baseBounds = mSprite.getLocalBounds();

    //Create the base Towers origin and rotation
    mOrigin = {baseBounds.width/2.0f, baseBounds.height/2.0f};
    mSprite.setOrigin(mOrigin);
    mSprite.setPosition(window.getSize().x/2.f, (window.getSize().y/2.f)+200.f);
    mSprite.setScale(spriteSize.x/2.5, spriteSize.y/2.5);

    //Create the cannons origin and rotation in the top center of the tower base
    mCannon.setTexture(mCannonTexture);
    sf::FloatRect cannonBounds = mCannon.getLocalBounds();
    mCannon.setOrigin(cannonBounds.width/2.0f, (cannonBounds.height/2.0f)-45.0f);
    mCannon.setPosition(window.getSize().x/2.f+5.f, (window.getSize().y/2.f)+50.f);
    mCannon.setScale(spriteSize.x/2.5, spriteSize.y/2.5);

    //Shared data
    mSize = {size.x+65.f, size.y};

    //Hurtbox is only on the base of the tower
    mHurtbox.setSize(size.x*7, size.y);
    mHurtbox.setOrigin(size.x/2+40.f, size.y/2);
    mHurtbox.setPosition(window.getSize().x/2.13f, (window.getSize().y/2.f)+350.f);
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
    if(mCannon.getRotation() >= 230.0f && mCannon.getRotation() < 270.0f)
    {
        mCannon.setRotation(230.0f);
    }
    if(mCannon.getRotation() >= 270.0f && mCannon.getRotation() < 300.0f)
    {
        mCannon.setRotation(300.0f);
    }
}

/**
 * @brief Where the different magic bullet functions will be implemented 
 * 
 * @param window 
 */
void Tower::shoot(sf::RenderWindow& window)
{
    if(mShootClock.getElapsedTime().asSeconds() < (1.f / mFireRate)) return;
    mShootClock.restart();

    sf::Vector2f cannonPos = mCannon.getPosition();
    float rotationRad = (mCannon.getRotation() - 180.f) * (3.14159f / 180.f);
    sf::Vector2f direction = { std::cos(rotationRad), std::sin(rotationRad) };

    float barrelLength = 50.f;
    sf::Vector2f spawnPoint = cannonPos + (direction * barrelLength);

    auto defaultAttack = std::make_unique<Attack>();
    defaultAttack->createAttack(spawnPoint, direction);
    mAttack.push_back(std::move(defaultAttack)); // move, never copy
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
    mHurtbox.draw(target, states); //Uncomment to see the hurtbox of the tower
}

/**
 * @brief Just updates the cursor position in the world
 * 
 * @param window 
 */
void Tower::update(sf::RenderWindow& window, float deltaTime)
{
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window); //Where the mouse is in pixel coordinates, which is important for checking if the mouse is hovering over the tower and for clicking on the tower to select it
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel); //Where the mouse is in the world, not just the window, which is important for aiming and shooting projectiles towards the mouse position
    aim(mouseWorld);
}

/**
 * @brief Updates the position of the bullets and checks if they are still alive. 
 * If they are not alive, they are removed from the vector of attacks.
 * 
 * @param window 
 * @param deltaTime 
 */
void Tower::updateAttack(sf::RenderWindow& window, float deltaTime)
{
    for(auto& attack : mAttack)
        attack->update(window, deltaTime);

    mAttack.erase(
        std::remove_if(mAttack.begin(), mAttack.end(),
            [](const std::unique_ptr<Attack>& a){ return !a->isAlive(); }),
        mAttack.end());

    
}

/**
 * @brief Draws the bullets in the window
 * 
 * @param target 
 */
void Tower::drawAttack(sf::RenderTarget& target) const
{
    for(const auto& attack : mAttack)
        target.draw(*attack);
}

// bool Tower::towerDestroyed(sf::RenderWindow& window)
// {
//     if(mHP <= 10)
//     {
//         loadTextureFromFile("Sprites/Destroyed.png", mTexture);
//     if(!mTexture.loadFromFile("Fonts/Norse.ttf"))
//     {
//         std::cerr<<"Error opening \"Magic.ttf\" file" << std::endl;
//         exit(1);
//     }
//         return true;
//     }
//     return false;
// }

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
    mSpeed = 100;
    mHealth = 50;
    mDamage = 10;
    mXPValue = 10;
    loadTextureFromFile("Sprites/J_Walking.png", mJuveniles);

    mSprite.setTexture(mJuveniles);
    
    sf::Vector2u textureSize = mJuveniles.getSize();
    float frameWidth  = textureSize.x / mFrameCount;
    float frameHeight = textureSize.y;

    float scaleX = spriteSize.x / frameWidth;
    float scaleY = spriteSize.y / frameHeight;

    mSprite.setScale(scaleX*3, scaleY);

    // Position in bottom right
    mPosition = position;
    mSprite.setPosition(mPosition);

    mSize = {(size.x/2)+180.f, (size.y/2)-20.f};
    mOrigin = {(size.x/2.f)-20.f, (size.y/2.f)-150.f};

    //Juvenile Hurtbox
    mHurtbox.setSize(mSize.x, mSize.y);
    mHurtbox.setOrigin(mOrigin.x, mOrigin.y);
    mHurtbox.setPosition(mPosition.x, mPosition.y);
    mHurtbox.setFillColor(sf::Color::Transparent);
    mHurtbox.setOutlineColor(sf::Color::Red);
    mHurtbox.setOutlineThickness(1.f);

    return true;
}

// void Juvenile::juvenileAttack(sf::RenderWindow& window)
// {
//
// }

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
    mDamage = 20;
    mXPValue = 25;
    loadTextureFromFile("Sprites/M_Walking.png", mMatured);

    mSprite.setTexture(mMatured);
    sf::Vector2u textureSize = mMatured.getSize();
    float frameWidth  = static_cast<float>(textureSize.x) / mFrameCount;
    float frameHeight = static_cast<float>(textureSize.y);

    float scaleX = spriteSize.x / frameWidth;
    float scaleY = spriteSize.y / frameHeight;
    mSprite.setScale(scaleX*3, scaleY);

    mSprite.setPosition(mPosition);


    mSize = {(size.x/2)+60.f, (size.y/2)-40.f};
    mOrigin = {(size.x/2.f)-20.f, (size.y/2.f)-130.f};

    //Matured Hurtbox
    mHurtbox.setSize(mSize.x, mSize.y);
    mHurtbox.setOrigin(mOrigin.x, mOrigin.y);
    mHurtbox.setPosition(mPosition.x, mPosition.y);
    mHurtbox.setFillColor(sf::Color::Transparent);
    mHurtbox.setOutlineColor(sf::Color::Red);
    mHurtbox.setOutlineThickness(1.f);

    return true;
}

/**
 * @brief Loads the sprite sheet texture and hardcodes the Warden enemy with the 
 * correct position, size, and hurtbox.
 * 
 * @param window 
 * @param position 
 * @param size 
 * @param spriteSize 
 * @return true 
 * @return false 
 */
bool Warden::createWarden(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize)
{
    mFrameCount = 7;
    mSpeed = 10;
    mHealth = 200;
    mDamage = 40;
    mXPValue = 50;
    loadTextureFromFile("Sprites/W_Walking.png", mWarden);

    mSprite.setTexture(mWarden);
    sf::Vector2u textureSize = mWarden.getSize();
    float frameWidth  = static_cast<float>(textureSize.x) / mFrameCount;
    float frameHeight = static_cast<float>(textureSize.y);

    float scaleX = spriteSize.x / frameWidth;
    float scaleY = spriteSize.y / frameHeight;
    mSprite.setScale(scaleX*3, scaleY);

    mSprite.setPosition(mPosition);


    mSize = {(size.x/2)+60.f, (size.y/2)-40.f};
    mOrigin = {(size.x/2.f)-20.f, (size.y/2.f)-130.f};

    //Warden Hurtbox
    mHurtbox.setSize(mSize.x, mSize.y);
    mHurtbox.setOrigin(mOrigin.x, mOrigin.y);
    mHurtbox.setPosition(mPosition.x, mPosition.y);
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
    mHurtbox.draw(target, states);
}

/**
 * @brief Updates the enemy's position and animation.
 * 
 * @param window 
 */
void Enemies::update(sf::RenderWindow& window, float deltaTime)
{
    //All enemy walking animations
    if(!mSprite.getTexture()) 
    {
        return;
    }
    if(deltaTime > 0.05f) 
    {
        deltaTime = 0.05f;
    }

    sf::Vector2u textureSize = mSprite.getTexture()->getSize();
    int frameWidth  = textureSize.x / mFrameCount;
    int frameHeight = textureSize.y;

    if(mAnimClock.getElapsedTime().asSeconds() >= mFrameTime)
    {
        mCurrentFrame = (mCurrentFrame + 1) % mFrameCount;
        mSprite.setTextureRect(sf::IntRect(mCurrentFrame * frameWidth, 0, frameWidth, frameHeight));
        mAnimClock.restart();
    }

    // Stop the enemy and the hurt and hitboxes when it hits the tower bounds
    mPosition.x -= mSpeed * deltaTime;
    mSprite.setPosition(mPosition.x, mPosition.y);
    mHurtbox.setPosition(mPosition.x, mPosition.y);

    if(mPosition.x <= (window.getSize().x/2.f) + 100.f)
    {
        mPosition.x = (window.getSize().x/2.f) + 100.f;
        mSprite.setPosition(mPosition.x, mPosition.y);
        mHurtbox.setPosition(mPosition.x, mPosition.y);
    }

    //Remove the enemies from the vector if their health is below 1
    if(!mSprite.getTexture()) 
    {
        return;
    }
    if(isDead()) 
    {
        return;
    }
}

//ALL BULLET/MAGIC FUNCTIONS 
/**
 * @brief Creates default yellow bullets that spawn a few inches inside of the cannon and move in the direction of the cannon's rotation. 
 * The size of the bullet is 10x10 and the origin is in the center of the bullet so that it rotates around its center. 
 * The bullets are set to alive when they are created, and will be set to not alive when they go off the screen so that they can be deleted.
 * 
 * @param position 
 * @param direction 
 * @return true 
 * @return false 
 */
bool Attack::createAttack(sf::Vector2f position, sf::Vector2f direction)
{
    mDirection = direction;
    mPosition = position;
    mDamage = 15.f;

    mBulletShape.setRadius(10.f);
    mBulletShape.setOrigin({5.f, 5.f});
    mBulletShape.setPosition(mPosition);
    mBulletShape.setFillColor(sf::Color::White);

    //Create the bullet's hitbox here and set it to the same position as the bullet
    mHitbox.setSize({20.f, 20.f});
    mHitbox.setOrigin({10.f, 10.f});
    mHitbox.setPosition(mPosition);
    mHitbox.setFillColor(sf::Color::Transparent);;
    mHitbox.setOutlineColor(sf::Color::Blue);
    mHitbox.setOutlineThickness(2.f);

    return true;
}

/**
 * @brief Moves the bullet in the direction of the velocity, and if the bullet goes off the screen, 
 * it is set to not alive so that it can be deleted.
 * 
 * @param window 
 * @param deltaTime 
 */
void Attack::update(sf::RenderWindow& window, float deltaTime)
{
    //Move the attack in the direction of the velocity and include the hitbox in the movement
    mPosition += mDirection * mSpeed * deltaTime;
    mBulletShape.setPosition(mPosition);
    mHitbox.setPosition(mPosition);

    //If the attack goes off the screen, set it to not alive so that the bullet and hitbox can be deleted
    
    sf::Vector2u windowSize = window.getSize();
    if(mPosition.x < 0 || mPosition.x > windowSize.x 
     || mPosition.y < 0 || mPosition.y > windowSize.y)
    {
        mAlive = false;
    }
}

/**
 * @brief Draws the attack on the target.
 * 
 * @param target 
 * @param states 
 */
void Attack::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mBulletShape, states);
    mHitbox.draw(target, states); //Uncomment to see the hitbox of the bullet
}
