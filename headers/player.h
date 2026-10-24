#ifndef PLAYER_H
#define PLAYER_H

#include "gameutils.h"

typedef struct Player
{
    BulletPool bulletPool;

    SpriteData spriteData;

    Rectangle hitbox;

    float size;

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