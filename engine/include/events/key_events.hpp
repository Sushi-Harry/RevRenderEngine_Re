#pragma once

#include "core/keys.hpp"
#include "events/events.hpp"

// This file has the events related to keyboard inputs

class KeyEvent : public Event{
public:
    KeyEvent(const Key keycode) : _key(keycode) {}

    Key getCode() const { return _key; }
    EventCategory getEventCategoryFlags() const override { return EventCategory::EVENT_CATEGORY_INPUT; }
    const char* getName() const override { return "KeyboardInput"; }

protected:
    Key _key;
};

class KeyPressed : public KeyEvent {
public:
    KeyPressed(const Key keycode, bool repeat = false) : KeyEvent(keycode), _repeat(repeat) {}
    EventType getEventType() const override { return EventType::KEY_EVENT_PRESSED; }
    static EventType getStaticType() { return EventType::KEY_EVENT_PRESSED; }

    Key getCode() const { return _key; }
    EventCategory getEventCategoryFlags() const override { return EventCategory::EVENT_CATEGORY_INPUT; }
    const char* getName() const override { return "KeyPressed"; }

private:
    bool _repeat = false;
};

class KeyReleased : public KeyEvent {
public:
    KeyReleased(const Key keycode) : KeyEvent(keycode) {}
    EventType getEventType() const override { return EventType::KEY_EVENT_RELEASED; }
    static EventType getStaticType() { return EventType::KEY_EVENT_RELEASED; }

    Key getCode() const { return _key; }
    EventCategory getEventCategoryFlags() const override { return EventCategory::EVENT_CATEGORY_INPUT; }
    const char* getName() const override { return "KeyReleased"; }
};
