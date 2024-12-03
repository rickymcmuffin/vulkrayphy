
#include "../include/controls.hpp"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <sys/types.h>

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#define NUM_SHAPES 26
#define WHITE_BALL 0
#define TEST_BALL 3

#define NUM_BALLS 16

#define BALLS_SHAPE_IND 10

#define TABLE_HEIGHT 1.009425f
#define TABLE_EAST_EDGE 0.93f
#define TABLE_WEST_EDGE -0.93f
#define TABLE_NORTH_EDGE 0.4375f
#define TABLE_SOUTH_EDGE -0.45f

#define BALL_RADIUS 0.0292657f
#define BALL_CIRCUMFRANCE 0.183788f

float randomFloat(float a, float b)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

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
    std::cout << "Started GameState constructor" << std::endl;

    float max_vel = 0.5f;

    for (size_t i = 0; i < NUM_BALLS; i++)
    {
        Ball new_ball = {};
        new_ball.pos = glm::vec3(
            randomFloat(TABLE_EAST_EDGE, TABLE_WEST_EDGE), TABLE_HEIGHT,
            randomFloat(TABLE_SOUTH_EDGE, TABLE_NORTH_EDGE));
        new_ball.velocity = glm::vec3(randomFloat(-max_vel, max_vel), 0.0f,
                                      randomFloat(-max_vel, max_vel));
        new_ball.rotation = glm::mat4(1.0f);

        // new_ball.pos = glm::vec3(0.0f, 0.0f, 0.0f);
        // new_ball.velocity = glm::vec3(0.0f);
        // new_ball.rotation = glm::mat4(1.0f);

        balls_all.push_back(new_ball);
    }

    // balls_all[3].pos = glm::vec3(0, TABLE_HEIGHT, 0);
    // balls_all[3].velocity = glm::vec3(0.1f, 0.0f, 0.1f);
    // balls_all[3].rotation = glm::mat4(1.0f);
    //
    // balls_all[WHITE_BALL + 1].pos =
    //     glm::vec3(TABLE_EAST_EDGE / 2, TABLE_HEIGHT, TABLE_SOUTH_EDGE);
    // balls_all[WHITE_BALL + 1].velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    // balls_all[WHITE_BALL + 1].rotation = glm::vec3(1.0f, 1.0f, 1.0f);
    //
    // balls_all[WHITE_BALL + 2].pos =
    //     glm::vec3(TABLE_EAST_EDGE, TABLE_HEIGHT, 0.0f);
    // balls_all[WHITE_BALL + 2].velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    // balls_all[WHITE_BALL + 2].rotation = glm::vec3(1.0f, 1.0f, 1.0f);
    //
    // balls_all[WHITE_BALL + 3].pos =
    //     glm::vec3(TABLE_WEST_EDGE, TABLE_HEIGHT, 0.0f);
    // balls_all[WHITE_BALL + 3].velocity = glm::vec3(0.1f, 0.0f, 0.1f);
    // balls_all[WHITE_BALL + 3].rotation = glm::vec3(1.0f, 1.0f, 1.0f);
    //
    // std::cout << , 1.0f, 1.0f"Finished GameState constructor" << std::endl;
}

void GameState::updateGame(GLFWwindow *window, float deltaTime)
{

    processInput(window, deltaTime, &camera);
    updateBalls(deltaTime);
    printLogs();
}

