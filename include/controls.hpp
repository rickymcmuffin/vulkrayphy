#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <cstdint>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "camera.hpp"

class GameState
{

  public:
    GameState();
    void updateGame(GLFWwindow *window, float deltaTime);
    Camera getCamera();
    glm::mat4 getModelMatrix(size_t index);
    glm::vec3 getObjectPos(size_t index);

    bool getUseColor(size_t index);

  private:
    struct Ball
    {
        glm::vec3 pos;
        glm::vec3 velocity;
        glm::mat4 rotation;
        bool colliding;
    };

    Camera camera;

    uint64_t frameCounter;
    int last_time = glfwGetTime();

    Ball whiteBall;

    std::vector<Ball> balls_all;

    void printLogs();
    void updateBalls(float deltaTime);
    void resolveCollision(Ball *first, Ball *second);
    

    void checkCollision();
};

#endif
