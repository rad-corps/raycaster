#include "RenderComponent.h"
#include "GameObject.h"

namespace game
{
	RenderComponent::RenderComponent(GameObject* gameObject)
		: m_gameObject{ gameObject }
	{
		std::cout << "RenderComponent: " << m_gameObject->m_transform.pos.x << ", " << m_gameObject->m_transform.pos.y << std::endl;
	}
}