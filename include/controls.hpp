#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "camera.hpp"

namespace DoThings
{


class Controls
{
  public:
    Camera updateCamera(GLFWwindow *window, float deltaTime, Camera camera);
};
} // namespace DoThings

#endif
