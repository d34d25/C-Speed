#ifndef SCENE_H
#define SCENE_H

#include "player.h"

#include "enemy.h"

typedef struct Scene
{
    Player player;

    EnemyPool enemyPool;

    BulletPool backgroundStars;

    PowerUpPool powerUps;

    float timer;

    int frameCounter;

    bool gameover;

} Scene;

void InitScene(Scene* scene);

void UpdateScene(Scene* scene, float dt);

void DrawScene(Scene* scene, float dt);

void TickSceneCounter(Scene* scene);

#endif