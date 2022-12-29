#pragma once

#include <functional> // std::function
#include "Globals.h"

namespace game::events
{
    // Base Event type
    struct Event {
        explicit Event(const std::string& eventType) : type{ eventType } {}
        virtual ~Event() = default;
        std::string type;
    };

    using EventHandler = std::function<void(const Event&)>;

    /*
    *
    * Usage:
    *
    *    events::subscribe(events::EventNameChanged::type, [](const events::Event& event)
    *    {
    *        std::cout << "caught event: " << event.type << std::endl;
    *        const events::EventNameChanged& derivedEvent = static_cast<const events::EventNameChanged&>(event);
    *        std::cout << derivedEvent.oldName << " - " << derivedEvent.newName;
    *    });
    *
    *    events::publish(events::EventNameChanged{ "old", "new" });
    *
    *
    */
    void publish(const Event& event);
    void subscribe(const std::string& eventType, EventHandler&& handler); // todo: return a handle for unsubscribe
    void clear();

    struct ColouredLineEvent : public Event
    {
        constexpr static auto type = "ColouredLineEvent";
        
        ColouredLineEvent() = delete;
        ColouredLineEvent(const ColouredLine& tdl) 
            : Event(type)
            , tdl{tdl}
        {}

        ColouredLine tdl;
    };

    struct ColouredRectEvent : public Event
    {
        constexpr static auto type = "ColouredRectEvent";

        ColouredRectEvent() = delete;
        ColouredRectEvent(const ColouredRect& cr)
            : Event(type)
            , colouredRect{ cr }
        {}

        ColouredRect colouredRect;
    };


}