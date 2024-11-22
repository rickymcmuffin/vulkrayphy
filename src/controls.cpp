
#include "../include/controls.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#define NUM_SHAPES 26
#define WHITE_BALL 10

#define TABLE_HEIGHT 1.009425f
#define TABLE_LENGTH 0.93f

#define BALL_RADIUS 0.0292657f
#define BALL_CIRCUMFRANCE 0.183788f

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

GameState::GameState()
    : camera(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW,
             PITCH)
{
    whiteBall = {};
    // whiteBall.pos = glm::vec3(-TABLE_LENGTH, TABLE_HEIGHT, 0.0f);
    whiteBall.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    whiteBall.velocity = glm::vec3(0.5f, 0.0f, 0.0f);
    whiteBall.rotation = glm::vec3(1.0f, 1.0f, 1.0f);
}

void GameState::updateGame(GLFWwindow *window, float deltaTime)
{

    processInput(window, deltaTime, &camera);
    updateBalls(deltaTime);
    printLogs();
}

void GameState::updateBalls(float deltaTime)
{
    Ball newBall = whiteBall;
    // newBall.pos += whiteBall.velocity * deltaTime;
    //
    // if (newBall.pos.x > TABLE_LENGTH)
    // {
    //     newBall.pos.x = TABLE_LENGTH - (whiteBall.pos.x - TABLE_LENGTH);
    //     newBall.velocity.x = -whiteBall.velocity.x;
    // }
    //
    // if (newBall.pos.x < -TABLE_LENGTH)
    // {
    //     newBall.pos.x = -TABLE_LENGTH + (-TABLE_LENGTH - whiteBall.pos.x);
    //     newBall.velocity.x = -whiteBall.velocity.x;
    // }
    //
    // auto delta_pos = newBall.pos - whiteBall.pos;
    //
    // newBall.rotation += delta_pos / BALL_CIRCUMFRANCE /1.0f;

    newBall.pos = glm::vec3(1.5f, 0.0f, 0.0f);
    newBall.pos =
        glm::rotate(glm::mat4(1.0f), 0.5f * (float) glfwGetTime() * glm::radians(90.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(newBall.pos, 1.0f);
    newBall.pos.y += 1.0f;

    whiteBall = newBall;
}

glm::mat4 GameState::getModelMatrix(size_t index)
{

    glm::mat4 ret = glm::mat4(1.0f);
    if (index == WHITE_BALL)
    {
        ret = glm::translate(ret, whiteBall.pos);
        ret = glm::rotate(ret, whiteBall.rotation.x * -glm::radians(180.0f),
                          glm::vec3(0.0f, 0.0f, 1.0f));
        ret = glm::rotate(ret, whiteBall.rotation.y * glm::radians(180.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f));
        ret = glm::rotate(ret, whiteBall.rotation.z * glm::radians(180.0f),
                          glm::vec3(1.0f, 0.0f, 0.0f));
        return ret;
    }
    ret = glm::rotate(ret, (float)glfwGetTime() * glm::radians(0.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f));
    ret = glm::translate(ret, glm::vec3(0.0f, 0.0f, 0.0f));

    return ret;
}

glm::vec3 GameState::getObjectPos(size_t index) { return whiteBall.pos; }

Camera GameState::getCamera() { return camera; }

void GameState::printLogs()
{
    frameCounter++;
    if (glfwGetTime() - last_time >= 1.0f)
    {
        std::cout << "\rFPS: " << frameCounter
                  << " | Camera Pos: " << glm::to_string(camera.Position)
                  << " | Ball Pos: " << glm::to_string(whiteBall.rotation)
                  << "                        ";
        frameCounter = 0;
        last_time = glfwGetTime();
    }
}
