#include "renderer/render_calls.hpp"

std::unique_ptr<RenderingAPI> GeneralRenderCalls::_render_api = RenderingAPI::create();
