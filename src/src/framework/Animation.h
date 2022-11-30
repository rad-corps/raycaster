#pragma once

#include <memory>
#include "Texture.h"

namespace rcgf
{
	// only create one of these for each animation/spritesheet consumed
	// i.e. many sprites will share a single animation
	class Animation
	{
	public:

		// remove default construction and copy assignment/move assignment
		Animation() = delete;
		Animation(const Animation&) = delete;
		Animation& operator=(const Animation&) = delete;
		
		// allow compiler provided move operations
		Animation(Animation&&) = default;
		Animation& operator=(Animation&&) = default;
		
		// TODO: Animation should not have a unique pointer to Texture. Raw pointer is fine. Make this light weight and copyable.
		Animation(
			std::unique_ptr<Texture>, 
			int pxSpriteWidth,
			int pxSpriteHeight,
			int rows,
			int cols
		);

		Color getPixelColor(int animIdx, int x, int y);

		Texture* getTexture()
		{
			return m_texture.get();
		}

		void render(
			int animIdx, 
			int x, 
			int y
		);

		void render(
			int animIdx,
			SDL_Rect* dstRect
		);

	private:
		std::unique_ptr<Texture> m_texture;
		int m_pxSpriteWidth;
		int m_pxSpriteHeight;
		int m_rows;
		int m_cols;
	};
}