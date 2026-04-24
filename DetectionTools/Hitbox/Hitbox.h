#ifndef HITBOX_H
#define HITBOX_H

#include "SFML/Graphics.hpp"

class Hurtbox;

class Hitbox
{
    public:
        Hitbox() = default;
        explicit Hitbox(sf::FloatRect bounds);
        ~Hitbox();
        //Literally all  of the freaking settings for the freaking hitbox
        void setSize(float x, float y) { mHitbox.setSize({x, y}); }
        void setSize(sf::Vector2f size) { mHitbox.setSize(size); }
        void setOrigin(float x, float y) { mHitbox.setOrigin({x, y}); }
        void setOrigin(sf::Vector2f origin) { mHitbox.setOrigin(origin); }
        void setPosition(float x, float y) { mHitbox.setPosition({x, y}); }
        void setPosition(sf::Vector2f pos) { mHitbox.setPosition(pos); }
        void setFillColor(sf::Color color) { mHitbox.setFillColor(color); }
        void setOutlineColor(sf::Color color) { mHitbox.setOutlineColor(color); }
        void setOutlineThickness(float thickness) { mHitbox.setOutlineThickness(thickness); }
        sf::FloatRect getGlobalBounds() const { return mHitbox.getGlobalBounds(); }
        //Drawing the hitbox for testing purposes, will be removed later
        void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(mHitbox, states); }
        //we are grabbing a referal to the memory of the distinct hurtbox this hitbox is colliding with
        void detectIntersection(const Hurtbox& HB);

    private:
        sf::RectangleShape mHitbox;
        bool isColliding = false;
        bool isActive = true; // We can disable or pause activity if need be for timed moves

    
};

#endif