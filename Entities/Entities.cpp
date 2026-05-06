/**
 * @file Entities.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-05-05
 * 
 * @copyright Copyright (c) 2026
 * 
 */

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
    loadTextureFromFile("Sprites/TowerStats.png", mHealthTexture);
    loadTextureFromFile("Sprites/Fire.png", mFireBulletTexture);

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
    // mHurtbox.setOutlineColor(sf::Color::Red);
    // mHurtbox.setOutlineThickness(1.f);

    //Health bar attributes
    const sf::Vector2f frameSize = {500.f, 80.f}; 
    const sf::Vector2f framePosition = {        
        window.getSize().x / 2.f - frameSize.x / 2.f, 
        window.getSize().y - frameSize.y - 10.f
    };

    const sf::Vector2f healthFillSize = {400.f, 10.f}; 
    const sf::Vector2f healthFillPosition = {      
        framePosition.x + 50.f,       
        framePosition.y + 20.f      
    };

    const sf::Vector2f stabilityFillSize = {360.f, 10.f};
    const sf::Vector2f stabilityFillPosition = {   
        framePosition.x + 70.f,  
        framePosition.y + 50.f                             
    };

    // Setup the frame sprite
    mHealthBar.setTexture(mHealthTexture);
    // Scale the frame to the desired size
    sf::Vector2u texSize = mHealthTexture.getSize(); // Get original texture size
    float scaleX = frameSize.x / texSize.x;          // Scale factor for width
    float scaleY = frameSize.y / texSize.y;          // Scale factor for height
    mHealthBar.setScale(scaleX, scaleY);             // Apply scaling
    mHealthBar.setPosition(framePosition);

    // Setup health fill (red part that decreases)
    mHealthFill.setSize(healthFillSize);
    mHealthFill.setFillColor(sf::Color::Red);
    mHealthFill.setPosition(healthFillPosition);

    // Setup stability fill (blue part)
    mStabilityFill.setSize(stabilityFillSize);
    mStabilityFill.setFillColor(sf::Color::Cyan);
    mStabilityFill.setPosition(stabilityFillPosition);

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
    defaultAttack->setColor(mBulletColor);
    defaultAttack->setDamage(mBulletDamage * mDamageMultiplier);
    defaultAttack->setBulletSpeed(mBulletSpeed);

    defaultAttack->setSpriteTexture(mBulletTexturePtr);
    mAttack.push_back(std::move(defaultAttack));
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
    mHurtbox.draw(target, states);
    target.draw(mStabilityFill, states);
    target.draw(mHealthFill, states);
    target.draw(mHealthBar, states);
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

 
    float healthRatio = std::max(0.f, mHP / mMaxHP);
    mHealthFill.setSize({mMaxHealthBarWidth * healthRatio, 20.f});

    if(healthRatio > 0.5f)
    {
        mHealthFill.setFillColor(sf::Color::Green);
    }
    else if(healthRatio > 0.25f)
    {
        mHealthFill.setFillColor(sf::Color::Yellow);
    }
    else
    {
        mHealthFill.setFillColor(sf::Color::Red);
    }

    //Stability bar update
    float stabilityRatio = std::max(0.f, mStability / mMaxStability);
    mStabilityFill.setSize({mMaxStabilityBarWidth * stabilityRatio, 20.f});
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

bool Tower::spendXP(int amount)
{
    if (mXPPoints >= amount) {
        mXPPoints -= amount;
        return true;
    }
    return false;
}

void Tower::setBulletColor(sf::Color color) 
{ 
    mBulletColor = color;
}
void Tower::setBulletDamage(float damage) 
{ 
    mBulletDamage = damage;
}

void Tower::setBulletSpeed(float speed) 
{ 
    mBulletSpeed = speed;
}

