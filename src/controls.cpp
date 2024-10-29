
#include "../include/controls.hpp"
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

void processInput(GLFWwindow *window, float deltaTime, Camera *camera)
{
    // float speed = 1.0;
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        deltaTime *= 2;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(DOWN, deltaTime);
    }

    float xoffset = 0.0f;
    float yoffset = 0.0f;
    float offsetAmount = 500.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        yoffset += offsetAmount;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        yoffset -= offsetAmount;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        xoffset += offsetAmount;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        xoffset -= offsetAmount;
    }
    camera->ProcessMouseMovement(xoffset, yoffset);
}

GameState::GameState(): camera(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH){
}

void GameState::updateGame(GLFWwindow *window, float deltaTime)
{

    processInput(window, deltaTime, &camera);
    printFPS();
}

Camera GameState::getCamera() { return camera; }

void GameState::printFPS()
{
    frameCounter++;
    if (glfwGetTime() - last_time >= 1.0f)
    {
        std::cout << "\rFPS: " << frameCounter;//<<std::endl;
        // std::cout << glm::to_string(camera.Position) <<std::endl;
        frameCounter = 0;
        last_time = glfwGetTime();
    }
}
