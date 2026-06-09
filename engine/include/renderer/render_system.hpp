#pragma once

#include "renderer/camera.hpp"
#include "renderer/render_calls.hpp"
#include "core/resource_manager.hpp"

class RenderSystem {
public:
    void BeginFrame();
    void Submit(const RenderCall& _render_packet, ResourceManager& res_mgr);
    void EndFrame(ResourceManager& res_mgr, const glm::mat4& view_proj_mat, const Camera3D& cam, const std::vector<PointLightComponent>& active_pl_data, const DirectionalLightComponent& dirLight = {false});
private:
    std::vector<RenderCall> _render_queue;
};
