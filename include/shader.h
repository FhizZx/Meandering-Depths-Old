#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
public:
    //program ID
    unsigned int ID;

    //constructor - reads files and builds shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

    //use shader
    void use();

    //utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVectorFloat(const std::string &name, float x, float y) const;
    void setVectorFloat(const std::string &name, float x, float y, float z)const;
    void setVectorFloat(const std::string &name, float x, float y, float z, float w)const;
    void setMatrix(const std::string &name, glm::mat4 matrix)const;
};


#endif // SHADER_H
