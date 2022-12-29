#include "EventSystem.h"
#include <map>

namespace
{
    std::map<std::string, std::vector<game::events::EventHandler>> m_eventHandlers;
}

namespace game::events
{
    void clear()
    {
        m_eventHandlers.clear();
    }

    void publish(const Event& event)
    {
        if (m_eventHandlers.find(event.type) == m_eventHandlers.end())
        {
            return;
        }

        std::vector<EventHandler>& eventHandlers = m_eventHandlers[event.type];
        for (auto& eventHandler : eventHandlers)
        {
            eventHandler(event);
        }

    }
    void subscribe(const std::string& eventName, EventHandler&& handler)
    {
        m_eventHandlers[eventName].push_back(handler);
    }
}
