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

inline void DrawSpriteData(SpriteData* data, int x, int y, float dt)
{
    if(data->animationSpeed > 0.0f)
    {
        data->currentFrame += data->animationSpeed * dt;

        if(data->currentFrame <= data->frameStart) data->currentFrame = data->frameEnd;

        if(data->currentFrame >= data->frameEnd) data->currentFrame = data->frameStart;
    }

    DrawSprite((int)data->currentFrame, x, y, data->width, data->height, data->rotation, data->flipX, data->flipY);
}

inline void DrawSpriteDataCustomFrame(SpriteData* data, int x, int y, int frame)
{
    DrawSprite(frame, x, y, data->width, data->height, data->rotation, data->flipX, data->flipY);
}

inline void UpdateHitbox(Rectangle* rec, float x, float y, float size)
{
    float halfSize = size * 0.5f;

    rec->x = x - halfSize;
    rec->y = y - halfSize;
}

inline bool IsPositionOutOfBounds(float x, float y, float offset)
{
    return y > NATIVE_SIZE + offset || x < -offset || x > NATIVE_SIZE + offset;
}

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

inline void ClearActiveBullets(BulletPool* bulletpool)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        Bullet* b = bulletpool->activeBullets[i];

        if(!b) continue;

        bulletpool->activeBullets[i] = NULL;
        bulletpool->inactiveBullets[i] = b;
    }
}

//physics / collisions

inline void CollideScreenEdges(Rectangle* rec, float* x, float* y)
{
    if(rec->x <= 0.0f) *x = rec->width * 0.5f;

    if(rec->x + rec->width >= NATIVE_SIZE) *x = NATIVE_SIZE - rec->width * 0.5f;

    if(rec->y <= 0.0f) *y = rec->height * 0.5f;

    if(rec->y + rec->height >= NATIVE_SIZE) *y = NATIVE_SIZE - rec->height * 0.5f;
}

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