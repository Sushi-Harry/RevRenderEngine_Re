#pragma once

#include "renderer/camera.hpp"
#include "renderer/render_calls.hpp"
#include "renderer/shader.hpp"

class Renderer{
public:
    static void Init();
    static void Shutdown();

    static void onWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(Camera3D& cam);
    static void EndScene();
private:

};
