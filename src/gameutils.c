#include "../headers/gameutils.h"

const int NATIVE_SIZE = 128;

const int SCALE = 7;

const int SCREEN_SIZE = NATIVE_SIZE * SCALE;

int halfScreen = SCREEN_SIZE * 0.5f;

Texture2D atlas;

void InitAssets()
{
    Image atlasimg = LoadImage("assets/testspriteatlas.png");

    atlas = LoadTextureFromImage(atlasimg);

    UnloadImage(atlasimg);
}

void UnloadAssets()
{
    UnloadTexture(atlas);
}

void DrawSprite(int frame, int posX, int posY, int frameWidth, int frameHeight, float rotation, bool flipX, bool flipY)
{
    int col = 0;
    int row = 0;

    if(frame > 0)
    {
        row = (frame / (atlas.width / frameWidth)) * frameHeight;

        col = (frame % (atlas.width / frameWidth)) * frameWidth;
    }

    float sourceRecWidth = (float)frameWidth;
    float sourceRecHeight = (float)frameHeight;

    Rectangle sourceRec = {
        col, row,
        (float)frameWidth, (float)frameHeight
    };

    if(flipX) sourceRecWidth = (float)-frameWidth;

    if(flipY) sourceRecHeight = (float)-frameHeight;

    DrawTexturePro(
        atlas,
        (Rectangle){col, row, sourceRecWidth, sourceRecHeight},
        (Rectangle){posX, posY, (float)frameWidth, (float)frameHeight},
        (Vector2){frameWidth * 0.5f, frameHeight * 0.5f},
        rotation,
        WHITE
    );
}

//pools

const int BULLET_POOL_SIZE = 128;

void InitBullet(Bullet *bullet)
{
    bullet->speed = 0;

    bullet->x = 0.0f;
    bullet->y = 0.0f;

    bullet->xDir = 0;
    bullet->yDir = -1;

    bullet->currentTime = 0.0f;
    bullet->maxTime = 1.5f;

    bullet->alive = false;

    SpriteData* sprite = &bullet->spriteData;

    sprite->animationSpeed = 0.0f;
    sprite->currentFrame = 6;
    sprite->flipX = false;
    sprite->flipY = true;

    sprite->frameStart = 6;
    sprite->frameEnd = 6;

    sprite->width = 8;
    sprite->height = 8;

    sprite->rotation = 0.0f;
}

void InitBulletPool(BulletPool *bulletpool, float firerate, int pelletCount, float spread)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        InitBullet(&bulletpool->bullets[i]);

        bulletpool->inactiveBullets[i] = &bulletpool->bullets[i];

        bulletpool->activeBullets[i] = NULL;
    }
    
    bulletpool->fireTimer = 0.0f;

    bulletpool->firerate = firerate;

    bulletpool->pelletCount = pelletCount;

    bulletpool->spread = spread;
}

void UpdateBulletPool(BulletPool* bulletpool, float dt)
{
    //active bullets
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        Bullet* b = bulletpool->activeBullets[i];

        if(b == NULL) continue;

        //update bullet

        b->currentTime += dt;

        b->x += b->speed * b->xDir * dt;
        b->y += b->speed * b->yDir * dt;

        if(b->currentTime >= b->maxTime) b->alive = false;

        //check if dead and swap if so
        if(!b->alive)
        {
            bulletpool->inactiveBullets[i] = b;

            bulletpool->activeBullets[i] = NULL;
        }
    }
}

void SpawnBullet(BulletPool* bulletpool, float x, float y, float speed, float xDir, float yDir)
{
    Bullet* b;

    int bulletIndex = 0; 

    //inactive bullets
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        Bullet* currentBullet = bulletpool->inactiveBullets[i];

        if(currentBullet == NULL) continue;
        else
        {
            b = currentBullet;
            bulletIndex = i;
        }
    } 
    
    if(b != NULL && bulletIndex > 0 && bulletIndex < BULLET_POOL_SIZE)
    {
        b->x = x;
        b->y = y;

        b->speed = speed;

        b->xDir = xDir;
        b->yDir = yDir;

        b->currentTime = 0.0f;

        b->alive = true;

        b->spriteData.rotation = (atan2f(b->yDir, b->xDir) * 180.0f / PI) - 90.0f;

        bulletpool->activeBullets[bulletIndex] = b;
        
        bulletpool->inactiveBullets[bulletIndex] = NULL;
    }
}

void FireBullet(
    BulletPool *bulletpool,
    float x,
    float y,
    float speed,
    float xDir,
    float yDir,
    bool keyPress,
    bool keyHold,
    float dt
)
{
    if(!keyHold) return;

    bulletpool->fireTimer += dt;

    bool canShoot = false;

    if(bulletpool->fireTimer > bulletpool->firerate || keyPress)
    {
        bulletpool->fireTimer = 0.0f;
    
        canShoot = true;
    }
    else
    {
        canShoot = false;
    }

    if(canShoot)
    {
        float halfPelletCount = bulletpool->pelletCount * 0.5f;

        for(float i = -halfPelletCount; i < halfPelletCount; i++)
        {
            float angleDelta = PI * 2.0f * (i + 0.5f) * bulletpool->spread / bulletpool->pelletCount;

            float angleSin = sinf(angleDelta);

            float angleCos = cosf(angleDelta);

            float newX = xDir * angleCos - yDir * angleSin;
            float newY = xDir * angleSin + yDir * angleCos;
            
            SpawnBullet(bulletpool, x, y, speed, newX, newY);
        }
    }
}