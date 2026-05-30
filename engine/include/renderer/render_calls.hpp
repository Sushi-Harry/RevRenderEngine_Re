#pragma once

#include "renderer/api.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"

class GeneralRenderCalls{
public:
    static void Init(){
        _render_api->Init();
    }
    static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
        _render_api->setViewport(x, y, width, height);
    }
    static void clearColor(const glm::vec4& color){
        _render_api->clearColor(color);
    }
    static void clear(){
        _render_api->clear();
    }

    static void drawElements(const std::shared_ptr<VertexArray>& vert_array, uint32_t idx_cnt = 0){
        _render_api->drawElements(vert_array, idx_cnt);
    }
    static void drawLines(const std::shared_ptr<VertexArray>& vert_array, uint32_t vert_cnt){
        _render_api->drawLines(vert_array, vert_cnt);
    }

    static void setLineWidth(float width){
        _render_api->setLineWidth(width);
    }

private:
    static std::unique_ptr<RenderingAPI> _render_api;
};


// A relatively lightweight struct to establish a base for a general draw call
struct RenderCall{
    std::shared_ptr<VertexArray> _vertex_array;
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<Texture2D> _texture;
    glm::mat4 _transform;
    float _depth;
};

// --------CONTINUE FROM HERE------------
