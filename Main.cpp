#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//---------------------------Prototype functions-----------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void keyPress_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
inline GLuint createVertexShader();
inline GLuint createFragmentShader();

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

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); //Create window
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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //Set color (RGBA color scheme) for clearing window

    float triangleVertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    GLuint vertexShader = createVertexShader();
    GLuint fragmentShader = createFragmentShader();
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)) //While window shouldn't close
    {
        glClear(GL_COLOR_BUFFER_BIT); //Clear color buffer before rendering next frame
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window); //Swap color buffers (render next frame)
        glfwPollEvents(); //Check for events
    }

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
inline GLuint createVertexShader()
{
    //Create vertex shader
    const char *vertexShaderSourceCode =
    "#version 330 core\n"
    "layout (location=0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\0";
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceCode, NULL);
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
inline GLuint createFragmentShader()
{
    //Create Fragment shader
    const char *fragmentShaderSourceCode =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.5f, 0.3f, 1.0f);\n"
    "}\0";
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
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