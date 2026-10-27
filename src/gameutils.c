#include "gameutils.h"

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

void DrawSpriteData(SpriteData *data, int x, int y, float dt)
{
    if(data->animationSpeed > 0.0f)
    {
        data->currentFrame += data->animationSpeed * dt;

        if(data->currentFrame <= data->frameStart) data->currentFrame = data->frameEnd;

        if(data->currentFrame >= data->frameEnd) data->currentFrame = data->frameStart;
    }

    DrawSprite((int)data->currentFrame, x, y, data->width, data->height, data->rotation, data->flipX, data->flipY);
}

void DrawSpriteDataCustomFrame(SpriteData* data, int x, int y, int frame)
{
    DrawSprite(frame, x, y, data->width, data->height, data->rotation, data->flipX, data->flipY);
}

void UpdateHitbox(Rectangle *rec, float x, float y, float size)
{
    float halfSize = size * 0.5f;

    rec->x = x - halfSize;
    rec->y = y - halfSize;
}

bool IsPositionOutOfBounds(float x, float y, float offset)
{
    return y > NATIVE_SIZE + offset || x < -offset || x > NATIVE_SIZE + offset;
}

//pools

const int BULLET_POOL_SIZE = 128;

void InitBullet(Bullet *bullet, float maxTime, int bulletSprite)
{
    bullet->speed = 0;

    bullet->x = 0.0f;
    bullet->y = 0.0f;

    bullet->xDir = 0;
    bullet->yDir = -1;

    bullet->currentTime = 0.0f;
    bullet->maxTime = maxTime;//1.5f;

    bullet->alive = false;

    bullet->radius = 1.5f;

    SpriteData* sprite = &bullet->spriteData;

    sprite->animationSpeed = 0.0f;
    
    sprite->flipX = false;
    sprite->flipY = true;

    sprite->currentFrame = bulletSprite;

    sprite->frameStart = bulletSprite;
    sprite->frameEnd = bulletSprite;

    sprite->width = 8;
    sprite->height = 8;

    sprite->rotation = 0.0f;
}

void InitBulletPool(
    BulletPool *bulletpool,
    float firerate,
    int pelletCount,
    float spread,
    float bulletMaxTime,
    int bulletSprite
)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        InitBullet(&bulletpool->bullets[i], bulletMaxTime, bulletSprite);

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

        if(b->currentTime >= b->maxTime || IsPositionOutOfBounds(b->x, b->y, -b->radius)) b->alive = false;

        //check if dead and swap if so
        if(!b->alive)
        {
            bulletpool->inactiveBullets[i] = b;

            bulletpool->activeBullets[i] = NULL;
        }
    }
}

void SpawnBullet(
    BulletPool* bulletpool,
    float x,
    float y,
    float speed,
    float xDir,
    float yDir,
    bool rotateSprite
)
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

        if(rotateSprite) b->spriteData.rotation = (atan2f(b->yDir, b->xDir) * 180.0f / PI) - 90.0f;

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
    bool rotateSprite,
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
            
            SpawnBullet(bulletpool, x, y, speed, newX, newY, rotateSprite);
        }
    }
}

void ClearActiveBullets(BulletPool *bulletpool)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        Bullet* b = bulletpool->activeBullets[i];

        if(!b) continue;

        bulletpool->activeBullets[i] = NULL;
        bulletpool->inactiveBullets[i] = b;
    }
}

void CollideScreenEdges(Rectangle *rec, float *x, float *y)
{
    if(rec->x <= 0.0f) *x = rec->width * 0.5f;

    if(rec->x + rec->width >= NATIVE_SIZE) *x = NATIVE_SIZE - rec->width * 0.5f;

    if(rec->y <= 0.0f) *y = rec->height * 0.5f;

    if(rec->y + rec->height >= NATIVE_SIZE) *y = NATIVE_SIZE - rec->height * 0.5f;
}

void InitPowerUp(PowerUP * powerUP)
{
    powerUP->alive = false;

    powerUP->speed = 10.0f;

    powerUP->x = 0.0f;
    powerUP->y = 0.0f;

    powerUP->radius = 1.5f;

    powerUP->type = FIRERATE_UP;

    SpriteData* sprite = &powerUP->spriteData;

    sprite->animationSpeed = 0.0f;
    
    sprite->flipX = false;
    sprite->flipY = false;

    sprite->currentFrame = 5;

    sprite->frameStart = 5;
    sprite->frameEnd = 5;

    sprite->width = 8;
    sprite->height = 8;

    sprite->rotation = 0.0f;
}

void InitPowerUpPool(PowerUpPool *powerups)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        InitPowerUp(&powerups->powerUpPool[i]);

        powerups->inactivePowerUps[i] = &powerups->powerUpPool[i];

        powerups->activePowerUps[i] = NULL;
    }
}

void UpdatePowerUPPool(PowerUpPool *powerups, float dt)
{
    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        PowerUP* p = powerups->activePowerUps[i];

        if(!p) continue;

        p->y += p->speed * dt;

        if(IsPositionOutOfBounds(p->x, p->y, -p->radius))
        {
            p->alive = false;
        }

        if(!p->alive)
        {
            powerups->inactivePowerUps[i] = p;

            powerups->activePowerUps[i] = NULL;
        }
    }
}

void SpawnPowerUp(PowerUpPool* powerups, float x, float y, PowerUpType type)
{
    PowerUP* p;

    int powerUpIndex = 0;

    for(int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        PowerUP* currentPowerUp = powerups->inactivePowerUps[i];

        if(currentPowerUp == NULL) continue;
        else
        {
            p = currentPowerUp;
            powerUpIndex = i;
        }
    }

    if(p != NULL && powerUpIndex > 0 && powerUpIndex < BULLET_POOL_SIZE)
    {
        p->x = x;
        p->y = y;

        p->type = type;

        p->alive = true;

        SpriteData* sprite = &p->spriteData;

        int frame;

        switch (p->type)
        {
        case FIRERATE_UP: frame = 8; break;

        case PELLET_COUNT_UP: frame = 9; break;

        case DAMAGE_UP: frame = 10; break;
        
        default: frame = 5; break;
        }

        sprite->currentFrame = frame;

        powerups->activePowerUps[powerUpIndex] = p;

        powerups->inactivePowerUps[powerUpIndex] = NULL;
    }
}
