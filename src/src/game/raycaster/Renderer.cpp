#include "Renderer.h"

namespace game
{
	void Renderer::RenderWalls(const std::vector<ColumnRenderData>& crdVec)
	{
		for (const ColumnRenderData& crd : crdVec)
		{
			crd.columnTexture->render2(&crd.srcRect, &crd.dstRect);
		}
	}
}