void Attack::setSpriteTexture(const sf::Texture* tex) {
    if (!tex) {
        mUseSprite = false;
        return;
    }
    float angleDeg = std::atan2(mDirection.y, mDirection.x) * (180.f / 3.14159f);
    mBulletSprite.setRotation(angleDeg);

    mBulletSprite.setTexture(*tex);
    sf::Vector2u texSize = tex->getSize();
    mBulletSprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);

    float diameter = computeVisualSize();
    mBulletSprite.setScale(diameter / texSize.x, diameter / texSize.y);
    
    mUseSprite = true;
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
    mAttackFrameCount = 5;
    mSpeed = 100.f;
    mHealth = 50.f;
    mDamage = 10.f;
    mXPValue = 10.f;

    loadTextureFromFile("Sprites/J_Walking.png", mJuveniles);
    mWalkTextureRef = &mJuveniles;

    if (mAttackTexture.loadFromFile("Sprites/JuvenileAttack.png"))
    {
        mHasAttackSprite = true;
    }
    else
    {
        std::cerr << "Warning: failed to load JuvenileAttack.png\n";
    }

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
    // mHurtbox.setOutlineColor(sf::Color::Red);
    // mHurtbox.setOutlineThickness(1.f);

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
    mAttackFrameCount = 5;
    mSpeed = 120.f;
    mHealth = 100.f;
    mDamage = 20.f;
    mXPValue = 25.f;

    loadTextureFromFile("Sprites/M_Walking.png", mMatured);
    mWalkTextureRef = &mMatured;

    if (mAttackTexture.loadFromFile("Sprites/MaturedAttack.png"))
    {
        mHasAttackSprite = true;
    }
    else
    {
        std::cerr << "Warning: failed to load MaturedAttack.png\n";
    }

    mSprite.setTexture(mMatured);
    sf::Vector2u textureSize = mMatured.getSize();
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
    // mHurtbox.setOutlineColor(sf::Color::Red);
    // mHurtbox.setOutlineThickness(1.f);

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
    // mHasAttackSprite = 5; not done yet!
    mSpeed = 140.f;
    mHealth = 200.f;
    mDamage = 40.f;
    mXPValue = 50.f;

    loadTextureFromFile("Sprites/W_Walking.png", mWarden);
    mWalkTextureRef = &mWarden;

    mSprite.setTexture(mWarden);
    sf::Vector2u textureSize = mWarden.getSize();
    float frameWidth  = textureSize.x / mFrameCount;
    float frameHeight = textureSize.y;

    float scaleX = spriteSize.x / frameWidth;
    float scaleY = spriteSize.y / frameHeight;

    mSprite.setScale(scaleX*3*(-1), scaleY); //Must fix

    // Position in bottom right
    mPosition = position;
    mSprite.setPosition(mPosition);

    mSize = {(size.x/2)+40.f, (size.y/2)+80.f};
    mOrigin = {(size.x/2.f)-20.f, (size.y/2.f)-150.f};

    //Juvenile Hurtbox
    mHurtbox.setSize(mSize.x, mSize.y);
    mHurtbox.setOrigin(mOrigin.x, mOrigin.y);
    mHurtbox.setPosition(mPosition.x, mPosition.y);
    mHurtbox.setFillColor(sf::Color::Transparent);
    // mHurtbox.setOutlineColor(sf::Color::Red);
    // mHurtbox.setOutlineThickness(1.f);


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

    if(!mSprite.getTexture()) return;
    if(deltaTime > 0.05f) deltaTime = 0.05f;

    if (mIsAttacking &&
        mAttackAnimTimer.getElapsedTime().asSeconds() >= Attackanimationduration)
    {
        mIsAttacking = false;
        if (mHasAttackSprite)
            swapToWalkTexture();
        mCurrentFrame = 0;
        mAnimClock.restart();
    }


    int activeFrameCount =
        (mIsAttacking && mHasAttackSprite) ? mAttackFrameCount : mFrameCount;
    if (activeFrameCount <= 0) activeFrameCount = 1;

    sf::Vector2u textureSize = mSprite.getTexture()->getSize();
    int frameWidth  = textureSize.x / activeFrameCount;
    int frameHeight = textureSize.y;

    if(mAnimClock.getElapsedTime().asSeconds() >= mFrameTime)
    {
        mCurrentFrame = (mCurrentFrame + 1) % activeFrameCount;
        mSprite.setTextureRect(sf::IntRect(mCurrentFrame * frameWidth, 0, frameWidth, frameHeight));
        mAnimClock.restart();
    }

    if (!mIsAttacking)
    {
        if(mSpawnedLeft)
            mPosition.x += mSpeed * deltaTime;
        else
            mPosition.x -= mSpeed * deltaTime;
    }

    mSprite.setPosition(mPosition.x, mPosition.y);
    mHurtbox.setPosition(mPosition.x, mPosition.y);

    float enemySpriteWidth = mHurtbox.getGlobalBounds().width - 5.f;

    if(!mSpawnedLeft && mPosition.x <= mTowerBounds.left + mTowerBounds.width)
    {
        mPosition.x = mTowerBounds.left + mTowerBounds.width - 5;
        mSprite.setPosition(mPosition.x, mPosition.y);
        mHurtbox.setPosition(mPosition.x, mPosition.y);
    }
    if(mSpawnedLeft && mPosition.x + enemySpriteWidth >= mTowerBounds.left)
    {
        mPosition.x = mTowerBounds.left - enemySpriteWidth;
        mSprite.setPosition(mPosition.x, mPosition.y);
        mHurtbox.setPosition(mPosition.x, mPosition.y);
    }

    if(isDead()) return;
}

