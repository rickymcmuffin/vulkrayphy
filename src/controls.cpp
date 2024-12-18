
#include "../include/controls.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <iterator>
#include <sys/types.h>

#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#define NUM_SHAPES 26
#define WHITE_BALL 0
#define TEST_BALL 3

#define NUM_BALLS 16

#define BALLS_SHAPE_IND 10

#define FRICTION_COF 0.015f
#define BALL_MASS 0.17f

#define BALL_BALL_REST 0.95f
#define BALL_RAIL_REST 0.8f
// #define BALL_BALL_REST 1.0f
// #define BALL_RAIL_REST 1.0f

#define TABLE_HEIGHT 1.009425f
#define TABLE_EAST_EDGE 0.93f
#define TABLE_WEST_EDGE -0.93f
#define TABLE_NORTH_EDGE 0.44f
#define TABLE_SOUTH_EDGE -0.459f

#define STOP_VELOCITY 0.001f

#define BALL_RADIUS 0.0292657f
#define BALL_CIRCUMFRANCE 0.183788f

float randomFloat(float a, float b)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void processInput(GLFWwindow *window, float deltaTime, Camera *camera,
                  glm::vec3 *ball_pos)
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

    float ball_speed = 0.2f;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        ball_pos->z -= ball_speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        ball_pos->z += ball_speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        ball_pos->x -= ball_speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        ball_pos->x += ball_speed * deltaTime;
    }


    // bool should_restart = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;

    // return should_restart;
}


float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

GameState::GameState()
    : camera(glm::vec3(0.0f, 2.5f, 0.1f), glm::vec3(0.0f, 1.0f, 0.0f), YAW,
             PITCH)
{
    restartGame();
}


void GameState::restartGame(){
    std::cout << "Started GameState constructor" << std::endl;

    balls_all.erase(balls_all.begin(), balls_all.end());

    float max_vel = 1.0f;

    for (size_t i = 0; i < NUM_BALLS; i++)
    {
        Ball new_ball = {};
        new_ball.pos = glm::vec3(
            randomFloat(TABLE_EAST_EDGE, TABLE_WEST_EDGE), TABLE_HEIGHT,
            randomFloat(TABLE_SOUTH_EDGE, TABLE_NORTH_EDGE));
        new_ball.velocity = glm::vec3(randomFloat(-max_vel, max_vel), 0.0f,
                                      randomFloat(-max_vel, max_vel));
        new_ball.rotation = glm::mat4(1.0f);


        balls_all.push_back(new_ball);
    }
}

void GameState::updateGame(GLFWwindow *window, float deltaTime)
{
    processInput(window, deltaTime, &camera, &balls_all[TEST_BALL].pos);
    updateBalls(deltaTime);
    printLogs();
}

GameState::Ball GameState::applyForces(Ball ball, float delta_time)
{
    const float friction = FRICTION_COF * BALL_MASS * 9.81;
    const float vel_len = glm::length(ball.velocity) - friction * delta_time;

    if (glm::length(ball.velocity) >= STOP_VELOCITY)
    {
        ball.velocity = glm::normalize(ball.velocity) * vel_len;
    }
    return ball;
}

void GameState::updateBalls(float deltaTime)
{
    auto old_balls = balls_all;

    checkCollision();

    checkRimCollision();

    for (size_t i = 0; i < balls_all.size(); i++)
    {
        Ball newBall = balls_all[i];
        newBall = applyForces(newBall, deltaTime);
        newBall.pos += balls_all[i].velocity * deltaTime;

        if (glm::length(newBall.velocity) < STOP_VELOCITY)
        {
            newBall.velocity = glm::vec3(0.0f);
        }

        float distance = glm::distance(newBall.pos, old_balls[i].pos);
        if (glm::length(newBall.velocity) >= STOP_VELOCITY)
        {
            float angle = 3.14 * distance / BALL_CIRCUMFRANCE;

            auto rotationAxis = glm::normalize(
                glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), newBall.velocity));

            // if (i == TEST_BALL)
            // std::cout << "rotation axis: " << angle
            //           << std::endl;

            auto new_rotation =
                glm::rotate(glm::mat4(1.0f), angle, rotationAxis);
            newBall.rotation = new_rotation * newBall.rotation;
        }

        balls_all[i] = newBall;
    }
}

