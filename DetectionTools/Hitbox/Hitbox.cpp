#include <iostream>
#include <cmath>
#include "Hitbox.h"
#include "../Hurtbox/Hurtbox.h"

/**
 * @brief Construct a new Hitbox:: Hitbox object
 * 
 * @param bounds 
 */
Hitbox::Hitbox(sf::FloatRect bounds)
{
    setSize({ bounds.width, bounds.height });
    setPosition({ bounds.left, bounds.top });
    setOrigin({ bounds.width / 2.0f, bounds.height / 2.0f });
}

Hitbox::~Hitbox(){}
