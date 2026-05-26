#pragma once
#include <cstdint>

// used uint8_t because clang's warning said I could reduce the size of each element from a standard 4 byte int each to 1 byte each so I just went ahead and did it
// This is the Event Type enum
enum class EventType : uint8_t{
    WINDOW_RESIZE,
    WINDOW_CLOSE,
    KEY_EVENT_PRESSED,
    KEY_EVENT_RELEASED,
    MOUSE_EVENT_PRESSED,
    MOUSE_EVENT_RELEASED,
    MOUSE_EVENT_MOVED
};

// This is the Event Category enum
enum class EventCategory : uint8_t{
    EVENT_CATEGORY_APPLICATION,
    EVENT_CATEGORY_KEYBOARD,
    EVENT_CATEGORY_MOUSE
};

// This is the main Abstract Event Class
class Event{
public:
    virtual ~Event() = default;

    virtual EventType getEventType() const = 0;
    virtual EventCategory getEventCategoryFlags() const = 0;
    virtual const char* getName() const = 0;
    bool isInCategory(EventCategory cat) const { return cat == getEventCategoryFlags(); }

    // This is a variable that keeps track of whether the task has been handled or not
    bool handled = false;
};

class WindowResizeEvent : public Event{
public:
    WindowResizeEvent(unsigned int W, unsigned int H) : _width(W), _height(H) {}

    EventType getEventType() const override { return EventType::WINDOW_RESIZE; }
    static EventType getStaticType() { return EventType::WINDOW_RESIZE; }

    EventCategory getEventCategoryFlags() const override { return EventCategory::EVENT_CATEGORY_APPLICATION; }
    const char* getName() const override { return "ResizeWindow"; }

    unsigned int getWidth() const { return _width; }
    unsigned int getHeight() const { return _height; }

private:
    unsigned int _width, _height;
};

class WindowCloseEvent : public Event {
public:
    WindowCloseEvent() = default;
    EventType getEventType() const override { return EventType::WINDOW_CLOSE; }
    static EventType getStaticType() { return EventType::WINDOW_CLOSE; }
    EventCategory getEventCategoryFlags() const override { return EventCategory::EVENT_CATEGORY_APPLICATION; }
    const char* getName() const override { return "CloseWindow"; }
};
