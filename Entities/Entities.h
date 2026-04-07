#ifndef ENTITIES_H
#define ENTITIES_H

#include <SFML/Graphics.hpp>
#include <vector>

//Entities class, general use, able to be used for polymorphism and inheritance later.



class Entity : public sf::Drawable, public sf::Transformable
{
public :
    Entity(){};
    ~Entity(){};

    void draw(sf::RenderWindow& window);
    void update(sf::RenderWindow& window);

protected:
    sf::Sprite mSprite;
    sf::Texture mTexture;
    sf::Vector2f mPosition;
        //This struct will hold all the data for an entity, such as health, speed, damage, etc.
    //This will be used to easily create new entities by just filling out this struct and passing it to the Entity constructor.
    sf::RectangleShape mHitbox; //Shares the same position as the entity, but can be used for collision detection and other things.
    sf::Vector2f mSize;
    sf::Vector2f mOrigin;
};

#endif