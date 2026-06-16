#include "core/application.hpp"
#include "core/window.hpp"

int main(){

    Window* Window = Window::create(WindowProperties("RevRender RE", 1440, 810));
    auto app = std::make_unique<Application>(Window, "RevRender RE");

    app->Run();
    return 0;
}
