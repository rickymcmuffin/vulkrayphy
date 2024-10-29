#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <cstdint>
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "camera.hpp"


class GameState
{

  public:
    GameState();
    void updateGame(GLFWwindow *window, float deltaTime);
    Camera getCamera();

  private:
    Camera camera;

    uint64_t frameCounter;
    int last_time = glfwGetTime();

    void printFPS();
};

#endif
