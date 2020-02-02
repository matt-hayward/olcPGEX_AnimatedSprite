# olcPGEX_AnimatedSprite

This extension is designed for use with the [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine) by [Javidx9](https://github.com/OneLoneCoder). It allows you to easily use animated sprites that utilise either single-file spritesheets or multiple image files for each frame.

---

## Requirements

The extension requires the [olcPGEX_Graphics2D](https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Extensions/olcPGEX_Graphics2D.h) extension to work, but should require no additional libraries beyond that and the olcPixelGameEngine itself.

---

## Usage

To use the olcPGEX_AnimatedSprite extension, it needs to be included in your application. This is done like so:

```cpp
#define OLC_PGEX_ANIMSPR
#include "olcPGEX_AnimatedSprite.h"
```

### Single Spritesheet

```cpp
// define sprite in your PGE program
olc::AnimatedSprite sprite;

bool OnUserCreate()
{
    // configure the sprite:
    sprite.mode = olc::AnimatedSprite::SPRITE_MODE::SINGLE; // set sprite to use a single spritesheet
    sprite.spriteSheet = new olc::Sprite("spritesheet.png"); // define image to use for the spritesheet
    sprite.SetSpriteSize({50, 50}); // define size of each sprite with an olc::vi2d
    sprite.SetSpriteScale(2.0f); // define scale of sprite; 1.0f is original size. Must be above 0 and defaults to 1.0f

    // define states - state name and vector of olc::vi2d to define the top-left position of each frame in the spritesheet
    sprite.AddState("idle", {
        // let's assume a sprite sheet with 8 rows and columns, using the 50x50 sprite size defined above
        {0, 0}, // row 1, column 1 (top left of entire image)
        {0, 50}, // row 1, column 2
        {400, 200}, // row 8, column 4
    });

    sprite.AddState("walking", {
        {50, 0},
        {50, 50},
        {50, 100},
    });

    // set initial state
    sprite.SetState("idle")

    return true;
}

bool OnUserUpdate(float fElapsedTime)
{
    sprite.Draw(felapsedTime, {20.0f, 20.0f}); // draws the sprite at location x:20, y:20 and animates it

    return true;
}
```

### Multiple Sprite Files

```cpp
// define sprite in your PGE program
olc::AnimatedSprite sprite;

bool OnUserCreate()
{
    // configure the sprite
    sprite.mode = olc::AnimatedSprite::SPRITE_MODE::MULTI;
    sprite.SetSpriteSize({50.0f, 50.0f);
    sprite.SetSpriteScale(2.0f);

    // define states - state name and a vector of std::strings that define the location of each image file
    sprite.AddState("idle", {
        "frame1.png",
        "frame2.png",
        "frame3.png",
        "frame4.png",
    });

    // set default state
    sprite.SetState("idle");

    return true;
}

bool OnUserUpdate(float fElapsedTime)
{
    sprite.Draw(fElapsedTime, {20.0f, 20.0f});
}
```

### Flipping Sprites

You may want to flip a sprite vertically or horizontally - for example, if your spritesheet only has right-facing images. This can be achieved by changing the flip mode, like so:

```cpp
sprite.flip = olc::AnimatedSprite::FLIP_MODE::HORIZONTAL; // flip horizontally (e.g. make right-facing image face the left)
sprite.flip = olc::AnimatedSprite::FLIP_MODE::VERTICAL; // flip vertically (e.g. make image upside down)
sprite.flip = olc::AnimatedSprite::FLIP_MODE::NONE; // display original image
```

---

## Contributions

Contributions are more than welcome. Please see [CONTRIBUTING.md](https://github.com/matt-hayward/olcPGEX_AnimatedSprite/blob/develop/CONTRIBUTING.md) for more information.