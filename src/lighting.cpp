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
unsigned int WIN_WIDTH = 800;
unsigned int WIN_HEIGHT = 600;
const char *WIN_TITLE = "Learning OpenGL";
const char *VERTEX_SHADER_PATH = "..\\shaders\\colorShader.vs";
const char *FRAGMENT_SHADER_PATH = "..\\shaders\\colorShader.fs";
const char *LIGHT_VERTEX_SHADER_PATH = "..\\shaders\\lightShader.vs";
const char *LIGHT_FRAGMENT_SHADER_PATH = "..\\shaders\\lightShader.fs";
//-------------------------------Global Variables----------------------------------------
Camera cam = Camera();
float currFrameTime = 0.0f, lastFrameTime = 0.0f;
bool isFirstMouseMovement = true;
bool mouseControlsEnabled = true;
//---------------------------Prototype functions-----------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void keyPress_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void checkForKeyPress(GLFWwindow *window);
void mouse_move_callback(GLFWwindow *window, double xpos, double ypos);
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
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

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
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float triangleVertices[] = {
        //     positions            normals
        // Back face
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // 0 (back-bot-left)
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,// 1 (back-bot-right)
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,// 2 (back-top-right)
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,// 3 (back-top-left)
        // Front face
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,// 4 (front-bot-left)
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // 5 (front-bot-right)
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // 6 (front-top-right)
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // 7 (front-top-left)
        // Left face
        -0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f, // 8  (front-bot-left)
        -0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f, // 9  (back-bot-left)
        -0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f, // 10 (back-top-left)
        -0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f, // 11 (front-top-left)
        // Right face
         0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, // 12 (back-bot-right)
         0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, // 13 (front-bot-right)
         0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, // 14 (front-top-right)
         0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, // 15 (back-top-right)
        // Top face
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, // 16 (back-top-left)
         0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, // 17 (back-top-right)
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f, // 18 (front-top-right)
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f, // 19 (front-top-left)
        // Bottom face
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f, // 20 (front-bot-left)
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f, // 21 (front-bot-right)
         0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, // 22 (back-bot-right)
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, // 23 (back-bot-left)
    };
    unsigned int indices[] = {
        // Back face
        0, 1, 2,
        2, 3, 0,
        // Front face
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
    //Initialize shader program
    Shader vShader = {VERTEX_SHADER_PATH, GL_VERTEX_SHADER};
    Shader fShader = {FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER};
    ShaderProgram shaderProgram({&vShader, &fShader});
    shaderProgram.use();
    //Shader program for light
    Shader vlShader = {LIGHT_VERTEX_SHADER_PATH, GL_VERTEX_SHADER};
    Shader flShader = {LIGHT_FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER};
    ShaderProgram lShaderProgram({&vlShader,&flShader});

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    //Define attribute arrays and enable them in VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Generate light VAO
    GLuint lVAO;
    glGenVertexArrays(1, &lVAO);
    //Bind VAO so I can modify vertex attributes
    glBindVertexArray(lVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //Apply transformations to make the picture rotate (world space)
    glm::vec3 axis(0.0f, 1.0f, 0.0f);
    glm::vec3 lightPos(1.5f, 1.0f, 1.5f);
    float rotDegrees = glm::radians(60.0f);
    glm::mat4 modelMat = glm::rotate(glm::mat4(1.0f), rotDegrees, axis);
    //Apply Camera transformations (transform to view space)
    glm::mat4 viewMat = glm::lookAt(cam.pos, cam.pos + cam.getDir(), cam.getUp());
    //Apply clip-space transformation (perspective)
    //glm::mat4 projMat = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 10.0f);
    glm::mat4 projMat = glm::perspective(glm::radians(45.0f),
                                        (float)WIN_WIDTH/(float)WIN_HEIGHT,
                                        0.1f, 10.0f);
    glm::mat4 lModelMat = glm::mat4(1.0f);
    lModelMat = glm::translate(lModelMat, lightPos);
    lModelMat = glm::scale(lModelMat, glm::vec3(0.2f));

    GLint projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
    GLint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
    GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
    GLint lProjLoc = glGetUniformLocation(lShaderProgram.ID, "proj");
    GLint lViewLoc = glGetUniformLocation(lShaderProgram.ID, "view");
    GLint lModelLoc = glGetUniformLocation(lShaderProgram.ID, "model");

    shaderProgram.setMat4(modelLoc, modelMat);
    shaderProgram.setMat4(viewLoc, viewMat);
    shaderProgram.setMat4(projLoc, projMat);
    shaderProgram.setVec3("color", 1.0f, 0.5f, 0.3f);
    shaderProgram.setVec3("lightPos", lightPos);
    lShaderProgram.use();
    lShaderProgram.setMat4(lModelLoc, lModelMat);
    lShaderProgram.setMat4(lViewLoc, viewMat);
    lShaderProgram.setMat4(lProjLoc, projMat);
    lShaderProgram.setVec3("color", 1.0f, 1.0f, 0.85f);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set color (RGBA color scheme) for clearing window
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) // While window shouldn't close
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clear color buffer before rendering next frame
        viewMat = glm::lookAt(cam.pos, cam.pos + cam.getDir(), cam.getUp());
        //Draw the main cube
        shaderProgram.use();
        shaderProgram.setMat4(viewLoc, viewMat);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*) 0);
        //Draw light
        lShaderProgram.use();
        lShaderProgram.setMat4(lViewLoc, viewMat);
        glBindVertexArray(lVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*) 0);

        glfwSwapBuffers(window);

        lastFrameTime = currFrameTime;
        currFrameTime = glfwGetTime();
        checkForKeyPress(window);
        cam.computeDirFromAngles();
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram.ID);
    glDeleteProgram(lShaderProgram.ID);

    glfwTerminate(); // Close window
    return 0;
}

//------------------------------------------Functions-----------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    WIN_WIDTH = width;
    WIN_HEIGHT = height;
}
void keyPress_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if(mouseControlsEnabled)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseControlsEnabled = false;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseControlsEnabled = true;
        }
    }
}
void checkForKeyPress(GLFWwindow *window)
{
    float dt = currFrameTime - lastFrameTime;
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
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
        cam.moveDown(dt);
    
}
void mouse_move_callback(GLFWwindow *window, double xpos, double ypos)
{
    static float lastPosX = WIN_WIDTH/2;
    static float lastPosY = WIN_HEIGHT/2;
    if(isFirstMouseMovement)
    {
        lastPosX = xpos;
        lastPosY = ypos;
        isFirstMouseMovement = false;
        return;
    }
    float dx = (xpos - lastPosX);
    float dy = (lastPosY - ypos);
    lastPosX = xpos;
    lastPosY = ypos;
    cam.turn(dx, dy);
}