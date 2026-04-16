#ifndef HITBOX_H
#define HITBOX_H

#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>

class Hurtbox;

class Hitbox : public sf::RectangleShape
{
    public:
            Hitbox() = default;
            explicit Hitbox(sf::FloatRect bounds);
            ~Hitbox();
        //we are grabbing a referal to the memory of the distinct hurtbox this hitbox is colliding with
        bool detectIntersection(Hurtbox& HB);

    private:
        bool isColliding {false};
        bool isActive { true }; // We can disable or pause activity if need be for timed moves

    
};

#endif