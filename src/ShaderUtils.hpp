#ifndef SHADER_UTILS_HPP
#define SHADER_UTILS_HPP

#include <string>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderUtils {
public:
    unsigned int getId();
    bool Load(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
    void Use();
    void Delete();
    
private:
    unsigned int ID;
    unsigned int GetCompiledShader(unsigned int shader_type, const std::string& shader_source);
};

#endif
