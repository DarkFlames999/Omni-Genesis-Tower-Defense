#ifndef HURTBOX_H
#define HURTBOX_H

#include <SFML/Graphics.hpp>

class Hitbox;

class Hurtbox 
{
public:
    Hurtbox() = default;
    explicit Hurtbox(sf::FloatRect bounds);
    ~Hurtbox();
    //Literally all  of the freaking settings for the freaking hitbox
    void setSize(float x, float y)  { mHurtbox.setSize({x, y}); }
    void setPosition(float x, float y) { mHurtbox.setPosition({x, y}); }
    void setOrigin(float x, float y) { mHurtbox.setOrigin({x, y}); }
    void setFillColor(sf::Color color) { mHurtbox.setFillColor(color); }
    void setOutlineColor(sf::Color color) { mHurtbox.setOutlineColor(color); }
    void setOutlineThickness(float thickness) { mHurtbox.setOutlineThickness(thickness); }
    sf::FloatRect getGlobalBounds() const { return mHurtbox.getGlobalBounds(); }

    //Drawing the hurtbox for testing purposes, will be removed later
    void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(mHurtbox, states); }
    //we are grabbing a referal to the memory of the distinct hurtbox this hitbox is colliding with
    void detectIntersection(const Hitbox& hb);

protected:
    sf::RectangleShape mHurtbox;
    bool isColliding;
    bool isActive;
};

#endif
