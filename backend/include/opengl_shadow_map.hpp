#pragma once

#include "renderer/shadow_map.hpp"

class opengl_shadow_map : public ShadowMap{
public:
    opengl_shadow_map(const ShadowMapSpecs& specs);
    ~opengl_shadow_map();

    void BindFramebuffer() const override;
    void UnbindFramebuffer() const override;

    void BindLayerTarget(uint32_t layerIndex) const override;

    void BindTexture(uint32_t slot) const override;

    uint32_t GetTextureID() const override { return _texture_id; };
    const ShadowMapSpecs& GetShadowMapSpecs() const override { return _specs; };

private:
    void AllocateResources();

    uint32_t _framebuffer_id = 0;
    uint32_t _texture_id = 0;
    ShadowMapSpecs _specs;
};
