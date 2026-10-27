#ifndef GAME_UTILS_H

#define GAME_UTILS_H

#include <stddef.h>

#include <stdio.h>

#include "raylib.h"

#include "raymath.h"

#define PICO_WHITE (Color){255,241,232,255}

extern const int NATIVE_SIZE;

extern const int SCALE;

extern const int SCREEN_SIZE;

extern int halfScreen;

//the whole game will use a shared spritesheet
extern Texture2D atlas;

//only holds visual data for the sprite
typedef struct SpriteData
{
    float animationSpeed;

    float rotation;

    int width;
    int height;

    int frameStart;
    int frameEnd;

    float currentFrame;

    bool flipX;
    bool flipY;

} SpriteData;

void InitAssets();

void UnloadAssets();

void DrawSprite(int frame, int posX, int posY, int frameWidth, int frameHeight, float rotation, bool flipX, bool flipY);

void DrawSpriteData(SpriteData* data, int x, int y, float dt);

void DrawSpriteDataCustomFrame(SpriteData* data, int x, int y, int frame);

void UpdateHitbox(Rectangle* rec, float x, float y, float size);

bool IsPositionOutOfBounds(float x, float y, float offset);

typedef struct Bullet
{
    SpriteData spriteData;

    float radius;

    float currentTime;
    float maxTime;

    float speed;

    float xDir;
    float yDir;

    float x;
    float y;

    bool alive;

} Bullet;

void InitBullet(Bullet* bullet, float maxTime, int bulletSprite);

typedef struct BulletPool
{
    Bullet bullets[128];

    Bullet* inactiveBullets[128];

    Bullet* activeBullets[128];

    float firerate;
    float fireTimer;

    int pelletCount;
    float spread;

} BulletPool;

extern const int BULLET_POOL_SIZE;

void InitBulletPool(
    BulletPool* bulletpool,
    float firerate,
    int pelletCount,
    float spread,
    float bulletMaxTime,
    int bulletSprite
);

void UpdateBulletPool(BulletPool* bulletpool, float dt);

void SpawnBullet(
    BulletPool* bulletpool,
    float x,
    float y,
    float speed,
    float xDir,
    float yDir,
    bool rotateSprite
);

void FireBullet(
    BulletPool* bulletpool,
    float x,
    float y,
    float speed,
    float xDir,
    float yDir,
    bool keyPress,
    bool keyHold,
    bool rotateSprite,
    float dt
);

void ClearActiveBullets(BulletPool* bulletpool);

//physics / collisions

void CollideScreenEdges(Rectangle* rec, float* x, float* y);

//power ups

typedef enum PowerUpType
{
    FIRERATE_UP,
    PELLET_COUNT_UP,
    DAMAGE_UP,
    POWER_UP_TYPE_COUNT

} PowerUpType;

typedef struct PowerUP
{
    SpriteData spriteData;

    PowerUpType type;

    float radius;

    float speed;

    float x;
    float y;

    bool alive;

} PowerUP;

void InitPowerUp(PowerUP* powerUP);

typedef struct PowerUpPool
{
    //same size as bullets
    PowerUP powerUpPool[128];

    PowerUP* activePowerUps[128];

    PowerUP* inactivePowerUps[128];

} PowerUpPool;

void InitPowerUpPool(PowerUpPool* powerups);

void UpdatePowerUPPool(PowerUpPool* powerups, float dt);

void SpawnPowerUp(PowerUpPool* powerups, float x, float y, PowerUpType type);

#endif