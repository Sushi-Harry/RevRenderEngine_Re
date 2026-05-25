#pragma once

#include "core/layer.hpp"
#include <vector>

class LayerStack{
public:
    LayerStack() = default;
    ~LayerStack();

    void push_layer(Layer* layer);
    void push_overlay(Layer* overlay);

    void pop_layer(Layer* layer);
    void pop_overlay(Layer* overlay);

    std::vector<Layer*>::iterator begin() { return _layers.begin(); }
    std::vector<Layer*>::iterator end() { return _layers.end(); }

    std::vector<Layer*>::reverse_iterator rbegin() { return _layers.rbegin(); }
    std::vector<Layer*>::reverse_iterator rend() { return _layers.rend(); }

private:
    std::vector<Layer*> _layers;
    unsigned int _layer_idx = 0;
};


// I could've used separate vectors for normal layers and overlay layers but I didn't cause from what I understand, later if I want to check for a general layer variable and i don't know where it might be, I'll have to traverse both arrays separately
// That would still kinda be equal in terms of speed I imagine but would make for messy code later on
// So I'm going with the same approach as the hazel engine.
// I know everything looks identical to hazel engine so far but I promise I'm looking up why I'm writing what I'm writing and trying my best to understand why I'm writing it like this.
// Real differences will start appearing in the renderer part of the engine later so yeah
