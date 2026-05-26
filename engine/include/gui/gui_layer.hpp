#pragma once

#include "core/layer.hpp"
#include "events/events.hpp"

class gui_layer : public Layer{
public:
    gui_layer() : Layer("gui_layer") {}
    ~gui_layer() = default;

    void onAttach() override;
    void onDetach() override;
    void onEvent(Event& e) override;

    void begin();
    void end();

    void blockEvents() { _blockEvents = true; }
    void unblockEvents() { _blockEvents = false; }
    void setEventsStatus(bool stat) { _blockEvents = stat; }

private:
    bool _blockEvents = false;
};
