#ifndef SCENE_H
#define SCENE_H

#include "player.h"

#include "enemy.h"

typedef struct Scene
{
    Player player;

    Texture2D titleTexture;

    EnemyPool enemyPool;

    BulletPool backgroundStars;

    PowerUpPool powerUps;

    float timer;

    int frameCounter;

    bool gameover;

    bool inTitle;

} Scene;

void InitScene(Scene* scene);

void UpdateScene(Scene* scene, float dt);

void DrawScene(Scene* scene, float dt);

void TickSceneCounter(Scene* scene);

void UnloadScene(Scene* scene);

#endif