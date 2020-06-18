/*
	olcPGEX_AnimatedSprite.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                AnimatedSprites - v2.0.0			              |
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
	Moros1138 - https://github.com/Moros1138
*/

#ifndef OLC_PGEX_ANIMATEDSPRITE
#define OLC_PGEX_ANIMATEDSPRITE

namespace olc
{
	class AnimatedSprite : public olc::PGEX
	{
	public:
		enum class SPRITE_MODE {
			MULTI,
			SINGLE
		};
		enum class SPRITE_TYPE {
			SPRITE,
			DECAL
		};
		enum class PLAY_MODE {
			LOOP,
			PING_PONG
		};

	public:
		// Set current state of sprite
		void SetState(std::string newState);
		// Get current sprite state
		std::string GetState();
		// Draw sprite
		void Draw(float fElapsedTime, olc::vf2d position, uint8_t flip = olc::Sprite::Flip::NONE, olc::Pixel tint = olc::WHITE);
		// Add state for sprite in SPRITE_MODE::MULTI with a specified frameDuration and playMode
		void AddState(std::string stateName, float frameDuration, PLAY_MODE mode, std::vector<std::string> imagePaths);
		// Add state for sprite in SPRITE_MODE::SINGLE with a specified frameDuration and playMode
		void AddState(std::string stateName, float frameDuration, PLAY_MODE mode, std::vector<olc::vi2d> spriteLocations);
		// Add state for sprite in SPRITE_MODE::MULTI using the default frameDuration and playMode
		void AddState(std::string stateName, std::vector<std::string> imagePaths);
		// Add state for sprite in SPRITE_MODE::SINGLE using the default frameDuration and playMode
		void AddState(std::string stateName, std::vector<olc::vi2d> spriteLocations);
		// Set size of sprite
		void SetSpriteSize(olc::vi2d size);
		// Get size of sprite
		olc::vi2d GetSpriteSize();
		// Set sprite scale factor
		void SetSpriteScale(float scale);

	protected:
		olc::Sprite* GetMultiFrame(float fElapsedTime);
		olc::Decal* GetMultiRenderable(float fElapsedTime);
		olc::vi2d GetSingleFrame(float fElapsedTime);
		olc::vf2d GetDecalScale(uint8_t flip);
		olc::vf2d GetDecalPosition(olc::vf2d position, uint8_t flip);

	public:
		float defaultFrameDuration = 0.1f; // Frame duration to be used if one is not specified otherwise
		SPRITE_MODE mode = SPRITE_MODE::MULTI;
		SPRITE_TYPE type = SPRITE_TYPE::SPRITE;
		Renderable* spriteSheet;

	protected:
		std::string state;
		std::map<std::string, std::vector<olc::Sprite*>> multiFrames;
		std::map<std::string, std::vector<olc::vi2d>> singleFrames;
		std::map<std::string, std::vector<Renderable*>> multiRenderables;
		std::map<std::string, float> frameDurations;
		std::map<std::string, PLAY_MODE> playModes;
		float frameTimer = 0.0f, spriteScale = 1.0f;
		int currentFrame;
		olc::vi2d spriteSize;
		olc::Sprite* placeholder = nullptr;
		bool playForward = true;
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
			frameTimer = 0.0f;

			if (playModes[state] == PLAY_MODE::PING_PONG && !playForward) {
				currentFrame--;
			} else {
				currentFrame++;
			}

			if (currentFrame >= multiFrames[state].size()) {
				currentFrame = playModes[state] == PLAY_MODE::LOOP
					? 0
					: multiFrames[state].size() - 2;
				playForward = false;
			} else if (currentFrame <= 0) {
				playForward = true;
			}
		}

