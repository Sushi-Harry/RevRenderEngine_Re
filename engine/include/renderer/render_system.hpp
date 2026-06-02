#pragma once

#include "renderer/render_calls.hpp"
#include "core/resource_manager.hpp"

class RenderSystem {
public:
    void BeginFrame();
    void Submit(const RenderCall& _render_packet);
    void EndFrame(const ResourceManager& res_mgr, const glm::mat4& view_proj_mat);
private:
    std::vector<RenderCall> _render_queue;
};
