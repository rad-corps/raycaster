#include "Animation.h"

namespace rcgf
{
	Animation::Animation(
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


	void Animation::render(
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

}