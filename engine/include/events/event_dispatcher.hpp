#pragma once
#include "events/events.hpp"

// This is the event dispatcher class. This class calls the associated functions from the application class and updates the value of _event.handled according to whether the function was called successfully (1) or not (0)
class EventDispatcher{
public:
    EventDispatcher(Event& e) : _event(e) {}

    // Now F is the callback function and then T is the type of event I wanna look for
    template<typename T, typename F>
    bool Dispatch(const F& callback){
        if(_event.getEventType() == T::getStaticType()){
            // This statement makes _event.handled's value equal to the result of the callback function being called. If callback function runs successfully it returns 1 and then event.handled becomes 1 otherwise it becomes 0. Great stuff huh?
            _event.handled |= callback(static_cast<T&>(_event));
            return true;
        }
        return false;
    }

private:
    Event& _event;
};

// Here's the flow of the events
// Application has an onEvent function that takes an event as an argument and then creates an event dispatcher object to handle and dispatch the event. Simple.
