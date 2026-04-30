#ifndef HURTBOX_H
#define HURTBOX_H

#include <SFML/Graphics.hpp>
#include "../Entities/Entities.h"

class Hurtbox
{
public:
    Hurtbox() = default;
    ~Hurtbox() = default;

    void setSize(sf::Vector2f size)            { mHurtbox.setSize(size); }
    void setSize(float x, float y)             { mHurtbox.setSize({x, y}); }
    void setPosition(sf::Vector2f pos)         { mHurtbox.setPosition(pos); }
    void setPosition(float x, float y)         { mHurtbox.setPosition({x, y}); }
    void setOrigin(sf::Vector2f origin)        { mHurtbox.setOrigin(origin); }
    void setOrigin(float x, float y)           { mHurtbox.setOrigin({x, y}); }
    void setFillColor(sf::Color color)         { mHurtbox.setFillColor(color); }
    void setOutlineColor(sf::Color color)      { mHurtbox.setOutlineColor(color); }
    void setOutlineThickness(float thickness)  { mHurtbox.setOutlineThickness(thickness); }
    sf::FloatRect getGlobalBounds() const      { return mHurtbox.getGlobalBounds(); }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    { target.draw(mHurtbox, states); }

protected:
    sf::RectangleShape mHurtbox;
    bool isColliding = false;
    bool isActive    = true;
};

#endif