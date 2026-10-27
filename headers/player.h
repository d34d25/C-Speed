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

    float maxInvulTimer;

    float invulTimer;

    int lives;

    bool movingRight;
    bool movingLeft;

    bool movingUp;
    bool movingDown;

    bool shooting;
    bool shootingPress;

    bool hit;
    bool wasHit;

    bool dead;
    
} Player;

void InitPlayer(Player* player);

void UpdatePlayerInput(Player* player);

void UpdatePlayer(Player* player, float dt);

void UpdatePlayerFlags(Player* player);

void ResetPlayerInput(Player* player);

void ResetPlayerFlags(Player *player);

bool IsPlayerAlive(Player* player);

#endif