/**
 * @brief Attempts a bite attack on the tower. Respects the 2s cooldown
 * and triggers attack animation if the enemy has an attack sprite.
 */
void Enemies::tryAttack(Tower& tower)
{
    if (mIsAttacking) return;

    // Cooldown gate
    if (mAttackCooldown.getElapsedTime().asSeconds() < Attackcooldown) return;

    mIsAttacking = true;
    mCurrentFrame = 0;
    mAnimClock.restart();
    mAttackAnimTimer.restart();
    mAttackCooldown.restart();

    if (mHasAttackSprite)
        swapToAttackTexture();

    tower.takeDamage(mDamage);
    std::cout << "Bite! Tower health: " << tower.getHealth() << std::endl;
}

void Enemies::swapToAttackTexture()
{
    if (!mHasAttackSprite) return;

    sf::Vector2f currentScale = mSprite.getScale();
    mSprite.setTexture(mAttackTexture, true);

    sf::Vector2u texSize = mAttackTexture.getSize();
    if (mAttackFrameCount <= 0) return;
    int frameWidth  = static_cast<int>(texSize.x) / mAttackFrameCount;
    int frameHeight = static_cast<int>(texSize.y);

    mSprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

    // Maintain flip-by-origin behavior set up in setSpawnSide
    if (mSpawnedLeft)
        mSprite.setOrigin(static_cast<float>(frameWidth), 0.f);
    else
        mSprite.setOrigin(0.f, 0.f);

    mSprite.setScale(currentScale);
}

void Enemies::swapToWalkTexture()
{
    if (!mWalkTextureRef) return;

    sf::Vector2f currentScale = mSprite.getScale();
    mSprite.setTexture(*mWalkTextureRef, true);

    sf::Vector2u texSize = mWalkTextureRef->getSize();
    if (mFrameCount <= 0) return;
    int frameWidth  = static_cast<int>(texSize.x) / mFrameCount;
    int frameHeight = static_cast<int>(texSize.y);

    mSprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

    if (mSpawnedLeft)
        mSprite.setOrigin(static_cast<float>(frameWidth), 0.f);
    else
        mSprite.setOrigin(0.f, 0.f);

    mSprite.setScale(currentScale);
}

/**
 * @brief Set the side the enemies spawn on
 * 
 * @param spawnedLeft 
 */
void Enemies::setSpawnSide(bool spawnedLeft)
{

    mSpawnedLeft = spawnedLeft;

    sf::Vector2f scale = mSprite.getScale();

    if(mSpawnedLeft)
    {
        // Get the current frame width to offset the flip anchor
        sf::Vector2u textureSize = mSprite.getTexture()->getSize();
        float frameWidth = static_cast<float>(textureSize.x) / mFrameCount;

        // Move origin to right edge of the frame so negating scaleX flips in place
        mSprite.setOrigin(frameWidth, 0.f);
        mSprite.setScale(-std::abs(scale.x), scale.y);
    }
    else
    {
        mSprite.setOrigin(0.f, 0.f);
        mSprite.setScale(std::abs(scale.x), scale.y);
    }
}

//ALL BULLET/MAGIC FUNCTIONS 
/**
 * @brief Creates default white bullets that spawn a few inches inside of the cannon and move in the direction of the cannon's rotation. 
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
    // mHitbox.setOutlineColor(sf::Color::Blue);
    // mHitbox.setOutlineThickness(2.f);

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
    mBulletSprite.setPosition(mPosition);
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
    if (mUseSprite) {
        target.draw(mBulletSprite, states);
    } else {
        target.draw(mBulletShape, states);
    }
    mHitbox.draw(target, states);
}

void Tower::reset()
{
    mHP = mMaxHP;
    mStability = mMaxStability;
    mDamageMultiplier = 1.0f;
    mBulletDamage = 15.f;
    mBulletSpeed = 700.f;
    mFireRate = 1.5f;
    mBulletColor = sf::Color::White;
    mBulletTexturePtr = nullptr;
    mXPPoints = 0;
    mAttack.clear();
}