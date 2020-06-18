# olcPGEX_AnimatedSprite

This extension is designed for use with the [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine) by [Javidx9](https://github.com/OneLoneCoder). It allows you to easily use animated sprites that utilise either single-file spritesheets or multiple image files for each frame.

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
    olc::Renderable spritesheet = new olc::Renderable();
    spritesheet.Load("spritesheet.png");
    sprite.mode = olc::AnimatedSprite::SPRITE_MODE::SINGLE; // set sprite to use a single spritesheet
    sprite.spriteSheet = spritesheet; // define image to use for the spritesheet
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

### Customising Animations

The Animated Sprite PGEX supports two animation modes: loop and 'ping-pong'.

In loop mode, when an animation reaches its final frame, it starts again from the first. This gives it the pattern of 123123123...

In 'ping-pong' mode, when an animation reaches its final frame, it starts backwards until it reaches the first frame again, then goes back. This gives it the pattern of 12321232123...

It is also possible to define the frame length of each animation. This determines the amount of time in seconds, a frame is displayed on screen before the next one is shown.

Animation mode and frame duration are defined on a per-state basis, when the `AddState` method is called:

```cpp
sprite.AddState("foo", 2.0f, olc::AnimatedSprite::PLAY_MODE::LOOP, foo); // set each frame to display for two seconds, with the default - loop - play mode
sprite.AddState("bar", 0.1f, olc::AnimatedSprite::PLAY_MODE::PING_PONG, bar); // set play mode to ping pong with default frame duration of 0.1 seconds
```

### Flipping Sprites

You may want to flip a sprite vertically or horizontally - for example, if your spritesheet only has right-facing images. This can be achieved by passing in an `olc::Pixel::Flip` when drawing:

```cpp
sprite.Draw(fElapsedTime, {10.0f, 10.0f}, olc::Sprite::Flip::NONE); // no flip - default
sprite.Draw(fElapsedTime, {10.0f, 10.0f}, olc::Sprite::Flip::HORIZ); // flip sprite horizontally
sprite.Draw(fElapsedTime, {10.0f, 10.0f}, olc::Sprite::Flip::VERT); // flip sprite vertically
```

### Using Decals

You can draw Decals rather than Sprites by setting the sprite type for your animated sprite:

```cpp
olc::AnimatedSprite sprite;

sprite.type = olc::AnimatedSprite::SPRITE_TYPE::SPRITE; // set animated sprite to draw sprites, the default behaviour
sprite.type = olc::AnimatedSprite::SPRITE_TYPE::DECAL; // set animated sprite to draw decals
```

**NOTE:** If using decals, you must define the states after the Pixel Game Engine has fully loaded to ensure that the decals are correctly initialised. As such, you should do this in the `OnUserCreate` method of your PGE-derived class.

### Tinting Decals

The `olc::AnimatedSprite::Draw()` method accepts an optional `olc::Pixel` parameter which will tint the sprite in the specified colour. **Note:** This only works for decals.

```cpp
// assuming sprite is using decals...
sprite.Draw(fElapsedTime, {10.0f, 10.0f}, olc::Sprite::Flip::NONE, olc::WHITE); // draw sprite with default white tinting (no tinting)
sprite.Draw(fElapsedTime, {10.0f, 10.0f}, olc::Sprite::Flip::NONE, olc::CYAN); // draw sprite with cyan tinting
```

---

## Contributions

Contributions are more than welcome. Please see [CONTRIBUTING.md](https://github.com/matt-hayward/olcPGEX_AnimatedSprite/blob/develop/CONTRIBUTING.md) for more information.