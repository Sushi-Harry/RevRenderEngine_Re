#pragma once

#include <cstdint>
#include <memory>


// These map to the GL enums written next to them
enum class ShadowMapType : uint8_t{
    REV_SHADOW_MAP_DIRECTIONAL = 0, // GL_TEXTURE_2D
    REV_SHADOW_MAP_SPOT = 1, // GL_TEXTURE_2D_ARRAY
    REV_SHADOW_MAP_POINT = 2 // GL_TEXTURE_CUBE_MAP
};

struct ShadowMapSpecs{
    ShadowMapType _type = ShadowMapType::REV_SHADOW_MAP_SPOT;
    uint32_t _width = 1024;
    uint32_t _height = 1024;
    uint32_t _slices = 4;
};

class ShadowMap {
public:
    virtual ~ShadowMap() = default;

    virtual void BindFramebuffer() const = 0;
    virtual void UnbindFramebuffer() const = 0;

    // This function is a bit confusing as for why it's in this class. But here's the explanation:
    // It'll change the texture layer to which stuff is drawn in case I'm using arrays or cubemaps. 6 layers for cubemaps and sizeof(array object) for arrays.
    virtual void BindLayerTarget(uint32_t layerIndex) const = 0;

    virtual void BindTexture(uint32_t slot) const = 0;

    virtual uint32_t GetTextureID() const = 0;
    virtual const ShadowMapSpecs& GetShadowMapSpecs() const = 0;

    static std::shared_ptr<ShadowMap> Create(const ShadowMapSpecs& specs);
};
