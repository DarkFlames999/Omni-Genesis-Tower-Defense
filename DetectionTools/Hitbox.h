#ifndef HITBOX_H
#define HITBOX_H

#include <SFML/Graphics.hpp>
#include "../Entities/Entities.h"

class Hitbox
{
    public:
        Hitbox() = default;
        ~Hitbox() = default;

        void setSize(sf::Vector2f size)            { mHitbox.setSize(size); }
        void setSize(float x, float y)             { mHitbox.setSize({x, y}); }
        void setPosition(sf::Vector2f pos)         { mHitbox.setPosition(pos); }
        void setPosition(float x, float y)         { mHitbox.setPosition({x, y}); }
        void setOrigin(sf::Vector2f origin)        { mHitbox.setOrigin(origin); }
        void setOrigin(float x, float y)           { mHitbox.setOrigin({x, y}); }
        void setFillColor(sf::Color color)         { mHitbox.setFillColor(color); }
        void setOutlineColor(sf::Color color)      { mHitbox.setOutlineColor(color); }
        void setOutlineThickness(float thickness)  { mHitbox.setOutlineThickness(thickness); }
        sf::FloatRect getGlobalBounds() const      { return mHitbox.getGlobalBounds(); }
        void draw(sf::RenderTarget& target, sf::RenderStates states) const
        { target.draw(mHitbox, states); }
    private:
        sf::RectangleShape mHitbox;
        bool isColliding = false;
        bool isActive    = true;
};

#endif