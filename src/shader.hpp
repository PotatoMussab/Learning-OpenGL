#ifndef SHADER
#define SHADER
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct
{
    const char* path;
    GLenum type;
    GLuint ID = 0;
} Shader;

class ShaderProgram
{
    public:
        GLuint ID;
        ShaderProgram(std::initializer_list<Shader*> shaderList)
        {
            ID = glCreateProgram();
            for(Shader *pShader:shaderList)
            {
                const char *shaderPath = pShader->path;
                GLenum shaderType = pShader->type;
                pShader->ID = createShaderFromFile(shaderType, &shaderPath);
                glAttachShader(ID, pShader->ID);
            }
            glLinkProgram(ID);
            //Check for errors
            int success;
            char infoLog[512];
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                std::cout << "ERROR LINKING SHADERS\n"
                        << infoLog << std::endl;
            }
            //Delete shaders
            for(Shader *pShader:shaderList)
                glDeleteShader(pShader->ID);
        }


    private:
        GLuint createShader(GLenum shaderType, const char *const *pSourceCode)
        {
            // Create vertex shader
            GLuint shader = glCreateShader(shaderType);
            glShaderSource(shader, 1, pSourceCode, NULL);
            glCompileShader(shader);
            // Check for compilation errors
            int success;
            char infoLog[512];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                std::cout << "ERROR COMPILING SHADER\n"
                        << infoLog << std::endl;
            }
            return shader;
        }
        GLuint createShaderFromFile(GLenum shaderType, const char *const *filePath)
        {
            GLuint shader = 0;
            std::ifstream file;
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                file.open(*filePath);
                std::stringstream fileStream;
                fileStream << file.rdbuf();
                file.close();
                std::string fileContent = fileStream.str();
                const char *sourceCode = fileContent.c_str();
                shader = createShader(shaderType, &sourceCode);
            }
            catch (std::ifstream::failure e)
            {
                std::cout << "ERROR READING SHADER FILE" << std::endl;
            }
            return shader;
        }
};
#endif