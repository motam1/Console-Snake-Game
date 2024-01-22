#ifndef FOOD_H
#define FOOD_H

#include "objPos.h"
#include "objPosArrayList.h"
#include "GameMechs.h"

class Food
{
public:
    Food(GameMechs *gameRef);
    ~Food();
    void generateFood(objPosArrayList *blockOff);
    objPosArrayList *getFoodPos();

private:
    objPosArrayList *foodBucket;
    GameMechs *game;
};

#endif