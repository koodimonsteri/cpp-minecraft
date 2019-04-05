#include <iostream>
#include "game/window.hpp"
#include "game/event/key_type.hpp"
#include "game/event/event_util.hpp"
#include "util/log.hpp"

GLuint WIDTH = 800, HEIGHT = 600;


void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static bool keysPressed[NUM_KEY_TYPES] = {false};

    // PRESS
    if (key == GLFW_KEY_W && action == GLFW_PRESS) keysPressed[KEY_W] = true;
    if (key == GLFW_KEY_A && action == GLFW_PRESS) keysPressed[KEY_A] = true;
    if (key == GLFW_KEY_S && action == GLFW_PRESS) keysPressed[KEY_S] = true;
    if (key == GLFW_KEY_D && action == GLFW_PRESS) keysPressed[KEY_D] = true;
    if (key == GLFW_KEY_U && action == GLFW_PRESS) keysPressed[KEY_U] = true;
    if (key == GLFW_KEY_F && action == GLFW_PRESS) keysPressed[KEY_F] = true;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) keysPressed[KEY_SPACE] = true;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) keysPressed[KEY_LEFT_SHIFT] = true;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  keysPressed[KEY_ESCAPE] = true;

    // RELEASE
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) keysPressed[KEY_W] = false;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) keysPressed[KEY_A] = false;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) keysPressed[KEY_S] = false;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) keysPressed[KEY_D] = false;
    if (key == GLFW_KEY_U && action == GLFW_RELEASE) keysPressed[KEY_U] = false;
    if (key == GLFW_KEY_F && action == GLFW_RELEASE) keysPressed[KEY_F] = false;
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) keysPressed[KEY_SPACE] = false;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) keysPressed[KEY_LEFT_SHIFT] = false;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) keysPressed[KEY_ESCAPE] = false;

    Event e(EventType::E_KEY, keysPressed);
    dispatch(e);
}

void glfwMouseCallback(GLFWwindow* window, double xpos1, double ypos1) {
    static double xpos0 = xpos1;
    static double ypos0 = ypos1;

    float dx = xpos1 - xpos0;
    float dy = ypos1 - ypos0;

    xpos0 = xpos1;
    ypos0 = ypos1;

    Event e(EventType::E_MOUSE, dx, dy);
    dispatch(e);
}

void glfwWindowMaximizeCallback(GLFWwindow* window, int maximized)
{
    GameConfig& gc = *(GameConfig*)glfwGetWindowUserPointer(window);
    const GLFWvidmode* vmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    gc.windowWidth = vmode->width;
    gc.windowHeight = vmode->height;
    glViewport(0, 0, gc.windowWidth, gc.windowHeight);
}

void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    GameConfig& gc = *(GameConfig*)glfwGetWindowUserPointer(window);
    gc.windowWidth = width;
    gc.windowHeight = height;
    glViewport(0, 0, gc.windowWidth, gc.windowHeight);
}

void Window::init() {
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac

    windowHandle_ = glfwCreateWindow(gameConfig.windowWidth, gameConfig.windowHeight, "Minecraft", NULL, NULL);
    glfwMakeContextCurrent(windowHandle_);
    glfwSetWindowUserPointer(windowHandle_, &gameConfig);

    monitorHandle_ = glfwGetPrimaryMonitor();

    if (windowHandle_ == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwGetWindowPos(windowHandle_, &windowPos[0], &windowPos[1]);    // Get window position
    glfwGetWindowSize(windowHandle_, &windowSize[0], &windowSize[1]); // Get window size

    if(cursorEnabled){
        glfwSetInputMode(windowHandle_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(windowHandle_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    setVSync(gameConfig.vSync);
    
    glfwSetWindowSizeCallback(windowHandle_, glfwWindowSizeCallback);
    glfwSetWindowMaximizeCallback(windowHandle_, glfwWindowMaximizeCallback);

    glfwSetWindowCloseCallback(windowHandle_, [](GLFWwindow* window)
    {
        Event e(EventType::APPLICATION_CLOSE);
        dispatch(e);
    });

    glfwSetKeyCallback(windowHandle_, glfwKeyCallback);
    glfwSetCursorPosCallback(windowHandle_, glfwMouseCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize OpenGL context");
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_3D);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Define the viewport dimensions
    glViewport(0, 0, gameConfig.windowWidth, gameConfig.windowHeight);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(windowHandle_);
}

void Window::setVSync(bool vsync)
{
    if(vsync)
    {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}

void Window::toggleFullScreen()
{
    if(!gameConfig.isFullScreen)
    {
        glfwGetWindowPos(windowHandle_, &windowPos[0], &windowPos[1]);
        glfwGetWindowSize(windowHandle_, &windowSize[0], &windowSize[1]);

        const GLFWvidmode* vmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(windowHandle_, monitorHandle_, 0, 0, vmode->width, vmode->height, 0);
        gameConfig.windowWidth = vmode->width;
        gameConfig.windowHeight = vmode->height;
        gameConfig.isFullScreen = true;
    } else {
        glfwSetWindowMonitor(windowHandle_, nullptr,  windowPos[0], windowPos[1], windowSize[0], windowSize[1], 0);
        gameConfig.windowWidth = windowSize[0];
        gameConfig.windowHeight = windowSize[1];
        gameConfig.isFullScreen = false;
    }
    glViewport(0, 0, gameConfig.windowWidth, gameConfig.windowHeight);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

void Window::toggleCursor()
{
    if(cursorEnabled)
    {
        cursorEnabled = false;
        glfwSetInputMode(windowHandle_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        cursorEnabled = true;
        glfwSetInputMode(windowHandle_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Window::swapBuffers() const {
    glfwSwapBuffers(windowHandle_);
}

Window::~Window() {
    glfwTerminate();
}