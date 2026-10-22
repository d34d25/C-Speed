#ifndef PLAYER_H
#define PLAYER_H

#include "gameutils.h"

typedef struct Player
{
    SpriteData spriteData;

    float x;
    float y;

    float speed;

    bool movingRight;
    bool movingLeft;

    bool movingUp;
    bool movingDown;

    bool shooting;
    
} Player;

void InitPlayer(Player* player);

void UpdatePlayerInput(Player* player);

void UpdatePlayer(Player* player, float dt);

#endif