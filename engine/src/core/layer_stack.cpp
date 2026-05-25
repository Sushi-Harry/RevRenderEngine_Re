#include "core/layer_stack.hpp"
#include <algorithm>

LayerStack::~LayerStack(){
    for(Layer* layer : _layers){
        layer->onDetach();
        delete layer;
    }
}

void LayerStack::push_layer(Layer* layer){
    _layers.emplace(_layers.begin() + _layer_idx, layer);
    _layer_idx++;
    layer->onAttach();
}

void LayerStack::push_overlay(Layer* overlay){
    _layers.emplace_back(overlay);
    overlay->onAttach();
}

void LayerStack::pop_layer(Layer* layer){
    auto iter = std::find(_layers.begin(), _layers.begin() + _layer_idx, layer);
    if(iter != _layers.begin() + _layer_idx ){
        layer->onDetach();
        _layers.erase(iter);
        _layer_idx--;
    }
}

void LayerStack::pop_overlay(Layer* overlay){
    auto iter = std::find(_layers.begin(), _layers.begin() + _layer_idx, overlay);
    if(iter != _layers.end()){
        overlay->onDetach();
        _layers.erase(iter);
    }
}