void GameState::updateBalls(float deltaTime)
{
    auto old_balls = balls_all;

    checkCollision();

    for (size_t i = 0; i < balls_all.size(); i++)
    {
        Ball newBall = balls_all[i];
        newBall.pos += balls_all[i].velocity * deltaTime;

        if (newBall.pos.x > TABLE_EAST_EDGE)
        {
            newBall.pos.x =
                TABLE_EAST_EDGE - (balls_all[i].pos.x - TABLE_EAST_EDGE);
            newBall.velocity.x = -balls_all[i].velocity.x;
        }

        else if (newBall.pos.x < TABLE_WEST_EDGE)
        {
            newBall.pos.x =
                TABLE_WEST_EDGE + (TABLE_WEST_EDGE - balls_all[i].pos.x);
            newBall.velocity.x = -balls_all[i].velocity.x;
        }

        if (newBall.pos.z > TABLE_NORTH_EDGE)
        {
            newBall.pos.z =
                TABLE_NORTH_EDGE - (balls_all[i].pos.z - TABLE_NORTH_EDGE);
            newBall.velocity.z = -balls_all[i].velocity.z;
        }

        else if (newBall.pos.z < TABLE_SOUTH_EDGE)
        {
            newBall.pos.z =
                TABLE_SOUTH_EDGE + (TABLE_SOUTH_EDGE - balls_all[i].pos.z);
            newBall.velocity.z = -balls_all[i].velocity.z;
        }

        auto delta_pos = newBall.velocity * deltaTime;

        float distance = glm::distance(newBall.pos, balls_all[i].pos);
        float angle = 3.14* distance / BALL_CIRCUMFRANCE;

        auto rotationAxis = glm::normalize(
            glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), newBall.velocity));

        // if (i == TEST_BALL)
            // std::cout << "rotation axis: " << glm::to_string(rotationAxis)<< std::endl;

        auto new_rotation = glm::rotate(glm::mat4(1.0f), angle, rotationAxis);
        newBall.rotation = new_rotation * newBall.rotation ;


        balls_all[i] = newBall;
    }
}

void GameState::checkCollision()
{
    for (size_t i = 0; i < balls_all.size(); i++)
    {
        balls_all[i].colliding = false;
        for (size_t j = 0; j < balls_all.size(); j++)
        {
            if (i == j)
                continue;

            float dist = glm::distance(balls_all[i].pos, balls_all[j].pos);
            if (dist <= BALL_RADIUS * 2)
            {
                balls_all[i].colliding = true;
                resolveCollision(&balls_all[i], &balls_all[j]);
            }
        }
    }
}

void GameState::resolveCollision(Ball *first, Ball *second)
{
    // std::cout << "resolving Collision!";
    glm::vec3 normal = glm::normalize(second->pos - first->pos);
    glm::vec3 relativeVelocity = second->velocity - first->velocity;

    glm::vec3 impulse = normal * (2 * glm::dot(relativeVelocity, normal) / 2);

    first->velocity += impulse;
    second->velocity -= impulse;

    // let repulsion = p5.Vector.mult(normal, minDistance - distance);
    float distance = glm::distance(first->pos, second->pos);
    float minDistance = BALL_RADIUS * 2;
    glm::vec3 repulsion = normal * (minDistance - distance);
    first->pos -= repulsion;
    second->pos += repulsion;
}

bool GameState::getUseColor(size_t index)
{
    return false;
    if (index < BALLS_SHAPE_IND)
    {
        return false;
    }

    return balls_all[index - BALLS_SHAPE_IND].colliding;
}

glm::mat4 GameState::getModelMatrix(size_t index)
{

    glm::mat4 ret = glm::mat4(1.0f);
    if (index >= BALLS_SHAPE_IND + WHITE_BALL)
    {
        size_t ball_ind = index - BALLS_SHAPE_IND;

        auto translate_mat =
            glm::translate(glm::mat4(1.0f), balls_all[ball_ind].pos);
        auto rotate_mat = balls_all[ball_ind].rotation;

        ret = translate_mat * balls_all[ball_ind].rotation * ret;
        // ret = balls_all[ball_ind].rotation * ret;
        return ret;
    }
    ret = glm::translate(ret, glm::vec3(0.0f, 0.0f, 0.0f));
    ret = glm::rotate(ret, (float)glfwGetTime() * glm::radians(0.0f),
                      glm::vec3(0.0f, 1.0f, 0.0f));

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
                  << "                        ";
        frameCounter = 0;
        last_time = glfwGetTime();
    }
}
