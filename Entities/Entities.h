#ifndef ENTITIES_H
#define ENTITIES_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../DetectionTools/Hurtbox/Hurtbox.h"

//Entities class, general use, able to be used for polymorphism and inheritance later.

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    Entity() {};
    virtual ~Entity() {};

    virtual bool loadTextureFromFile(const std::string& filename, sf::Texture& texture);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override= 0;
    virtual void update(sf::RenderWindow& window) = 0;

protected:
    sf::Sprite mSprite;
    sf::Texture mTexture;
    sf::Vector2f mPosition;
    Hurtbox mHurtbox{};
    sf::Vector2f mSize;
    sf::Vector2f mOrigin;
};

//TOWER CLASS!

class Tower: public Entity
{
    public:
        Tower() = default;
        ~Tower() = default;

        bool createTower(sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize);
        void aim(sf::Vector2f target);
        void shoot(sf::RenderWindow& window);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void update(sf::RenderWindow& window);

    private:
        sf::Sprite mCannon;
        sf::Texture mCannonTexture;

        float mHP = 100.f;
        float mDamageMultiplier = 1.0f;
        float mFireRate = 1.0f; //How many shots per second the tower can fire
};

//ENEMY CLASS - SUBCLASSES FOLLOW!

class Enemies: public Entity
{
    public:
        Enemies() = default;
        ~Enemies() = default;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void update(sf::RenderWindow& window) override;

    protected:
        sf::Texture mJuveniles;
        sf::Texture mMatured;
        sf::Texture mOvergrown;
        sf::Clock mAnimClock;
        int mCurrentFrame = 0;
        int mFrameCount = 0;
        float mFrameTime = 0.1f;

        //Enemy stats like movement and health
        sf::Clock mMovement;
        int mSpeed;
        int mHealth;
    
};

class Juvenile: public Enemies
{
    public:
        Juvenile() = default;
        ~Juvenile() = default;

        bool createJuvenile(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize);

    protected:
};

class Matured: public Enemies
{
    public:
        Matured() = default;
        ~Matured() = default;

        bool createMatured(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize);

    protected:

};

class Overgrown: public Enemies
{
    public:
        Overgrown() = default;
        ~Overgrown() = default;

        bool createOvergrown(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f size, sf::Vector2f spriteSize);

    protected:
        int mCurrentFrame = 0;
        int mFrameCount = 12;
        float mFrameTime = 0.5f;
};

//ATTACK CLASS - SUBCLASSES FOLLOW!
class Attack: public Entity
{
    public:
        Attack() = default;
        ~Attack() = default;
        bool createAttack(sf::Vector2f position, sf::Vector2f size);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void update(sf::RenderWindow& window);

    protected:
        float mDamage; //Classic Damage

        //Stun Tools
        float mStunFactor;
        float mStunTime;

        //How fast it moves
        sf::Vector2f mMovementVelocity;
        sf::Vector2f mMovementAcceleration;

        //How long the attack lasts for
        float mDuration;
        sf::Clock mDurationClock;

        //Type of attack (for example, kindness magic, bravery magic, etc.)
        enum class AttackType{ Projectile, AreaOfEffect, Buff, Debuff, Melee } mAttackType;
        enum class MagicType{ Determination, Bravery, Justice, Kindness, Patience, Integrity, Perseverance } mMagicType;
        
};

#endif