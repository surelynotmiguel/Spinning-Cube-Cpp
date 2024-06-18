#ifndef PROGRAM_CLASS_HPP
#define PROGRAM_CLASS_HPP

#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "ShaderUtils.hpp"

using namespace std;

class Program {
private:
    GLFWwindow* window;
    ShaderUtils shaderUtils;

    vector<GLfloat> vertices;
    vector<vector<GLfloat>> cubeVertices;
    vector<GLfloat> textureCoords;
    vector<GLfloat> theta;
    
public:
    Program();
    ~Program();

    void start();

    void setWindow();
    void glfwSetWindowCenter(GLFWwindow*);
    void makeFace(int, int, int, int);
    void drawShape();
};

#endif
