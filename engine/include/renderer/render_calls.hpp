#pragma once

#include "renderer/api.hpp"


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

    static void clear_depth_buffer_bit_only(){
        _render_api->clear_depth_buffer_bit_only();
    }

    static void toggle_cursor_input_mode(CursorInputMode mode){
        _render_api->toggle_cursor_input_mode(mode);
    }

    static void bindTexture(uint32_t _id, uint32_t slot){
        _render_api->bindTexture(_id, slot);
    }

    static void draw_full_screen_quad(){
        _render_api->draw_full_screen_quad();
    }

    static float get_time(){
        return _render_api->get_time();
    }

private:
    static std::unique_ptr<RenderingAPI> _render_api;
};


// A relatively lightweight struct to establish a base for a general draw call
struct RenderCall{
    uint32_t _shader_id;
    uint32_t _material_id;

    std::shared_ptr<VertexArray> _vao;
    uint32_t _idx_count;

    glm::mat4 _model_matrix;

    bool _is_transparent = false;
    float _dist_to_cam = 0.0F;
};

class DrawCommands{
public:
    // Binds the VAO even if it's been previously bound
    static void DrawIndexed(const std::shared_ptr<VertexArray>& vao, uint32_t indexCount);
    // Assumes that the required vao is already bound
    static void DrawIndexed(uint32_t indexCount);
    // Static function to unbind any currently bound vao
    static void UnbindVAO();
};
