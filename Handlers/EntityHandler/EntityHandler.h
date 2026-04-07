#ifndef ENTITYHANDLER_H
#define ENTITYHANDLER_H

#include "Entities/Entities.h"
#include "SFML/Graphics.hpp"
#include <vector>

class EntityHandler
{
    public:
        EntityHandler();
        ~EntityHandler();

        void SpawnEntity();
        void DeleteEntity();
        void DrawEntities(); //For each entity 
        void UpdateEntities();

    private:
        std::vector<Entity> mEntities;
        

        

};


#endif