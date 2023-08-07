#include <stdlib.h>
#include <stdio.h>
#include "game.h"

static PlaydateAPI* pd = NULL;

//game state
int score = 0;

LCDSprite *player = NULL;

// background
LCDSprite *bgSprite = NULL;
LCDBitmap *bgImage = NULL;
int bgY = 0;
int bgH = 0;

// cached images
//add some images here



typedef enum {
    kPlayer = 0,
    kPlayerClaw = 1,
    kEnemy = 2,
} SpriteType;



void setPDPtr(PlaydateAPI* p) {
    pd = p;
}

LCDBitmap *loadImageAtPath(const char *path)
{
    const char *outErr = NULL;
    LCDBitmap *img = pd->graphics->loadBitmap(path, &outErr);
    if ( outErr != NULL ) {
        pd->system->logToConsole("Error loading image at path '%s': %s", path, outErr);
    }
    return img;
}

//Backgrounds
static void drawBackgroundSprite(LCDSprite* sprite, PDRect bounds, PDRect drawrect)
{
    pd->graphics->drawBitmap(bgImage, 0, bgY, kBitmapUnflipped);
    pd->graphics->drawBitmap(bgImage, 0, bgY - bgH, kBitmapUnflipped);
}

static void updateBackgroundSprite(LCDSprite* s)
{
    bgY += 1;
    if (bgY > bgH) {
        bgY = 0;
    }

    pd->sprite->markDirty(bgSprite);
}

static void setupBackground(void)
{
    bgSprite = pd->sprite->newSprite();

    bgImage = loadImageAtPath("images/background_400x240px");
    pd->graphics->getBitmapData(bgImage, NULL, &bgH, NULL, NULL, NULL);

    pd->sprite->setUpdateFunction(bgSprite, updateBackgroundSprite);
    pd->sprite->setDrawFunction(bgSprite, drawBackgroundSprite);

    PDRect bgBounds = PDRectMake(0, 0, 400, 240);
    pd->sprite->setBounds(bgSprite, bgBounds);

    pd->sprite->setZIndex(bgSprite, 0);

    pd->sprite->addSprite(bgSprite);
}


static SpriteCollisionResponseType playerCollisionResponse(LCDSprite* sprite, LCDSprite* other)
{
    return kCollisionTypeOverlap;
}

static void updatePlayer(LCDSprite* s)
{
    PDButtons current;
    pd->system->getButtonState(&current, NULL, NULL);

    int dx = 0;
    int dy = 0;

    if (current & kButtonUp) {
        dy = -4;
    }
    else if (current & kButtonDown) {
        dy = 4;
    }
    if (current & kButtonLeft) {
        dx = -4;
    }
    else if (current & kButtonRight) {
        dx = 4;
    }

    float x, y;
    pd->sprite->getPosition(s, &x, &y);

    int len;
    SpriteCollisionInfo* cInfo = pd->sprite->moveWithCollisions(s, x + dx, y + dy, NULL, NULL, &len);

    int i;
    for (i = 0; i < len; i++)
    {
        SpriteCollisionInfo info = cInfo[i];

        if (pd->sprite->getTag(info.other) == kEnemy) {
            //destroyEnemyPlane(info.other);
            //score -= 1;
            //pd->system->logToConsole("Score: %d", score);
        }
    }

    pd->system->realloc(cInfo, 0); // caller is responsible for freeing memory of array returned by moveWithCollisions()
}

static LCDSprite* createPlayer(int centerX, int centerY)
{
    LCDSprite* plane = pd->sprite->newSprite();

    pd->sprite->setUpdateFunction(plane, updatePlayer);

    LCDBitmap* planeImage = loadImageAtPath("images/scuba_sam_idle");
    int w, h;
    pd->graphics->getBitmapData(planeImage, &w, &h, NULL, NULL, NULL);

    pd->sprite->setImage(plane, planeImage, kBitmapUnflipped);

    PDRect cr = PDRectMake(5, 5, w - 10, h - 10);
    pd->sprite->setCollideRect(plane, cr);
    pd->sprite->setCollisionResponseFunction(plane, playerCollisionResponse);

    pd->sprite->moveTo(plane, centerX, centerY);

    pd->sprite->setZIndex(plane, 1000);
    pd->sprite->addSprite(plane);

    pd->sprite->setTag(plane, kPlayer);

    return plane;
}


// game initialization
void setupGame(void)
{
    srand(pd->system->getSecondsSinceEpoch(NULL));

    setupBackground();
    player = createPlayer(200, 180);
    //preloadImages();
}

// main update function
int update(void* ud)
{

    pd->sprite->updateAndDrawSprites();

    return 1;
}