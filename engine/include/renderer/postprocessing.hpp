#pragma once

#include "core/resource_manager.hpp"
#include "renderer/framebuffer.hpp"
#include <string>
#include <cstdint>
#include <iostream>

class PostProcessingEffect;

struct PostProcessingContext{
    glm::vec2 _resolution;
    float _time;
};

class PostProcessingSystem{
public:
    void Init(uint32_t width, uint32_t height);
    void Resize(uint32_t width, uint32_t height);

    template <typename T, typename...Args>
    void AddEffect(Args&&... args){
        _effects.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }
    uint32_t Execute(uint32_t initial_src_texture, ResourceManager& res_mgr, const PostProcessingContext& ctx);

    const std::vector<std::unique_ptr<PostProcessingEffect>>& getEffects() const { return _effects; }
private:
    std::shared_ptr<Framebuffer> _ping_pong_fbos[2];
    std::vector<std::unique_ptr<PostProcessingEffect>> _effects;
};

class PostProcessingEffect{
public:
    PostProcessingEffect(const std::string& name) : _name(name), _is_enabled(false) {}
    virtual ~PostProcessingEffect() = default;

    virtual void Render(ResourceManager& res_mgr, uint32_t source_texture_id, std::shared_ptr<Framebuffer> dest_fbo, const PostProcessingContext& ctx) = 0;

    virtual void onRenderGUI() = 0;

    const std::string& getName() const { return _name; }
    bool isEnabled() const { return _is_enabled; }
    void Enable() { _is_enabled = true; }
    void Disable() { _is_enabled = false; }

protected:
    std::string _name;
    bool _is_enabled;
};

class ChromaticAbberation : public PostProcessingEffect{
public:
    ChromaticAbberation() : PostProcessingEffect("chromatic_aberration"), _intensity(0.005F) {}

    void Render(ResourceManager& res_mgr, uint32_t source_texture_id, std::shared_ptr<Framebuffer> dest_fbo, const PostProcessingContext& ctx) override;
    void onRenderGUI() override;

private:
    float _intensity = 0.0F;

};

class Pixelation : public PostProcessingEffect{
public:
    Pixelation() : PostProcessingEffect("pixelation"), _block_size(8.0F) {}

    void Render(ResourceManager& res_mgr, uint32_t source_texture_id, std::shared_ptr<Framebuffer> dest_fbo, const PostProcessingContext& ctx) override;
    void onRenderGUI() override;

private:
    float _block_size = 8.0F;
};


class CRT : public PostProcessingEffect{
public:
    CRT() : PostProcessingEffect("crt"), _intensity(0.2F), _line_count(1000.0F) {}
    void Render(ResourceManager& res_mgr, uint32_t source_texture_id, std::shared_ptr<Framebuffer> dest_fbo, const PostProcessingContext& ctx) override;
    void onRenderGUI() override;

private:
    float _line_count;
    float _intensity;
};


class FilmGrain : public PostProcessingEffect{
public:
    FilmGrain() : PostProcessingEffect("filmgrain"), _intensity(0.5F) {}
    void Render(ResourceManager& res_mgr, uint32_t source_texture_id, std::shared_ptr<Framebuffer> dest_fbo, const PostProcessingContext& ctx) override;
    void onRenderGUI() override;
private:
    float _intensity;

};


class FXAA : public PostProcessingEffect{
public:
    FXAA() : PostProcessingEffect("fxaa") {}
    void Render(ResourceManager& res_mgr, uint32_t source_texture_id, std::shared_ptr<Framebuffer> dest_fbo, const PostProcessingContext& ctx) override;
    void onRenderGUI() override;
private:

};
