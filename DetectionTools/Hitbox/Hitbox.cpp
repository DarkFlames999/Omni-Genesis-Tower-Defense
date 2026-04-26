#include <iostream>
#include <cmath>
#include "Hitbox.h"
#include "../Hurtbox/Hurtbox.h"
#include "../../Entities/Entities.h"

Hitbox::~Hitbox(){}


void Hitbox::detectIntersection(const Hurtbox& HB)
{
    if((mHitbox.getGlobalBounds().intersects(HB.getGlobalBounds())))
    {
        isColliding = true;
        std::cout << "Is Colliding Should be true! : "<< isColliding << std::endl;
    }
    
    else
    {
        isColliding = false;
        std::cout << "Is Colliding Should be false! : "<< isColliding << std::endl;
    }
}