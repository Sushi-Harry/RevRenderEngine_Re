#include "glfw_window.hpp"
#include <iostream>

Window* Window::create(const WindowProperties& prop){
    return new glfw_window(prop);
}

// glfw_window wrapper functions' definitions

// Constructor for glfw_window class
glfw_window::glfw_window(const WindowProperties& winProp){
    Init(winProp);
}
glfw_window::~glfw_window(){
    Shutdown();
}

// This function initializes everything with the basic opengl setup. Not much to say here but there's some new kind of magic at the end of this function. New to me at least...
void glfw_window::Init(const WindowProperties& props){
    _data.title = props.name;
    _data.width = props.width;
    _data.height = props.height;

    // Basic setup. Not much to say here
    static bool glfwInitialized = false;
    if(!glfwInitialized){
        int init = glfwInit();
        if(!init){
            std::cout << "ERROR::GLFW_INIT_FAILED\n";
            return;
        }
        glfwSetErrorCallback([](int error, const char* description) {
            std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
        });
        glfwInitialized = true;
    }

    // Basic setup. Nothing much to say here
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // This creates the window
    _window = glfwCreateWindow((int)_data.width, (int)_data.height, _data.title.c_str(), nullptr, nullptr);
    _context = new OpenGLContext(_window);
    _context->Init();

    // This line allows me to use the window pointer later to check if the glfw window should close and to define the EventCallbackFn part of _data
    glfwSetWindowUserPointer(_window, &_data);
    // This is the window resize callback
    glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int w, int h){
        WinData& data = *(WinData*)glfwGetWindowUserPointer(window);
        data.width = w;
        data.height = h;

        WindowResizeEvent event(w, h);
        data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(_window, [](GLFWwindow* window){
        WinData& data = *(WinData*)glfwGetWindowUserPointer(window);

        WindowCloseEvent e;
        data.EventCallback(e);
    });
}

// This function just terminates the current running window
void glfw_window::Shutdown(){
    glfwDestroyWindow(_window);
    glfwTerminate();
}

// This function starts event polling and swaps buffers
void glfw_window::onUpdate(){
    glfwPollEvents();
    _context->SwapBuffers();
}
