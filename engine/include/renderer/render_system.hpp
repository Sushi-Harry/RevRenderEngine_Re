#pragma once

#include "renderer/camera.hpp"
#include "renderer/render_calls.hpp"
#include "core/resource_manager.hpp"
#include "renderer/skybox.hpp"
#include "renderer/uniform_buffer.hpp"

class RenderSystem {
public:
    void Init();
    void Shutdown();

    void BeginFrame(const Camera3D& cam);
    void Submit(const RenderCall& _render_packet, ResourceManager& res_mgr);
    void ShadowMappingRenderPass(ResourceManager& res_mgr, const glm::mat4& light_space_matrix);
    glm::mat4 CalculateLightSpaceMatrix(const DirectionalLightComponent& dirLight, const Camera3D& cam);
    void ClearRenderQueue();
    void EndFrame(ResourceManager& res_mgr, const glm::mat4& view_proj_mat, const Camera3D& cam, const std::vector<SpotLightComponent>& active_pl_data, const DirectionalLightComponent& dirLight, uint32_t depth_map_texture_directional = 0);
private:
    std::vector<RenderCall> _render_queue;
    std::shared_ptr<UniformBuffer> _camera_ubo;
    std::shared_ptr<UniformBuffer> _lights_ubo;
};
