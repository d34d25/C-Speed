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

    bool hit;
    bool wasHit;
    
} Player;

void InitPlayer(Player* player);

void UpdatePlayerInput(Player* player);

void UpdatePlayer(Player* player, float dt);

inline void ResetPlayerFlags(Player* player)
{
    player->hit = false;
    player->wasHit = false;
};

inline void UpdatePlayerFlags(Player* player)
{
    player->wasHit = player->hit;
}

#endif