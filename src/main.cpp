#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
//--------------------------------Typedefs-----------------------------------------------
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef struct{
    int height = 0;
    int width = 0;
    int nrChannels = 0;
    unsigned char *data = 0;
} image;
//--------------------------------Settings-----------------------------------------------
const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;
const char *WIN_TITLE = "Learning OpenGL";
const char *VERTEX_SHADER_PATH = "..\\shaders\\shader.vs";
const char *FRAGMENT_SHADER_PATH = "..\\shaders\\shader.fs";
//---------------------------Prototype functions-----------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void keyPress_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
GLuint createShader(GLenum shaderType, const char *const *pSourceCode); // Pass pointer to a string
GLuint createShaderFromFile(GLenum shaderType, const char *const *filePath);
//--------------------------------Main---------------------------------------------------
int main()
{
    if (!glfwInit()) // Initialize GLFW
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set version to OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set to core profile

    GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL); // Create window
    if (window == NULL)                                                                  // In case window doesn't create
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Set the context to the created window

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // Load GLAD (needs an existing OpenGL context beforehand)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);                                        // Create a viewport for the window. Maps from (-1,1) to (0, max width/height)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Set callback for resizing window
    glfwSetKeyCallback(window, keyPress_callback);                     // Set callback for key press

    float triangleVertices[] = {
        //position             //texture coords  
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f,// top left
         0.5f,  0.5f, 0.0f,    1.0f, 1.0f // top right
    };

    unsigned int indices[] = {
        0, 1, 2, // bottom right triangle
        1, 2, 3  // bottom left triangle
    };
    Shader vShader = {VERTEX_SHADER_PATH, GL_VERTEX_SHADER, 0};
    Shader fShader = {FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER, 0}; 
    ShaderProgram shaderProgram({&vShader, &fShader});
    shaderProgram.use();

    GLuint VBO, VAO, EBO; //Generate VBO, VAO and EBO
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO); //Bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW); //Load vertex data into VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //Load indices into EBO
    //Create attribute pointers for VAO and enable them
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0); //First 3 floats are position coords XYZ
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float))); //Next 2 values are texture coords ST
    glEnableVertexAttribArray(0); //Enable attribute pointers
    glEnableVertexAttribArray(1);

    //Apply texture to the drawn shape
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../textures/bookshelf.jpg", &width, &height, &nrChannels, 0); //Load image

    GLuint texture;
    glGenTextures(1,&texture); //Generate texture
    glActiveTexture(GL_TEXTURE0); //Set active texture to 0
    glBindTexture(GL_TEXTURE_2D, texture); //Bind generated texture
    // Set texture wrapping/filtering optionss
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(data)
    {
        if(nrChannels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //Load texture data into buffer
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //Generate Mipmaps automatically
    }
    else
    {
        std::cout << "FAILED TO LOAD IMAGE" << std::endl;
    }
    stbi_image_free(data); //Free image data
    //Apply transformations to the data
    glm::vec3 axis(0.0f, 1.0f, 0.0f);
    float rotDegrees = glm::radians(90.0 * glfwGetTime());
    glm::mat4 transformMat = glm::rotate(glm::mat4(1.0f), rotDegrees, axis);
    GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformMat));

    glUniform1i(glGetUniformLocation(shaderProgram.ID, "texSampler"), 0); //Set the texture sampler to read from GL_TEXTURE0 (usually 0 by default)
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f); // Set color (RGBA color scheme) for clearing window

    while (!glfwWindowShouldClose(window)) // While window shouldn't close
    {
        glClear(GL_COLOR_BUFFER_BIT); // Clear color buffer before rendering next frame

        rotDegrees = glm::radians(90.0 * glfwGetTime());
        transformMat = glm::rotate(glm::mat4(1.0f), rotDegrees, axis);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformMat));

        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window); // Swap color buffers (render next frame)
        glfwPollEvents();        // Check for events
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);
    glDeleteProgram(shaderProgram.ID);

    glfwTerminate(); // Close window
    return 0;
}

//------------------------------------------Functions-----------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }
void keyPress_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}