#include <GL/glew.h>

#include "Camera.hpp"


Camera::Camera(int width, int height, glm::vec3 position){
    this->width = width;
    this->height = height;
    this->position = position;
}

void Camera::Matrix(float POVdeg, float near, float far, ShaderUtils& shader, const char* uniform){
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(this->position, this->position + this->orientation, this->up);
    projection = glm::perspective(glm::radians(POVdeg), (float)(this->width / this->height), near, far);

    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), uniform), 1, GL_FALSE, glm::value_ptr(projection * view)); 
}

void Camera::Inputs(GLFWwindow* window){
    //Camera Position
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        this->position += this->speed * this->orientation;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        this->position += this->speed * -glm::normalize(glm::cross(this->orientation, this->up));
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        this->position += this->speed * -(this->orientation);
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        this->position += this->speed * glm::normalize(glm::cross(this->orientation, this->up));
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        this->position += this->speed * this->up;
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        this->position += this->speed * -(this->up);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        this->speed = 0.4f;
    }

    else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE){
        this->speed = 0.1f;
    }
}

void Camera::cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    //TODO
}