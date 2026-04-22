#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//--------------------------------Settings-----------------------------------------------
const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;
const char *WIN_TITLE = "Learning OpenGL";
const char *vertexShaderSourceCode =
    "#version 330 core\n"
    "layout (location=0) in vec3 inPos;\n"
    "layout (location=1) in vec3 inColor;\n"
    "out vec3 vertexColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(inPos, 1.0f);\n"
    "vertexColor = inColor;\n"
    "}\0";
const char *fragmentShaderSourceCode =
    "#version 330 core\n"
    "in vec3 vertexColor;\n"
    "out vec4 fragColor;\n"
    "void main()\n"
    "{\n"
    "fragColor = vec4(vertexColor, 1.0f);\n"
    "}\0";
//---------------------------Prototype functions-----------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void keyPress_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
inline GLuint createVertexShader(const char *const *sourceCode); //Pass pointer to a string
inline GLuint createFragmentShader(const char *const *sourceCode);
//--------------------------------Main---------------------------------------------------
int main()
{
    if(!glfwInit()) //Initialize GLFW
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3); //Set version to OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// Set to core profile

    GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL); //Create window
    if (window == NULL) //In case window doesn't create
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //Set the context to the created window

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //Load GLAD (needs an existing OpenGL context beforehand)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0,0, 800,600); //Create a viewport for the window. Maps from (-1,1) to (0, max width/height)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //Set callback for resizing window
    glfwSetKeyCallback(window, keyPress_callback); //Set callback for key press

    float triangleVertices[] = {
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right 
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top 
    };

    GLuint vertexShader = createVertexShader(&vertexShaderSourceCode);
    GLuint fragmentShader = createFragmentShader(&fragmentShaderSourceCode);
    //Create shader program
    GLuint shaderProgram = glCreateProgram();
    //Attach shaders to program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR LINKING SHADERS\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glUseProgram(shaderProgram);
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //Set color (RGBA color scheme) for clearing window

    while(!glfwWindowShouldClose(window)) //While window shouldn't close
    {
        glClear(GL_COLOR_BUFFER_BIT); //Clear color buffer before rendering next frame
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window); //Swap color buffers (render next frame)
        glfwPollEvents(); //Check for events
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate(); //Close window
    return 0;
}

//------------------------------------------Functions-----------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0,0, width, height); }
void keyPress_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
inline GLuint createVertexShader(const char *const *sourceCode)
{
    //Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, sourceCode, NULL);
    glCompileShader(vertexShader);
    //Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR COMPILING VERTEX SHADER\n" << infoLog << std::endl;
    }
    return vertexShader;
}
inline GLuint createFragmentShader(const char *const *sourceCode)
{
    //Create Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, sourceCode, NULL);
    glCompileShader(fragmentShader);
    //Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR COMPILING FRAGMENT SHADER\n" << infoLog << std::endl;
    }
    return fragmentShader;
}