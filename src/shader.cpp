#include <shader.h>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream fShaderFile;
    std::ifstream vShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e) {
        std::cout << "ERROR SHADER FILE NOT SUCCESFULLY READ\n";
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR VERTEX SHADER COMPILATION FAILED\n" << infoLog << "\n";
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << "\n";
    }


    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR SHADER PROGRAM LINKING FAILED\n" << infoLog << "\n";
    }
    glDeleteShader(fragment);
    glDeleteShader(vertex);
}

void Shader::use(){
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVectorFloat(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
void Shader::setVectorFloat(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::setVectorFloat(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}


void Shader::setMatrix(const std::string &name, glm::mat4 matrix)const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
