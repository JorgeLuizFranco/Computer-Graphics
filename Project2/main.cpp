#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Object.h"

// Star Object Data
GLfloat star_vertices[] = {
    0.0f,  +0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 
    +0.2f,  +0.2f, 0.0f, 1.0f, 1.0f, 0.0f, 
    +0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f, 
    +0.2f,  -0.2f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.0f,  -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,
    -0.2f,  -0.2f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
    -0.2f,  +0.2f, 0.0f, 1.0f, 1.0f, 0.0f
};

GLuint star_elements[] = {
    0, 1, 3,
    1, 2, 3,
    0, 3, 4,
    0, 4, 5,
    5, 6, 7,
    0, 5, 7
};

vData starData{star_vertices, star_elements, sizeof(star_vertices), sizeof(star_elements)};

// Batman Object Data
GLfloat batman_vertices[] = {
    -0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 0.0f,
    +0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, +0.25f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.15f, -0.15f, 0.0f, 0.0f, 0.0f, 0.0f,
    +0.15f, -0.15f, 0.0f, 0.0f, 0.0f, 0.0f
};

GLuint batman_elements[] = {
    0, 1, 2,
    0, 3, 4,
};

vData batmanData{batman_vertices, batman_elements, sizeof(batman_vertices), sizeof(batman_elements)};

// House Object Data
GLfloat house_vertices[] = {
    -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 
    0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 
    0.25f,  0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 
    -0.25f,  0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
    -0.25f, -0.25f,  0.25f, 1.0f, 0.0f, 0.0f,
    0.25f, -0.25f,  0.25f, 1.0f, 0.0f, 0.0f,
    0.25f,  0.25f,   0.25f, 1.0f, 0.0f, 0.0f,
    -0.25f,  0.25f,  0.25f, 1.0f, 0.0f, 0.0f
};

GLuint house_elements[] = {
    0, 1, 2,
    0, 2, 3,
    4, 5, 6,
    4, 6, 7,
};

vData houseData{house_vertices, house_elements, sizeof(house_vertices), sizeof(house_elements)};

// Vertex and Fragment Shader Source Code
const char* vertexSource = R"(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
out vec3 fragColor;
uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(position, 1.0);
    fragColor = color;
}
)";

const char* fragmentSource = R"(
#version 330 core
in vec3 fragColor;
out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}
)";

// Main Rendering Function
void renderScene(GLuint shaderProgram) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw House
    Object house(shaderProgram, houseData);
    house.Move(-0.5f, -0.5f);
    house.Draw(true);

    // Draw Batman
    Object batman(shaderProgram, batmanData);
    batman.Move(0.5f, 0.5f);
    batman.Draw(false);

    // Draw Star
    Object star(shaderProgram, starData);
    star.Move(0.0f, 0.0f);
    star.Draw(false);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Project", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW" << std::endl;
        return -1;
    }

    

    // Shader program creation
    GLuint shaderProgram = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Vertex Shader Compilation
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    glAttachShader(shaderProgram, vertexShader);

    // Fragment Shader Compilation
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    glLinkProgram(shaderProgram);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        renderScene(shaderProgram);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
    return 0;
}
