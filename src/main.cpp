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
#include "camera.hpp"
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
//-------------------------------Global Variables----------------------------------------
Camera cam = Camera();
float frameStart = 0.0f;
float dt = 0.0f;
//---------------------------Prototype functions-----------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void keyPress_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void checkForKeyPress(GLFWwindow *window);
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
        // Front face
        -0.5f, -0.5f, -0.1f,  0.0f, 0.0f, // 0 (front-bot-left)
         0.5f, -0.5f, -0.1f,  1.0f, 0.0f, // 1 (front-bot-right)
         0.5f,  0.5f, -0.1f,  1.0f, 1.0f, // 2 (front-top-right)
        -0.5f,  0.5f, -0.1f,  0.0f, 1.0f, // 3 (front-top-left)
        // Back face
        -0.5f, -0.5f,  0.1f,  1.0f, 0.0f, // 4 (back-bot-left)
         0.5f, -0.5f,  0.1f,  0.0f, 0.0f, // 5 (back-bot-right)
         0.5f,  0.5f,  0.1f,  0.0f, 1.0f, // 6 (back-top-right)
        -0.5f,  0.5f,  0.1f,  1.0f, 1.0f, // 7 (back-top-left)
        // Left face
        -0.5f, -0.5f,  0.1f,  0.0f, 0.0f, // 8  (back-bot-left)
        -0.5f, -0.5f, -0.1f,  1.0f, 0.0f, // 9  (front-bot-left)
        -0.5f,  0.5f, -0.1f,  1.0f, 1.0f, // 10 (front-top-left)
        -0.5f,  0.5f,  0.1f,  0.0f, 1.0f, // 11 (back-top-left)
        // Right face
         0.5f, -0.5f, -0.1f,  0.0f, 0.0f, // 12 (front-bot-right)
         0.5f, -0.5f,  0.1f,  1.0f, 0.0f, // 13 (back-bot-right)
         0.5f,  0.5f,  0.1f,  1.0f, 1.0f, // 14 (back-top-right)
         0.5f,  0.5f, -0.1f,  0.0f, 1.0f, // 15 (front-top-right)
        // Top face
        -0.5f,  0.5f, -0.1f,  0.0f, 0.0f, // 16 (front-top-left)
         0.5f,  0.5f, -0.1f,  1.0f, 0.0f, // 17 (front-top-right)
         0.5f,  0.5f,  0.1f,  1.0f, 1.0f, // 18 (back-top-right)
        -0.5f,  0.5f,  0.1f,  0.0f, 1.0f, // 19 (back-top-left)
        // Bottom face
        -0.5f, -0.5f,  0.1f,  0.0f, 0.0f, // 20 (back-bot-left)
         0.5f, -0.5f,  0.1f,  1.0f, 0.0f, // 21 (back-bot-right)
         0.5f, -0.5f, -0.1f,  1.0f, 1.0f, // 22 (front-bot-right)
        -0.5f, -0.5f, -0.1f,  0.0f, 1.0f  // 23 (front-bot-left)
    };
    unsigned int indices[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Back face
        4, 5, 6,
        6, 7, 4,
        // Left face
        8,  9,  10,
        10, 11, 8,
        // Right face
        12, 13, 14,
        14, 15, 12,
        // Top face
        16, 17, 18,
        18, 19, 16,
        // Bottom face
        20, 21, 22,
        22, 23, 20
    };

    Shader vShader = {VERTEX_SHADER_PATH, GL_VERTEX_SHADER, 0};
    Shader fShader = {FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER, 0}; 
    ShaderProgram shaderProgram({&vShader, &fShader});
    shaderProgram.use();
    //Generate VAO, VBO and EBO 
    GLuint VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    //Bind VAO
    glBindVertexArray(VAO);
    //Bind EBO and buffer indices data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //Bind VBO and buffer vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_DYNAMIC_DRAW);
    //Define attribute arrays and enable them in VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(data)
    {
        if(nrChannels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //Load texture data into buffer
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //Generate Mipmaps automatically
    }
    else
    {
        std::cout << "FAILED TO LOAD IMAGE" << std::endl;
    }
    stbi_image_free(data); //Free image data
    //Apply transformations to make the picture rotate (local space)
    glm::vec3 axis(0.0f, 1.0f, 0.0f);
    float rotDegrees = glm::radians(90.0 * glfwGetTime());
    glm::mat4 localMat = glm::rotate(glm::mat4(1.0f), rotDegrees, axis);
    //Apply Camera transformations (transform to view space)
    glm::mat4 viewMat = glm::lookAt(cam.pos, cam.pos + cam.getDir(), cam.getUp());
    //Apply clip-space transformation (perspective)
    //glm::mat4 projMat = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 10.0f);
    glm::mat4 projMat = glm::perspective(glm::radians(45.0f),
                                        (float)WIN_WIDTH/(float)WIN_HEIGHT,
                                        0.1f, 10.0f);
    //Combine into one big transformation matrix
    glm::mat4 transformMat = projMat * viewMat * localMat;

    GLint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
    shaderProgram.setInt("texSampler", 0); //Set texSampler to read from GL_TEXTURE0
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f); // Set color (RGBA color scheme) for clearing window
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) // While window shouldn't close
    {
        frameStart = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clear color buffer before rendering next frame

        rotDegrees = glm::radians(90.0 * glfwGetTime());
        localMat = glm::rotate(glm::mat4(1.0f), rotDegrees, axis);
        viewMat = glm::lookAt(cam.pos, cam.pos + cam.getDir(), cam.getUp());
        transformMat = projMat * viewMat * localMat;
        shaderProgram.setMat4(transformLoc, transformMat);
        //Draw shapes
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 32, GL_UNSIGNED_INT, (void*) 0);
        glfwSwapBuffers(window);

        dt = glfwGetTime() - frameStart;
        checkForKeyPress(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);
    glDeleteProgram(shaderProgram.ID);

    glfwTerminate(); // Close window
    return 0;
}

//------------------------------------------Functions-----------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {glViewport(0, 0, width, height);}
void keyPress_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void checkForKeyPress(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_W))
        cam.moveForward(dt);
    if(glfwGetKey(window, GLFW_KEY_S))
        cam.moveBackward(dt);
    if(glfwGetKey(window, GLFW_KEY_A))
        cam.strafeLeft(dt);
    if(glfwGetKey(window, GLFW_KEY_D))
        cam.strafeRight(dt);
    if(glfwGetKey(window, GLFW_KEY_SPACE))
        cam.moveUp(dt);
    if(glfwGetKey(window, GLFW_KEY_X))
        cam.moveDown(dt);
    
}