bool GameState::checkTriangleCollision(Ball ball,
                                       std::vector<glm::vec3> triangle)
{
    // return false;
    // std::cout << "triangle: " << std::endl;
    for (auto i : triangle)
    {
        // std::cout << glm::to_string(i) << std::endl;
    }
    glm::vec2 ball_pos = glm::vec2(ball.pos.x, ball.pos.z);
    std::vector<glm::vec2> triangle2d;
    for (size_t i = 0; i < triangle.size(); i++)
    {
        glm::vec2 new_vert = glm::vec2(triangle[i].x, triangle[i].z);
        triangle2d.push_back(new_vert);
    }

    // Vertex within Circle
    for (size_t i = 0; i < triangle2d.size(); i++)
    {
        float dist = glm::distance(ball_pos, triangle2d[i]);
        if (dist <= BALL_RADIUS)
        {
            // std::cout << "test1" << std::endl;
            return true;
        }
    }

    // Circle within Triangle
    bool pointIsInTriangle =
        PointInTriangle(ball_pos, triangle2d[0], triangle2d[1], triangle2d[2]);
    if (pointIsInTriangle)
    {
        // std::cout << "test2" << std::endl;
        return true;
    }

    // Circle intersects Edge
    auto c1 = ball_pos - triangle2d[0];
    auto e1 = triangle2d[1] - triangle2d[0];

    auto k = glm::dot(c1, e1);

    if (k > 0)
    {
        float len = glm::length(e1);
        k = k / len;

        if (k < len && sqrt(c1.x * c1.x + c1.y * c1.y - k * k) <= BALL_RADIUS)
        {
            return true;
        }
    }

    auto c2 = ball_pos - triangle2d[1];
    auto e2 = triangle2d[2] - triangle2d[1];

    k = glm::dot(c2, e2);

    if (k > 0)
    {
        float len = glm::length(e2);
        k = k / len;

        if (k < len && sqrt(c2.x * c2.x + c2.y * c2.y - k * k) <= BALL_RADIUS)
        {
            return true;
        }
    }

    auto c3 = ball_pos - triangle2d[2];
    auto e3 = triangle2d[0] - triangle2d[2];

    k = glm::dot(c3, e3);

    if (k > 0)
    {
        float len = glm::length(e3);
        k = k / len;

        if (k < len && sqrt(c3.x * c3.x + c3.y * c3.y - k * k) <= BALL_RADIUS)
        {
            return true;
        }
    }

    return false;
}
void GameState::checkRimCollision()
{
    for (size_t i = 0; i < balls_all.size(); i++)
    {
        balls_all[i].colliding = false;
        for (size_t j = 0; j < rim_mesh.size() - 3; j += 3)
        {
            auto triangle =
                std::vector(rim_mesh.begin() + j, rim_mesh.begin() + j + 3);

            glm::vec3 triangle_normal = glm::cross(triangle[0] - triangle[1],
                                                   triangle[2] - triangle[0]);

            triangle_normal = glm::normalize(triangle_normal);
            triangle_normal.y = 0;

            if (glm::length(triangle_normal) < 0.1f)
            {
                continue;
            }
            if (checkTriangleCollision(balls_all[i], triangle))
            {
                // std::cout << "colliding!";
                // balls_all[i].colliding = true;
                resolveRimCollision(&balls_all[i], triangle);
            }
        }
    }
}
void GameState::resolveRimCollision(Ball *ball, std::vector<glm::vec3> triangle)
{
    glm::vec3 triangle_normal =
        glm::cross(triangle[0] - triangle[1], triangle[2] - triangle[0]);

    triangle_normal = glm::normalize(triangle_normal);

    triangle_normal.y = 0;


    glm::vec3 relativeVelocity = triangle_normal - ball->velocity;

    glm::vec3 impulse =
        BALL_RAIL_REST* triangle_normal * (2* glm::dot(ball->velocity, triangle_normal));

    impulse.y = 0;

    ball->velocity -= impulse;

    float minDistance = BALL_RADIUS;
    float distance = 0.03f;

    glm::vec3 repulsion = triangle_normal * (minDistance - distance);

    ball->pos += repulsion;
}

void GameState::checkCollision()
{
    for (size_t i = 0; i < balls_all.size(); i++)
    {
        for (size_t j = 0; j < balls_all.size(); j++)
        {
            if (i == j)
                continue;

            float dist = glm::distance(balls_all[i].pos, balls_all[j].pos);
            if (dist <= BALL_RADIUS * 2)
            {
                resolveCollision(&balls_all[i], &balls_all[j]);
            }
        }
    }
}

void GameState::resolveCollision(Ball *first, Ball *second)
{
    glm::vec3 normal = glm::normalize(second->pos - first->pos);
    glm::vec3 relativeVelocity = second->velocity - first->velocity;

    glm::vec3 impulse =
        BALL_BALL_REST * normal * (2 * glm::dot(relativeVelocity, normal) / 2);

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

        ret = translate_mat * rotate_mat * ret;
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

void GameState::setRimMesh(std::vector<glm::vec3> rim_mesh)
{
    this->rim_mesh = rim_mesh;
}

void GameState::printLogs()
{
    frameCounter++;
    if (glfwGetTime() - last_time >= 1.0f)
    {
        std::cout << "\rFPS: " << frameCounter
                  << " | Camera Pos: " << glm::to_string(camera.Position)
                  << " | Ball Pos: " << glm::to_string(balls_all[0].velocity)
                  << "                        ";
        frameCounter = 0;
        last_time = glfwGetTime();
    }
}
