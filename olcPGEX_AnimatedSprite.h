/*
	olcPGEX_AnimatedSprite.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                AnimatedSprites - v1.1.0			              |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	the ability to easily animate sprites with either a single
	spritesheets or individual image files for each frame.

	Use of this extension requires the olcPGEX_Graphics2D extension.

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2019 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Links
	~~~~~
	Homepage:	https://matthewhayward.co.uk

	Author
	~~~~~~
	Matt Hayward aka SaladinAkara

	Contributors
	~~~~~~~~~~~~
	0xnicholasc - https://github.com/0xnicholasc
*/

#include "olcPGEX_Graphics2D.h"

#ifndef OLC_PGEX_ANIMATEDSPRITE
#define OLC_PGEX_ANIMATEDSPRITE

namespace olc
{
	class AnimatedSprite : public olc::PGEX
	{
	public:
		// Set current state of sprite
		void SetState(std::string newState);
		// Get current sprite state
		std::string GetState();
		// Draw sprite
		void Draw(float fElapsedTime, olc::vf2d position);
		// Add state for sprite in SPRITE_MODE::MULTI with a specified frameDuration
		void AddState(std::string stateName, float frameDuration, std::vector<std::string> imagePaths);
		// Add state for sprite in SPRITE_MODE::SINGLE with a specified frameDuration
		void AddState(std::string stateName, float frameDuration, std::vector<olc::vi2d> spriteLocations);
		// Add state for sprite in SPRITE_MODE::MULTI using the default frameDuration
		void AddState(std::string stateName, std::vector<std::string> imagePaths);
		// Add state for sprite in SPRITE_MODE::SINGLE using the default frameDuration
		void AddState(std::string stateName, std::vector<olc::vi2d> spriteLocations);
		// Set size of sprite
		void SetSpriteSize(olc::vi2d size);
		// Get size of sprite
		olc::vi2d GetSpriteSize();
		// Set sprite scale factor
		void SetSpriteScale(float scale);

	protected:
		olc::Sprite* GetMultiFrame(float fElapsedTime);
		olc::vi2d GetSingleFrame(float fElapsedTime);

	public:
		bool flipped = false;
		float defaultFrameDuration = 0.1f; // Frame duration to be used if one is not specified otherwise
		enum class FLIP_MODE {
			NONE = 0,
			HORIZONTAL = 1,
			VERTICAL = 2
		};
		enum class SPRITE_MODE {
			MULTI = 0,
			SINGLE = 1
		};
		FLIP_MODE flip = FLIP_MODE::NONE;
		SPRITE_MODE mode = SPRITE_MODE::MULTI;
		olc::Sprite* spriteSheet = nullptr;

	protected:
		std::string state;
		std::map<std::string, std::vector<olc::Sprite*>> multiFrames;
		std::map<std::string, std::vector<olc::vi2d>> singleFrames;
		std::map<std::string, float> frameDurations;
		float frameTimer = 0.0f;
		unsigned int currentFrame;
		olc::vi2d spriteSize;
		float spriteScale = 1.0f;
		olc::Sprite* placeholder = nullptr;
	};
}

#ifdef OLC_PGEX_ANIMSPR
#undef OLC_PGEX_ANIMSPR

namespace olc
{
	olc::Sprite* AnimatedSprite::GetMultiFrame(float fElapsedTime)
	{
		frameTimer += fElapsedTime;

		if (frameTimer >= frameDurations[state]) {
			currentFrame++;
			frameTimer = 0.0f;

			if (currentFrame >= multiFrames[state].size()) {
				currentFrame = 0;
			}
		}

		return multiFrames[state][currentFrame];
	}

	olc::vi2d AnimatedSprite::GetSingleFrame(float fElapsedTime)
	{
		frameTimer += fElapsedTime;

		if (frameTimer >= frameDurations[state]) {
			currentFrame++;
			frameTimer = 0.0f;

			if (currentFrame >= singleFrames[state].size()) {
				currentFrame = 0;
			}
		}

		return singleFrames[state][currentFrame];
	}

	void AnimatedSprite::SetState(std::string newState)
	{
		if ((mode == SPRITE_MODE::MULTI && multiFrames.find(newState) == multiFrames.end())
			|| (mode == SPRITE_MODE::SINGLE && singleFrames.find(newState) == singleFrames.end())) {

			std::cout << "Error: State " << newState << " does not exist." << std::endl;
			return;
		}

		if (newState != state) {
			state = newState;
			currentFrame = 0;
		}
	}


	std::string AnimatedSprite::GetState()
	{
		return state;
	}

	void AnimatedSprite::AddState(std::string stateName, std::vector<std::string> imgPaths)
	{
		AnimatedSprite::AddState(stateName, defaultFrameDuration, imgPaths);
	}

	void AnimatedSprite::AddState(std::string stateName, std::vector<olc::vi2d> spriteLocations)
	{
		AnimatedSprite::AddState(stateName, defaultFrameDuration, spriteLocations);
	}

	void AnimatedSprite::AddState(std::string stateName, float frameDuration, std::vector<std::string> imgPaths)
	{
		for (std::string& path : imgPaths) {
			multiFrames[stateName].push_back(new olc::Sprite(path));
		}

		frameDurations[stateName] = frameDuration;
	}

	void AnimatedSprite::AddState(std::string stateName, float frameDuration, std::vector<olc::vi2d> spriteLocations)
	{
		for (olc::vi2d& location : spriteLocations) {
			singleFrames[stateName].push_back(location);
		}

		frameDurations[stateName] = frameDuration;
	}

	void AnimatedSprite::SetSpriteSize(olc::vi2d size)
	{
		spriteSize = size;
		if (placeholder != nullptr) {
			delete placeholder;
		}
		placeholder = new olc::Sprite(size.x, size.y);
	}

	olc::vi2d AnimatedSprite::GetSpriteSize()
	{
		return spriteSize;
	}

	void AnimatedSprite::SetSpriteScale(float scale)
	{
		if (scale <= 0.0f) {
			spriteScale = 1.0f;
		}
		else {
			spriteScale = scale;
		}
	}

	void AnimatedSprite::Draw(float fElapsedTime, olc::vf2d position)
	{
		olc::GFX2D::Transform2D t;

		if (flip == FLIP_MODE::HORIZONTAL) {
			t.Translate(-spriteSize.x, 0);
			t.Scale(-spriteScale, spriteScale);
		} else if (flip == FLIP_MODE::VERTICAL) {
			t.Translate(0, -spriteSize.y);
			t.Scale(spriteScale, -spriteScale);
		} else {
			t.Scale(spriteScale, spriteScale);
		}

		t.Translate(position.x, position.y);

		if (mode == SPRITE_MODE::MULTI) {
			olc::GFX2D::DrawSprite(GetMultiFrame(fElapsedTime), t);
		}
		else {
			olc::Pixel::Mode currentPixelMode = pge->GetPixelMode();
			olc::Sprite* currentDrawTarget = pge->GetDrawTarget();

			pge->SetDrawTarget(placeholder);
			pge->Clear(olc::BLANK);
			pge->SetPixelMode(olc::Pixel::NORMAL);
			pge->DrawPartialSprite({ 0, 0 }, spriteSheet, GetSingleFrame(fElapsedTime), spriteSize);
			pge->SetDrawTarget(currentDrawTarget);
			pge->SetPixelMode(currentPixelMode);
			olc::GFX2D::DrawSprite(placeholder, t);
		}
	}
}

#endif
#endif