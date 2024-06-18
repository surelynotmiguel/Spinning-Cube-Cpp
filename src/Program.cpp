#define STB_IMAGE_IMPLEMENTATION

#define ICON_PATH "src/images/icons/icon.jpg"
#define TEXTURE_PATH "src/images/textures/idfw.png"
#define DEFAULT_FONT "src/fonts/JAi_____.TTF"

#define V_SHADER "src/shaders/vs.vert"
#define F_SHADER "src/shaders/fs.frag"

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <windows.h>

#include "Program.hpp"
#include "Global.hpp"
#include "ShaderUtils.hpp"

Program::Program(){
    this->cubeVertices = {
        {-0.5f, -0.5f, 0.5f}, 
        {-0.5f, 0.5f, 0.5f}, 
        {0.5f, 0.5f, 0.5f},  
        {0.5f, -0.5f, 0.5f},  
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, 0.5f, -0.5f}, 
        {0.5f, 0.5f, -0.5f},  
        {0.5f, -0.5f, -0.5f}
    };

    this->textureCoords = {
        1, 0, 0, 0, 1, 1/6,
        0, 0, 0, 1/6, 1, 1/6,

        1, 1/6, 0, 1/6, 1, 2/6,
        0, 1/6, 0, 2/6, 1, 2/6,

        1, 2/6, 0, 2/6, 1, 3/6,
        0, 2/6, 0, 3/6, 1, 3/6,

        1, 3/6, 0, 3/6, 1, 4/6,
        0, 3/6, 0, 4/6, 1, 4/6,

        1, 4/6, 0, 4/6, 1, 5/6,
        0, 4/6, 0, 5/6, 1, 5/6,

        1, 5/6, 0, 5/6, 1, 6/6,
        0, 5/6, 0, 6/6, 1, 6/6,
    };

    this->theta = {1.0f, 2.0f, 3.0f};
}

Program::~Program() = default;

void Program::makeFace(int v1, int v2, int v3, int v4){
    vector<vector<float>> triangles = {
        cubeVertices[v1], cubeVertices[v2], 
        cubeVertices[v4], cubeVertices[v2], 
        cubeVertices[v3], cubeVertices[v4]
    };

    for(int i = 0; i < triangles.size(); i++){
        for(int j = 0; j < 3; j++){
            this->vertices.push_back(triangles[i][j]);
        }
    }
}

void Program::drawShape(){
    GLenum err = glewInit();
    if(GLEW_OK != err){
        Global::showMessage(glewGetErrorString(err));
        exit(-1);
    }

    makeFace(0, 3, 2, 1); // Front face
    makeFace(2, 3, 7, 6); // Back face
    makeFace(3, 0, 4, 7); // Top face
    makeFace(1, 2, 6, 5); // Bottom face
    makeFace(4, 5, 6, 7); // Right face
    makeFace(5, 4, 0, 1); // Left face

    if(!shaderUtils.Load(V_SHADER, F_SHADER)){
        exit(-1);
    }

    GLuint vao, vbo, tbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(GLfloat), textureCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, nrChannels;
    uint8_t* data = stbi_load(TEXTURE_PATH, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, (nrChannels < 4 ? GL_RGB32F : GL_RGBA32F), width, height, 0, (nrChannels < 4 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        Global::showMessage("Failed to load texture");
        exit(-1);
    }
    stbi_image_free(data);
    
    shaderUtils.Use();

    GLint thetaLocation = glGetUniformLocation(shaderUtils.getId(), "theta");
    if(thetaLocation == -1){
        Global::showMessage("Could not find uniform 'theta'");
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(this->window)){
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniform3fv(thetaLocation, 1, &this->theta[0]);
        glBindTextureUnit(GL_TEXTURE0, texture);
        glUniform1i(glGetUniformLocation(shaderUtils.getId(), "uSampler"), 0);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

        glfwSwapBuffers(this->window);
        glfwPollEvents();

        this->theta[0] += 0.5f;
        this->theta[1] += 0.5f;
        this->theta[2] += 0.5f;
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &tbo);
    glDeleteVertexArrays(1, &vao);

    shaderUtils.Delete();
}

/*
    @author mattdinthehouse <a href="">https://github.com/glfw/glfw/issues/310#issuecomment-52048508</a>
*/
void Program::glfwSetWindowCenter(GLFWwindow* window) {
    // Get window position and size
    int window_x, window_y;
    glfwGetWindowPos(window, &window_x, &window_y);

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);

    // Halve the window size and use it to adjust the window position to the center of the window
    window_width *= 0.5;
    window_height *= 0.5;

    window_x += window_width;
    window_y += window_height;

    // Get the list of monitors
    int monitors_length;
    GLFWmonitor **monitors = glfwGetMonitors(&monitors_length);

    if(monitors == NULL) {
        // Got no monitors back
        return;
    }

    // Figure out which monitor the window is in
    GLFWmonitor *owner = NULL;
    int owner_x, owner_y, owner_width, owner_height;

    for(int i = 0; i < monitors_length; i++) {
        // Get the monitor position
        int monitor_x, monitor_y;
        glfwGetMonitorPos(monitors[i], &monitor_x, &monitor_y);

        // Get the monitor size from its video mode
        int monitor_width, monitor_height;
        GLFWvidmode *monitor_vidmode = (GLFWvidmode*) glfwGetVideoMode(monitors[i]);

        if(monitor_vidmode == NULL) {
            // Video mode is required for width and height, so skip this monitor
            continue;

        } else {
            monitor_width = monitor_vidmode->width;
            monitor_height = monitor_vidmode->height;
        }

        // Set the owner to this monitor if the center of the window is within its bounding box
        if((window_x > monitor_x && window_x < (monitor_x + monitor_width)) && (window_y > monitor_y && window_y < (monitor_y + monitor_height))) {
            owner = monitors[i];

            owner_x = monitor_x;
            owner_y = monitor_y;

            owner_width = monitor_width;
            owner_height = monitor_height;
        }
    }

    if(owner != NULL) {
        // Set the window position to the center of the owner monitor
        glfwSetWindowPos(window, owner_x + (owner_width * 0.5) - window_width, owner_y + (owner_height * 0.5) - window_height);
    }
}

void Program::setWindow(){
    if(!glfwInit()){ 
        Global::showMessage("Unable to initialize GLFW");
        exit(-1);
    }

    std::string glewV = (const char*)glewGetString(GLEW_VERSION);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    this->window = glfwCreateWindow(800, 800, ("Shape - GLEW " + glewV).c_str(), NULL, NULL);
    if(!this->window){
        Global::showMessage("GLFW was unable to create window");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(this->window);
    glfwSetWindowCenter(this->window);

    int width, height, channels;
    uint8_t* pixels = stbi_load(ICON_PATH, &width, &height, &channels, 4);
    if(!pixels){
        Global::showMessage("Failed to load icon");
        glfwTerminate();
        exit(-1);
    }

    GLFWimage images[1];
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = pixels;
    glfwSetWindowIcon(this->window, 1, images);
    stbi_image_free(pixels);
}

void Program::start(){ 
    setWindow();
    drawShape();
    glfwTerminate();
}


