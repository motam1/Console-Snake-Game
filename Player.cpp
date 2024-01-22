#include "Player.h"

Player::Player(GameMechs *thisGMRef, Food *thisFoodRef)
{
    mainGameMechsRef = thisGMRef;
    myDir = STOP;
    // more actions to be included

    playerPosList = new objPosArrayList();

    // Sets the initial player position to the center of the board.
    objPos tempPos(mainGameMechsRef->getBoardSizeX() / 2, (mainGameMechsRef->getBoardSizeY()) / 2, '*');
    playerPosList->insertHead(tempPos);

    foodList = thisFoodRef; // Added a reference to the food class for the bonus iteration
}

Player::~Player()
{
    // delete any heap members here
    delete playerPosList;
}

objPosArrayList *Player::getPlayerPos()
{
    // return the reference to the playerPos arrray list
    return playerPosList;
}

void Player::updatePlayerDir()
{

    // PPA3 input processing logic
    switch (mainGameMechsRef->getInput())
    {
    case 'w':
        if (myDir != UP && myDir != DOWN)
        {
            myDir = UP;
        }
        break;
    case 'a':
        if (myDir != LEFT && myDir != RIGHT)
        {
            myDir = LEFT;
        }
        break;
    case 's':
        if (myDir != UP && myDir != DOWN)
        {
            myDir = DOWN;
        }
        break;
    case 'd':
        if (myDir != LEFT && myDir != RIGHT)
        {
            myDir = RIGHT;
        }
        break;
    case 27: // Escape key to force exit
        mainGameMechsRef->setExitTrue();
        break;
    default:
        break;
    }
    mainGameMechsRef->clearInput();
}

void Player::movePlayer()
{
    objPos currHead;
    playerPosList->getHeadElement(currHead);
    // PPA3 Finite State Machine logic
    switch (myDir)
    {
    case LEFT:
        currHead.x--;
        break;
    case RIGHT:
        currHead.x++;
        break;
    case UP:
        currHead.y--;
        break;
    case DOWN:
        currHead.y++;
        break;
    case STOP:
    default:
        break;
    }

    // Border Wraparound
    if (currHead.x <= 0)
    {
        currHead.x = (mainGameMechsRef->getBoardSizeX() - 2);
    }
    else if (currHead.x >= (mainGameMechsRef->getBoardSizeX() - 1))
    {
        currHead.x = currHead.x % (mainGameMechsRef->getBoardSizeX() - 2);
    }
    else if (currHead.y <= 0)
    {
        currHead.y = (mainGameMechsRef->getBoardSizeY() - 2);
    }
    else if (currHead.y >= (mainGameMechsRef->getBoardSizeY() - 1))
    {
        currHead.y = currHead.y % (mainGameMechsRef->getBoardSizeY() - 2);
    }

    // Check if collided with a food object
    if (checkFoodConsumption(currHead))
    {
    }
    else
    {
        playerPosList->insertHead(currHead);
        playerPosList->removeTail(); // remove tail if no collision is detected
    }

    // Check for self collisions
    if (checkSelfCollision(currHead))
    {
        // If a collsion is detected, set the exit and lose flags to true and exit.
        mainGameMechsRef->setExitTrue();
        mainGameMechsRef->setLoseFlag();
    }
}

bool Player::checkFoodConsumption(objPos &curHeadPos)
{
    // Detects collision with food object
    objPosArrayList *foodPosList;
    foodPosList = foodList->getFoodPos();

    objPos temp; // Holds the passed by reference elements

    for (int i = 0; i < foodPosList->getSize(); i++) // Loop through all the food objects on the board
    {
        foodPosList->getElement(temp, i);
        if (curHeadPos.isPosEqual(&temp)) // If the coordinates match,
        {
            if (temp.getSymbol() == '-') // Shrinks the player by 5 (Player must be bigger than 5 units)
            {
                if (playerPosList->getSize() > 5)
                {
                    for (int j = 0; j < 5; j++)
                    {
                        playerPosList->removeTail();
                    }
                }
                foodList->generateFood(playerPosList);
            }
            else if (temp.getSymbol() == '+') // Gain an extra 5 points
            {
                increasePlayerSize(curHeadPos);
                for (int k = 0; k < 5; k++)
                {
                    mainGameMechsRef->incrementScore();
                }
            }
            else // 'O' Regular food object
            {
                increasePlayerSize(curHeadPos);
            }

            return true;
        }
    }
    return false;
}

void Player::increasePlayerSize(objPos &curHeadPos)
{
    playerPosList->insertHead(curHeadPos);
    foodList->generateFood(playerPosList);
    mainGameMechsRef->incrementScore();
}

bool Player::checkSelfCollision(objPos &curHeadPos)
{
    objPos tempBodyPos;
    for (int i = 1; i < playerPosList->getSize(); i++) // Start the loop after the head position
    {
        playerPosList->getElement(tempBodyPos, i); // Get each body position
        if (curHeadPos.isPosEqual(&tempBodyPos))   // If a collision is detected, return true
        {
            return true;
        }
    }
    return false;
}
