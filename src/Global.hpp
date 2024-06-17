#ifndef GLOBAL_CLASS_HPP
#define GLOBAL_CLASS_HPP

#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include <windows.h>

class Global { 
public:
    static void showMessage(std::string);
    static void showMessage(const GLubyte*);
};

#endif