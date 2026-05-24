#pragma once

#include "core/mouse.hpp"
#include "events/events.hpp"

class MouseMoved : public Event{
public:
    MouseMoved(const float x, const float y) : _x(x), _y(y) {}

    float X() const { return _x; }
    float Y() const { return _y; }

    EventType getEventType() const override { return EventType::MOUSE_EVENT_MOVED; }
    EventCategory getEventCategoryFlags() const override { return EventCategory::EVENT_CATEGORY_INPUT; }
    static EventType getStaticType() { return EventType::MOUSE_EVENT_MOVED; }
    const char* getName() const override { return "MouseMoved"; }

private:
    float _x, _y;
};

class MouseButtonPressed : public Event {
public:
    MouseButtonPressed(const Mouse mb_code, bool repeat = false) : _mb(mb_code), _repeat(repeat) {}

    Mouse getMB() const { return _mb; }

    EventType getEventType() const override { return EventType::MOUSE_EVENT_PRESSED; }
    EventCategory getEventCategoryFlags() const override { return EventCategory::EVENT_CATEGORY_INPUT; }
    static EventType getStaticType() { return EventType::MOUSE_EVENT_PRESSED; }
    const char* getName() const override { return "MouseButtonPressed"; }

    bool isRepeat() const { return _repeat; }

private:
    Mouse _mb;
    bool _repeat;
};

class MouseButtonReleased : public Event{
public:
    MouseButtonReleased(const Mouse mb_code) : _mb(mb_code) {}

    Mouse getMB() const { return _mb; }

    EventType getEventType() const override { return EventType::MOUSE_EVENT_RELEASED; }
    EventCategory getEventCategoryFlags() const override { return EventCategory::EVENT_CATEGORY_INPUT; }
    static EventType getStaticType() { return EventType::MOUSE_EVENT_RELEASED; }
    const char* getName() const override { return "MouseReleased"; }

private:
    Mouse _mb;
};
