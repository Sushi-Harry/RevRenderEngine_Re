#pragma once

#include "ecs/scene.hpp"

class Entity{
public:
    Entity(entt::entity handle, Scene* scene) : _entity_handle(handle), _scene(scene) {}
    Entity() = default;

    template<typename T, typename... Args>
    T& addComponent(Args&&... args){
        return _scene->_registry.emplace<T>(_entity_handle, std::forward<Args>(args)...);
    }

    template<typename T>
    T& getComponent(){
        return _scene->_registry.get<T>(_entity_handle);
    }

    template<typename T>
    bool hasComponent(){
        return _scene->_registry.all_of<T>(_entity_handle);
    }

    const entt::entity& getEntityID() const { return _entity_handle; }


private:
    entt::entity _entity_handle{entt::null};
    Scene* _scene = nullptr;
};
