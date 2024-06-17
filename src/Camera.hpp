#ifndef CAMERA_CLASS_HPP
#define CAMERA_CLASS_HPP

#define GLM_ENABLE_EXPERIMENTAL

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "ShaderUtils.hpp"

class Camera {
public:
    typedef struct {
        GLFWwindow* window;
        int number;
        int closeable;
    } Slot;

    glm::vec3 position;
    glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec2 yaw = glm::vec2(0.0f, 0.0f);
    glm::vec2 pitch = glm::vec2(0.0f, 0.0f);

    int width;
    int height;

    float speed = 0.1f;
    float sensitivity = 100.0f;

    Camera(int width, int height, glm::vec3 position);

    void Matrix(float POVdeg, float near, float far, ShaderUtils& shader, const char* uniform);
    void Inputs(GLFWwindow* window);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
};

#endif 