#ifndef ENTITYHANDLER_H
#define ENTITYHANDLER_H

#include "Entities/Entities.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include <memory>

class EntityHandler
{
    public:
        EntityHandler();
        ~EntityHandler();

        void SpawnEntity();
        void DeleteEntity();
        
        void DrawEntities(sf::RenderWindow& window); //For each entity 
        void UpdateEntities(sf::RenderWindow& window);

    private:
        std::vector<std::unique_ptr<Entity>> mEntities;
        
};


#endif