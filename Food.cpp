#include "Food.h"

// Libraries for random object generation
#include <stdlib.h>
#include <time.h>

Food::Food(GameMechs *gameRef)
{
    game = gameRef;
    foodBucket = new objPosArrayList();
    srand(time(NULL)); // For the random object generation
}

Food::~Food()
{
    delete foodBucket; // Delete foodBucket from the heap to prevent memory leaks
}

void Food::generateFood(objPosArrayList *blockOff)
{
    int i = 0, randomX, randomY, conflict;

    // Clear the food bucket
    for (int x = 0; x < 5; x++)
    {
        foodBucket->removeTail();
    }

    while (i < 5) // Generate 5 new objects each time the function is called
    {
        conflict = 1;

        while (conflict)
        {
            // Generate a new random set of coordinates
            objPos tempBlockCheck; // Holds the position returned by the get element function (passed by reference)
            conflict = 0;
            randomX = (rand() % (game->getBoardSizeX() - 2)) + 1;
            randomY = (rand() % (game->getBoardSizeY() - 2)) + 1;

            // Ensure the coordinate does not overlap with the player position
            for (int k = 0; k < blockOff->getSize(); k++)
            {
                blockOff->getElement(tempBlockCheck, k);
                if (tempBlockCheck.x == randomX && tempBlockCheck.y == randomY)
                {
                    conflict = 1;
                }
            }
            // Ensure the coordinate does not overlap with a previously generated food element
            for (int j = 0; j < foodBucket->getSize(); j++)
            {
                foodBucket->getElement(tempBlockCheck, j);
                if (tempBlockCheck.x == randomX && tempBlockCheck.y == randomY)
                {
                    conflict = 1;
                }
            }
            if (conflict == 0) // If no overlap was detected, add the coordinates to the arraylist
            {
                objPos tempFood;
                if (i < 3) // Generate 3 regular, and 2 special food objects.
                {
                    tempFood.setObjPos(randomX, randomY, 'O'); // Regular food object
                }
                else if (i == 4)
                {
                    tempFood.setObjPos(randomX, randomY, '-'); // Shrinks the player by 5 (Player must be bigger than 5 units)
                }
                else
                {
                    tempFood.setObjPos(randomX, randomY, '+'); // Gain an extra 5 points
                }

                foodBucket->insertTail(tempFood);
            }
        }
        i++;
    }
}

objPosArrayList *Food::getFoodPos()
{
    return foodBucket;
}