#include "AI_Component.h"
#include "GameObject.h"

namespace game
{
	AI_Component::AI_Component(GameObject* gameObject)
		: m_gameObject{ gameObject }
	{
		std::cout << "AI_Component: " << gameObject->m_transform.pos.x << ", " << gameObject->m_transform.pos.y << std::endl;
	}
}