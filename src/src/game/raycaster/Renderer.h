#pragma once

#include "Globals.h"
#include "RaycasterConstants.h"
#include <vector>

namespace game
{
	class Renderer
	{
	public:
		// remove default/copy/move/assignment constructors
		Renderer() = delete;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		Renderer(SDL_Renderer* renderer) : m_renderer{ renderer } {}

		void RenderWalls(const std::vector<ColumnRenderData>& crd);

	private:
		SDL_Renderer* m_renderer;

	};
}