		return multiFrames[state][currentFrame];
	}

	olc::Decal* AnimatedSprite::GetMultiRenderable(float fElapsedTime)
	{
		frameTimer += fElapsedTime;

		if (frameTimer >= frameDurations[state]) {
			frameTimer = 0.0f;

			if (playModes[state] == PLAY_MODE::PING_PONG && !playForward) {
				currentFrame--;
			} else {
				currentFrame++;
			}

			if (currentFrame >= multiRenderables[state].size()) {
				currentFrame = playModes[state] == PLAY_MODE::LOOP
					? 0
					: multiRenderables[state].size() - 2;
				playForward = false;
			} else if (currentFrame <= 0) {
				playForward = true;
			}
		}

		return multiRenderables[state][currentFrame]->Decal();
	}

	olc::vi2d AnimatedSprite::GetSingleFrame(float fElapsedTime)
	{
		frameTimer += fElapsedTime;

		if (frameTimer >= frameDurations[state]) {
			frameTimer = 0.0f;

			if (playModes[state] == PLAY_MODE::PING_PONG && !playForward) {
				currentFrame--;
			} else {
				currentFrame++;
			}

			if (currentFrame >= singleFrames[state].size()) {
				currentFrame = playModes[state] == PLAY_MODE::LOOP
					? 0
					: singleFrames[state].size() - 2;
				playForward = false;
			} else if (currentFrame <= 0) {
				playForward = true;
			}
		}

		return singleFrames[state][currentFrame];
	}

	void AnimatedSprite::SetState(std::string newState)
	{
		bool stateFound = false;
		if (type == SPRITE_TYPE::SPRITE) {
			if ((mode == SPRITE_MODE::MULTI && multiFrames.find(newState) == multiFrames.end())
				|| (mode == SPRITE_MODE::SINGLE && singleFrames.find(newState) == singleFrames.end())) {

				std::cout << "Error: State " << newState << " does not exist." << std::endl;
				return;
			}
		} else if ((mode == SPRITE_MODE::MULTI && multiRenderables.find(newState) == multiRenderables.end())
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
		AnimatedSprite::AddState(stateName, defaultFrameDuration, PLAY_MODE::LOOP, imgPaths);
	}

	void AnimatedSprite::AddState(std::string stateName, std::vector<olc::vi2d> spriteLocations)
	{
		AnimatedSprite::AddState(stateName, defaultFrameDuration, PLAY_MODE::LOOP, spriteLocations);
	}

	void AnimatedSprite::AddState(std::string stateName, float frameDuration, PLAY_MODE mode, std::vector<std::string> imgPaths)
	{
		for (std::string& path : imgPaths) {
			if (type == SPRITE_TYPE::SPRITE) {
				multiFrames[stateName].push_back(new olc::Sprite(path));
			} else {
				multiRenderables[stateName].push_back(new Renderable());
				multiRenderables[stateName].back()->Load(path);
			}
		}

		frameDurations[stateName] = frameDuration;
		playModes[stateName] = mode;
	}

	void AnimatedSprite::AddState(std::string stateName, float frameDuration, PLAY_MODE mode, std::vector<olc::vi2d> spriteLocations)
	{
		for (olc::vi2d& location : spriteLocations) {
			singleFrames[stateName].push_back(location);
		}

		frameDurations[stateName] = frameDuration;
		playModes[stateName] = mode;
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

	void AnimatedSprite::Draw(float fElapsedTime, olc::vf2d position, uint8_t flip, olc::Pixel tint)
	{
		if (mode == SPRITE_MODE::MULTI) {
			if (type == SPRITE_TYPE::SPRITE) {
				pge->DrawSprite(position, GetMultiFrame(fElapsedTime), spriteScale, flip);
			} else {
				pge->DrawDecal(GetDecalPosition(position, flip), GetMultiRenderable(fElapsedTime), GetDecalScale(flip), tint);
			}
		}
		else {
			if (type == SPRITE_TYPE::SPRITE) {
				pge->DrawPartialSprite(position, spriteSheet->Sprite(), GetSingleFrame(fElapsedTime), spriteSize, spriteScale, flip);
			} else {
				pge->DrawPartialDecal(GetDecalPosition(position, flip), spriteSheet->Decal(), GetSingleFrame(fElapsedTime), spriteSize, GetDecalScale(flip), tint);
			}
		}
	}

	olc::vf2d AnimatedSprite::GetDecalScale(uint8_t flip)
	{
		olc::vf2d scale = { (float)spriteScale, (float)spriteScale };

		if (flip == olc::Sprite::Flip::HORIZ) {
			return { -(scale.x), scale.y };
		}
		
		if (flip == olc::Sprite::Flip::VERT) {
			return { scale.x, -scale.y };
		}

		return scale;
	}

	olc::vf2d AnimatedSprite::GetDecalPosition(olc::vf2d position, uint8_t flip)
	{
		if (flip == olc::Sprite::Flip::HORIZ) {
			return { position.x + (spriteSize.x * spriteScale), position.y };
		}

		if (flip == olc::Sprite::Flip::VERT) {
			return { position.x, position.y + (spriteSize.y * spriteScale) };
		}

		return position;
	}
}

#endif
#endif