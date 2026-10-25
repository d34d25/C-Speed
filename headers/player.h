#ifndef PLAYER_H
#define PLAYER_H

#include "gameutils.h"

extern float playerBaseFirerate;

extern float playerBaseSpread;

extern float playerBaseDamage;

extern int playerBasePelletCount;

extern int maxPelletCount;

extern float maxFireRate;

extern float maxSpread;

typedef struct Player
{
    BulletPool bulletPool;

    SpriteData spriteData;

    Rectangle hitbox;

    float size;

    float x;
    float y;

    float speed;

    float damage;

    float powerUpMaxTime;

    float fireRatePowerUpTimer;

    float pelletCountPowerUpTimer;

    float damagePowerUpTimer;

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