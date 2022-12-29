#pragma once

#include <functional> // std::function
#include "Globals.h"

namespace game::events
{
    // Lets define a base event type
    struct Event {
        explicit Event(const std::string& eventType) : type{ eventType } {}
        virtual ~Event() = default;
        std::string type;
    };

    // Next our observer interface
    using EventHandler = std::function<void(const Event&)>;

    struct EventAddTopDownLine : public Event
    {
        constexpr static auto type = "EventAddTopDownLine";

        EventAddTopDownLine(const TopDownLine& tdl) 
            : Event(type)
            , tdl{tdl}
        {}

        TopDownLine tdl;
    };

    /*
    * 
    * Usage:
    * 
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
    void subscribe(const std::string& eventName, EventHandler&& handler);
    void clear();
}