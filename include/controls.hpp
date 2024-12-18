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

    void setRimMesh(std::vector<glm::vec3> rim_mesh);
    void restartGame();

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

    std::vector<glm::vec3> rim_mesh;

    void printLogs();
    void updateBalls(float deltaTime);
    Ball applyForces(Ball ball, float delta_time);
    void resolveCollision(Ball *first, Ball *second);
    void resolveRimCollision(Ball *ball, std::vector<glm::vec3> triangle);
    

    void checkRimCollision();
    bool checkTriangleCollision(Ball ball, std::vector<glm::vec3> triangle);
    void checkCollision();

};

#endif
