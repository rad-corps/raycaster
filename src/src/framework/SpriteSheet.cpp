#include "SpriteSheet.h"

namespace rcgf
{
	SpriteSheet::SpriteSheet(
		std::unique_ptr<Texture> texture,
		int pxSpriteWidth,
		int pxSpriteHeight,
		int rows,
		int cols
	)
		: m_texture{std::move(texture)}
		, m_pxSpriteWidth(pxSpriteWidth)
		, m_pxSpriteHeight{pxSpriteHeight}
		, m_rows{rows}
		, m_cols{cols}
	{}


	void SpriteSheet::render(
		int animIdx,
		int x,
		int y
	)
	{
		// discover row and col
		const int col = animIdx % m_cols;
		const int row = animIdx / m_cols;

		// discover x position (col)
		const int spriteSheetX = col * m_pxSpriteWidth;
		
		// discover y position (row)
		const int spriteSheetY = row * m_pxSpriteHeight;

		SDL_Rect clip{};
		clip.x = spriteSheetX;
		clip.y = spriteSheetY;
		clip.w = m_pxSpriteWidth;
		clip.h = m_pxSpriteHeight;

		m_texture->render(x, y, &clip);
	}

	void SpriteSheet::render(int animIdx,SDL_Rect* dstRect)
	{
		// discover row and col
		const int col = animIdx % m_cols;
		const int row = animIdx / m_cols;

		// discover x position (col)
		const int spriteSheetX = col * m_pxSpriteWidth;

		// discover y position (row)
		const int spriteSheetY = row * m_pxSpriteHeight;

		SDL_Rect clip{};
		clip.x = spriteSheetX;
		clip.y = spriteSheetY;
		clip.w = m_pxSpriteWidth;
		clip.h = m_pxSpriteHeight;

		m_texture->render2(&clip, dstRect);
	}

	void SpriteSheet::render(int animIdx, SDL_Rect* srcRect, SDL_Rect* dstRect)
	{
		// discover row and col
		const int col = animIdx % m_rows;
		const int row = animIdx / m_cols;
		render(col, row, srcRect, dstRect);

	}

	void SpriteSheet::render(int col, int row, SDL_Rect* srcRect, SDL_Rect* dstRect )
	{
		// discover x position (col)
		const int spriteSheetX = col * m_pxSpriteWidth + srcRect->x;

		// discover y position (row)
		const int spriteSheetY = row * m_pxSpriteHeight + srcRect->y;

		SDL_Rect clip{};
		clip.x = spriteSheetX;
		clip.y = spriteSheetY;
		clip.w = srcRect->w;
		clip.h = srcRect->h;

		m_texture->render2(&clip, dstRect);
	}
	
	Color SpriteSheet::getPixelColor(int animIdx, int x, int y)
	{
		const int col = animIdx % m_cols;
		const int row = animIdx / m_cols;

		// discover x position (col)
		const int spriteSheetX = (col * m_pxSpriteWidth) + x;

		// discover y position (row)
		const int spriteSheetY = (row * m_pxSpriteHeight) + y;

		return m_texture->getPixelColor(spriteSheetX, spriteSheetY);
	}

}