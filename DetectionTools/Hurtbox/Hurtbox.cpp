#include <iostream>
#include <cmath>
#include "../Hitbox/Hitbox.h"
#include "Hurtbox.h"

/**
 * @brief Construct a new Hurtbox:: Hurtbox object
 * 
 * @param bounds 
 */
Hurtbox::Hurtbox(sf::FloatRect bounds)
{
    setSize({ bounds.width, bounds.height });
    setPosition({ bounds.left, bounds.top });
    setOrigin({ bounds.width / 2.0f, bounds.height / 2.0f });
}