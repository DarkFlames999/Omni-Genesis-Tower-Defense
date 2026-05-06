#ifndef ENTITIES_H
#define ENTITIES_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../DetectionTools/Hitbox.h"
#include "../DetectionTools/Hurtbox.h"

//Entities class, general use, able to be used for polymorphism and inheritance later.

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    Entity() {};
    virtual ~Entity() {};

    virtual bool loadTextureFromFile(const std::string& filename, sf::Texture& texture);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
    virtual void update(sf::RenderWindow& window, float deltaTime) = 0;
    friend class CollisionHandler; // Allow CollisionHandler to access protected members of Entity and its subclasses
    sf::FloatRect getHurtboxBounds() const { return mHurtbox.getGlobalBounds(); }
    sf::FloatRect getHitboxBounds()  const { return mHitbox.getGlobalBounds(); }

protected:
    sf::Sprite mSprite;
    sf::Texture mTexture;
    sf::Vector2f mPosition;
    Hurtbox mHurtbox;
    Hitbox mHitbox;
    sf::Vector2f mSize;
    sf::Vector2f mOrigin;
};

//ATTACK CLASS - SUBCLASSES FOLLOW!
class Attack: public Entity
{
    public:
        Attack() = default;
        ~Attack() = default;
        bool createAttack(sf::Vector2f position, sf::Vector2f direction);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void update(sf::RenderWindow& window, float deltaTime) override;
        bool isAlive() const { return mAlive; }
        void setAlive(bool alive) { mAlive = alive; }
        float getDamage() const { return mDamage; }
        sf::FloatRect getCircleBounds() const { return mBulletShape.getGlobalBounds(); }

    protected:
        float mDamage; //Classic Damage
        float mSpeed  = 700.f;
        bool  mAlive  = true;

        sf::Vector2f mDirection;
        sf::CircleShape mBulletShape;

        // //Stun Tools
        // float mStunFactor;
        // float mStunTime;

        //How fast it moves
        sf::Vector2f mMovementVelocity;
        sf::Vector2f mMovementAcceleration;

        //How long the attack lasts for
        float mDuration;
        sf::Clock mDurationClock;

        //Type of attack (for example, kindness magic, bravery magic, etc.)
        enum class AttackType{ Projectile, AreaOfEffect, Buff, Debuff, Melee } mAttackType;
        enum class MagicType{ Determination, Bravery, Justice, Kindness, Patience, Integrity, Perseverance, 
                              Apathy, Fear, Chaos, Wrath, Nihilistic, Deceit, Irresolution } mMagicType;
        
};

//TOWER CLASS!
class Tower: public Entity
{
    public:
        Tower() = default;
        ~Tower() = default;

        bool createTower(sf::RenderWindow& window, sf::Vector2f size, sf::Vector2f spriteSize);
        void aim(sf::Vector2f target);
        void shoot(sf::RenderWindow& window);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void update(sf::RenderWindow& window, float deltaTime) override;
        void updateAttack(sf::RenderWindow& window, float deltaTime);
        void drawAttack(sf::RenderTarget& target) const;
        void takeDamage(float damage) { mHP -= damage; }
        float getHealth() const { return mHP; }
        bool towerDestroyed(sf::RenderWindow& window);
        std::vector<std::unique_ptr<Attack>>& getAttacks() { return mAttack; }
        int getXPPoints() const { return mXPPoints; }

        friend class Enemies;

    protected:
        sf::Sprite mCannon;
        sf::Texture mCannonTexture;
        sf::Sprite mHealthBar;
        sf::Texture mHealthTexture;
        sf::RectangleShape mHealthFill;
        sf::RectangleShape mStabilityFill;
        sf::Clock mShootClock;

        float mHP = 100.f;
        float mMaxHP = 100.f;
        float mStability = 100.f;
        float mMaxStability = 100.f;
        float mDamageMultiplier = 1.0f;
        float mFireRate = 1.5f; //How many shots per second the tower can fire
        int mXPPoints = 0; //Experience points for leveling up the tower and unlocking skills in the skill tree
        std::vector<std::unique_ptr<Attack>> mAttack;
};

//ENEMY CLASS - SUBCLASSES FOLLOW!

class Enemies: public Entity
{
    public:
        Enemies() = default;
        ~Enemies() = default;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void update(sf::RenderWindow& window, float deltaTime) override;
        // sf::FloatRect getHurtboxBounds() const { return mHurtbox.getGlobalBounds(); }
        int getDamage() { return mDamage; }
        void takeDamage(int damage) { mHealth -= damage; }
        int getHealth() const { return mHealth; }
        bool isDead() const { return mHealth <= 0; }
        int getXPValue() const { return mXPValue; }
        void giveXP(Tower& tower) { tower.mXPPoints += mXPValue; }

        friend class Tower;

        void setSpawnSide(bool spawnedLeft);

    protected:
        sf::Texture mJuveniles;
        sf::Texture mMatured;
        sf::Texture mWarden;
        sf::Clock mAnimClock;
        int mCurrentFrame = 0;
        int mFrameCount = 0;
        float mFrameTime = 0.1f;
        bool mSpawnedLeft = false;

        //Enemy stats like movement and health
        float mSpeed = 0.f;
        float mHealth = 0.f;
        float mDamage = 0.f;
        float mXPValue = 0.f;
    
};

class Juvenile: public Enemies
{
    public:
        Juvenile() = default;
        ~Juvenile() = default;

        bool createJuvenile(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize);
        void juvenileAttack(sf::RenderWindow& window);

    protected:

};

class Matured: public Enemies
{
    public:
        Matured() = default;
        ~Matured() = default;

        bool createMatured(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize);
        void maturedAttack(sf::RenderWindow& window);

    protected:

};

class Warden: public Enemies
{
    public:
        Warden() = default;
        ~Warden() = default;

        bool createWarden(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize);
        void wardenAttack(sf::RenderWindow& window);
        
    protected:

};

#endif