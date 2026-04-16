#ifndef HURTBOX_H
#define HURTBOX_H

#include <SFML/Graphics.hpp>


class Hurtbox : public sf::RectangleShape
{
public:
    Hurtbox() = default;
    explicit Hurtbox(sf::FloatRect bounds);

    bool detectIntersection(const Hitbox& hb) const;

private:
    bool isColliding { false };
    bool isActive { true };
};

#endif
