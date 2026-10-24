#ifndef GAME_UTILS_H

#define GAME_UTILS_H

#include <stddef.h>

#include <stdio.h>

#include "raylib.h"

#include "raymath.h"

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

    int currentFrame;

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

        if(data->currentFrame > data->frameEnd) data->currentFrame = data->frameStart;
    }

    DrawSprite(data->currentFrame, x, y, data->width, data->height, data->rotation, data->flipX, data->flipY);
}

typedef struct Bullet
{
    SpriteData spriteData;

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

#endif