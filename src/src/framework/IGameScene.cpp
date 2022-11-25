#include "IGameScene.h"

namespace rcgf
{
	void IGameScene::pushPendingState(std::unique_ptr<IGameScene> nextState_)
	{
		m_nextState = std::move(nextState_);
	}

	std::unique_ptr<IGameScene> IGameScene::popPendingState()
	{
		if (m_nextState)
		{
			return std::move(m_nextState);
		}
		return nullptr;
	}
	
	bool IGameScene::hasPendingState()
	{
		return m_nextState ? true : false;
	}
}