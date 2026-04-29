#ifndef SHADER
#define SHADER
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        void use()
            {glUseProgram(ID);}

        void setBool(const char *name, int value)
            {glUniform1i(glGetUniformLocation(ID, name), value);}
        void setInt(const char *name, int value)
            {glUniform1i(glGetUniformLocation(ID, name), value);}
        void setFloat(const char *name, float value)
            {glUniform1f(glGetUniformLocation(ID, name), value);}
        void setVec2(const char *name, const glm::vec2 &value)
            {glUniform2fv(glGetUniformLocation(ID, name), 1, &value[0]);}
        void setVec2(const char *name, float x, float y)
            {glUniform2f(glGetUniformLocation(ID, name), x, y);}
        void setVec3(const char *name, const glm::vec3 &value)
            {glUniform3fv(glGetUniformLocation(ID, name), 1, &value[0]);}
        void setVec3(const char *name, float x, float y, float z)
            {glUniform3f(glGetUniformLocation(ID, name), x, y, z);}
        void setVec4(const char *name, const glm::vec4 &value)
            {glUniform4fv(glGetUniformLocation(ID, name), 1, &value[0]);}
        void setVec4(const char *name, float x, float y, float z, float w) 
            {glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);}
        void setMat2(const char *name, const glm::mat2 &mat)
            {glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);}
        void setMat3(const char *name, const glm::mat3 &mat)
            {glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);}
        void setMat4(const char *name, const glm::mat4 &mat)
            {glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);}

        void setBool(GLint location, int value)
            {glUniform1i(location, value);}
        void setInt(GLint location, int value)
            {glUniform1i(location, value);}
        void setFloat(GLint location, float value)
            {glUniform1f(location, value);}
        void setVec2(GLint location, const glm::vec2 &value)
            {glUniform2fv(location, 1, &value[0]);}
        void setVec2(GLint location, float x, float y)
            {glUniform2f(location, x, y);}
        void setVec3(GLint location, const glm::vec3 &value)
            {glUniform3fv(location, 1, &value[0]);}
        void setVec3(GLint location, float x, float y, float z)
            {glUniform3f(location, x, y, z);}
        void setVec4(GLint location, const glm::vec4 &value)
            {glUniform4fv(location, 1, &value[0]);}
        void setVec4(GLint location, float x, float y, float z, float w) 
            {glUniform4f(location, x, y, z, w);}
        void setMat2(GLint location, const glm::mat2 &mat)
            {glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);}
        void setMat3(GLint location, const glm::mat3 &mat)
            {glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);}
        void setMat4(GLint location, const glm::mat4 &mat)
            {glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);}

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