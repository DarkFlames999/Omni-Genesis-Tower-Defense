#include <iostream>
#include <cmath>
#include "../Hitbox/Hitbox.h"
#include "Hurtbox.h"
#include "../../Entities/Entities.h"

Hurtbox::~Hurtbox(){};

void Hurtbox::detectIntersection(const Hitbox& hb)
{
    if((mHurtbox.getGlobalBounds().intersects(hb.getGlobalBounds())))
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