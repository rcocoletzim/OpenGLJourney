#include <iostream>
#include <string>
#include <format>
#include <print>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

const char * APP_TITLE= "Introduction to Modern OpenGL - Hello Triangle";
const int gWindowWidth = 1024;
const int gWindowHeight = 768;
GLFWwindow *gWindow = nullptr;
constexpr bool gFullScreen = false;
bool gWireframe = false;

const GLchar * vertexShaderSrc = 
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"void main()"
"{"
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

const GLchar * fragmentShaderSrc = 
"#version 330 core\n"
"out vec4 frag_color;"
"void main()"
"{"
"   frag_color = vec4(0.35f, 0.96f, 0.3f, 1.0f);"
"}";

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main()
{
    if(!initOpenGL())
    {
        std::cerr << "GLFW initialization failed"<< std::endl;
        return -1;
    }

    GLfloat vertices[] = {
        -0.5f,  0.5f,  0.0f,  // Top
         0.5f,   0.5f, 0.0f,  // Right
         0.5f, -0.5f,  0.0f,   // Left
         -0.5f, -0.5f, 0.0f
    };

    GLuint indices[] = {
        0,1,2,  //trin 0
        0,2,3   //trin 1
    };

    GLuint vbo,ibo,vao;

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,GL_STATIC_DRAW);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSrc, NULL);
    glCompileShader(vs);

    GLint result;
    GLchar infoLog[512];
    glGetShaderiv(vs,GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
        std::cout << "Error! Vertex shader failed to compile "<< infoLog<<std::endl;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&fragmentShaderSrc,NULL);
    glCompileShader(fs);

    glGetShaderiv(fs,GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
        std::cout << "Error! Fragment shader failed to compile "<< infoLog<<std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog),NULL, infoLog);
        std::cout << "Error! Shader program linker failure"<< infoLog <<std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        glBindVertexArray(0);

        glfwSwapBuffers(gWindow);
    }
    
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}


void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window,GL_TRUE);

    if(key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        gWireframe = !gWireframe;
        if(gWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void showFPS(GLFWwindow* window)
{
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSecons = glfwGetTime();

    elapsedSeconds = currentSecons - previousSeconds;

    // limit text update 4 times per second

    if (elapsedSeconds > 0.25)
    {
        previousSeconds = currentSecons;
        double fps = (double) frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;
        std::string outs = std::format("{}   FPS: {:.3f}   Frame Time: {:.3f} (ms)",APP_TITLE,fps,msPerFrame);
        glfwSetWindowTitle(window,outs.c_str());

        frameCount = 0;
    }
    frameCount++;
}

bool initOpenGL()
{
if(!glfwInit())
    {
        std::cerr << "GLWF intitialization failed\n";
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    
    if(gFullScreen)
    {
        GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
        if(pVmode != nullptr)
        {
            gWindow = glfwCreateWindow(pVmode->width,pVmode->height,APP_TITLE,pMonitor,nullptr);
        }
    }
    else
    {
       gWindow = glfwCreateWindow(gWindowWidth,gWindowHeight,APP_TITLE,nullptr,nullptr);
    }
    if(gWindow == nullptr)
    {
        std::cerr << "Failed to create the window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(gWindow);
    glewExperimental = GL_TRUE;

    glfwSetKeyCallback(gWindow, glfw_onKey);

    if(glewInit() != GLEW_OK)
    {
        std::cerr << "glew initialization failed\n";
        glfwTerminate();
        return false;
    }

    
    
    glClearColor(0.23f,0.38f,0.47f,1.0f);

    return true;
}