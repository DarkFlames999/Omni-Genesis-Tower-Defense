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
        void setSize(float radius) { mBulletShape.setRadius(radius); mBulletShape.setOrigin(radius, radius); }
        void setSpriteTexture(const sf::Texture* tex);
        float getDamage() const { return mDamage; }
        sf::FloatRect getCircleBounds() const { return mBulletShape.getGlobalBounds(); }

        void setColor(sf::Color color) { mBulletShape.setFillColor(color); }
        void setDamage(float damage) { mDamage = damage; }
        void setBulletSpeed(float speed) { mSpeed = speed; }
        float computeVisualSize() const { return 30.f + (mDamage - 15.f) * 2.f; }

    protected:
        float mDamage = 15.0f;
        float mSpeed  = 700.f;
        bool  mAlive  = true;

    
        sf::Vector2f mDirection;
        sf::CircleShape mBulletShape;
        sf::Sprite mBulletSprite;
        bool mUseSprite = false;

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
        friend class Tower; // Allow Tower to access protected members of Attack
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
        void takeDamage(float amount) { mHP = std::max(0.f, mHP - amount); }
        float getHealth() const { return mHP; }
        bool towerDestroyed(sf::RenderWindow& window);
        std::vector<std::unique_ptr<Attack>>& getAttacks() { return mAttack; }
        int getXPPoints() const { return mXPPoints; }
        bool spendXP(int amount);

        //Cool Stats Getter/Setters for Skill tree bullcrap
        float getDamageMultiplier() const { return mDamageMultiplier; }
        float getFireRate() const { return mFireRate; }
        const sf::Texture& getFireBulletTexture() const { return mFireBulletTexture; }

        void setDamageMultiplier(float multiplier) { mDamageMultiplier = multiplier; }
        void setFireRate(float rate) { mFireRate = rate; }
        void setBulletDamage(float damage);
        void setBulletColor(sf::Color color);
        void setBulletSpeed(float speed);
        void setBulletTexture(const sf::Texture* tex) { mBulletTexturePtr = tex; }
        void setMaxHP(float hp) { mMaxHP = hp; }
        void heal(float amount) { mHP = std::min(mMaxHP, mHP + amount); }

        void reset()
        {
            mAttack.clear();
            mHP = 500.f;
            mMaxHP = 500.f;
            mStability = 100.f;
            mMaxStability = 100.f;
            mDamageMultiplier = 1.0f;
            mBulletColor = sf::Color::White;
            mBulletDamage = 15.f;
            mBulletSpeed = 700.f;
            mFireRate = 1.5f;
            mXPPoints = 0;
            mBulletTexturePtr = nullptr;
        }

        friend class Enemies;

    protected:
        sf::Sprite mCannon;
        sf::Texture mCannonTexture;
        sf::Sprite mHealthBar;
        sf::Texture mHealthTexture;
        sf::RectangleShape mHealthFill;
        sf::RectangleShape mStabilityFill;
        sf::Clock mShootClock;
        const sf::Texture* mBulletTexturePtr = nullptr;
        sf::Texture mFireBulletTexture;

        float mHP = 500.f;
        float mMaxHP = 500.f;
        float mStability = 100.f;
        float mMaxStability = 100.f;
        float mDamageMultiplier = 1.0f;
        sf::Color mBulletColor = sf::Color::White;
        float mBulletDamage = 15.f;
        float mBulletSpeed = 700.f;
        float mFireRate = 1.5f; //How many shots per second the tower can fire
        int mXPPoints = 0; //Experience points for leveling up the tower and unlocking skills in the skill tree
        float mMaxHealthBarWidth = 400.f;
        float mMaxStabilityBarWidth = 360.f;
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
        sf::FloatRect getHurtboxBounds() const { return mHurtbox.getGlobalBounds(); }
        int getDamage() { return mDamage; }
        void takeDamage(int damage) { mHealth -= damage; }
        int getHealth() const { return mHealth; }
        bool isDead() const { return mHealth <= 0; }
        int getXPValue() const { return mXPValue; }
        void giveXP(Tower& tower);
        bool isAttacking() const { return mIsAttacking; }

        friend class Tower;

        void setSpawnSide(bool spawnedLeft);
        void attackTower(Tower& tower, float deltaTime);

    protected:

        sf::Texture mAttackTexture;
        const sf::Texture* mWalkTextureRef = nullptr;

        sf::Texture mJuveniles;
        sf::Texture mMatured;
        sf::Texture mWarden;
        sf::FloatRect mTowerBounds;
        sf::Clock mAnimClock;
        int mCurrentFrame = 0;
        int mFrameCount = 0;
        int mAttackFrameCount = 0;
        float mFrameTime = 0.1f;
        bool mSpawnedLeft = false;
        bool mXPGiven = false;


        bool mIsAttacking = false;
        bool mHasAttackSprite = false;
        sf::Clock mAttackCooldown;
        sf::Clock mAttackAnimTimer;
        static constexpr float Attackcooldown = 2.0f;
        static constexpr float Attackanimationduration = 0.6f;

        //Enemy stats like movement and health
        float mSpeed = 0.f;
        float mHealth = 0.f;
        float mDamage = 0.f;
        float mXPValue = 0.f;

        bool mIsAttacking = false;
        sf::Clock mAttackClock;
        float mAttackCooldown = 1.5f;
    
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