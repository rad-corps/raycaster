#include "IGameState.h"

namespace game
{
	void IGameState::pushPendingState(std::unique_ptr<IGameState> nextState_)
	{
		m_nextState = std::move(nextState_);
	}

	std::unique_ptr<IGameState> IGameState::popPendingState()
	{
		if (m_nextState)
		{
			return std::move(m_nextState);
		}
		return nullptr;
	}
	
	bool IGameState::hasPendingState()
	{
		return m_nextState ? true : false;
	}
}