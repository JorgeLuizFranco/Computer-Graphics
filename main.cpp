#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "Object.h"

// Shaders
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in vec3 color;
    out vec3 Color;

    uniform mat4 transform;
    void main()
    {
        Color = color;
        gl_Position = transform * vec4(position, 0.0, 1.0);
    }
)glsl";

const GLchar* fragmentSource = R"glsl(
    #version 150 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";

// Star Data
GLfloat star_vertices[] = {
    // Position          // Color
    0.0f,  +0.5f, 1.0f, 1.0f, 0.0f, // Top
     +0.2f,  +0.2f, 1.0f, 1.0f, 0.0f, // Inner top-right
     +0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // Right
     +0.2f,  -0.2f, 1.0f, 1.0f, 0.0f, // Inner bottom-right
    0.0f,  -0.5f,  1.0f, 1.0f, 0.0f, // Bottom
     -0.2f,  -0.2f, 1.0f, 1.0f, 0.0f, // Inner bottom-left
     -0.5f, 0.0f,  1.0f, 1.0f, 0.0f, // Left
     -0.2f,  +0.2f, 1.0f, 1.0f, 0.0f  // Inner top-left
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

// Bush Data
GLfloat bush_vertices[] = {
    0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.93f, 0.25f, 0.0f, 1.0f, 0.0f,
    0.43f, 0.25f, 0.0f, 1.0f, 0.0f,
    0.68f, 0.68f, 0.0f, 1.0f, 0.0f,
    0.25f, 0.43f, 0.0f, 1.0f, 0.0f,
    0.25f, 0.93f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.25f, 0.93f, 0.0f, 1.0f, 0.0f,
    -0.25f, 0.43f, 0.0f, 1.0f, 0.0f,
    -0.68f, 0.68f, 0.0f, 1.0f, 0.0f,
    -0.43f, 0.25f, 0.0f, 1.0f, 0.0f,
    -0.93f, 0.25f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
};
GLuint bush_elements[] = {
    0, 1, 2,
    2, 3, 4,
    4, 5, 6,
    6, 7, 8,
    8, 9, 10,
    10, 11, 12,
    0, 10, 12,
    0, 10, 8,
    0, 8, 6,
    0, 6, 4,
    0, 4, 2,
};
vData bushData{bush_vertices, bush_elements, sizeof(bush_vertices), sizeof(bush_elements)};

// Moon Data
const int moon_segments = 36; // Number of segments for the circle
GLfloat moon_vertices[moon_segments * 5 + 5]; // +5 for the center vertex
GLuint moon_elements[moon_segments * 3];

void createMoonData() {
    // Center vertex
    moon_vertices[0] = 0.0f;
    moon_vertices[1] = 0.0f;
    moon_vertices[2] = 0.8f; // Gray color
    moon_vertices[3] = 0.8f;
    moon_vertices[4] = 0.8f;

    // Circle vertices
    float angleStep = 2.0f * M_PI / moon_segments;
    for (int i = 0; i < moon_segments; ++i) {
        float angle = i * angleStep;
        moon_vertices[(i + 1) * 5] = 0.5f * cos(angle);
        moon_vertices[(i + 1) * 5 + 1] = 0.5f * sin(angle);
        moon_vertices[(i + 1) * 5 + 2] = 0.8f; // Gray color
        moon_vertices[(i + 1) * 5 + 3] = 0.8f;
        moon_vertices[(i + 1) * 5 + 4] = 0.8f;
    }

    // Circle elements (fan)
    for (int i = 0; i < moon_segments; ++i) {
        moon_elements[i * 3] = 0;
        moon_elements[i * 3 + 1] = i + 1;
        moon_elements[i * 3 + 2] = (i + 1) % moon_segments + 1;
    }
}
vData moonData{moon_vertices, moon_elements, sizeof(moon_vertices), sizeof(moon_elements)};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    // Create and compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Create objects
    Object star0(shaderProgram, starData);
    Object star1(shaderProgram, starData);
    Object bush(shaderProgram, bushData);
    createMoonData(); // Initialize moon data
    Object moon(shaderProgram, moonData);

    bool lines = false;
    while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw objects
    star0.Draw(lines);
    star1.Draw(lines);
    bush.Draw(lines);
    moon.Draw(lines);  // Draw the moon

    // Toggle polygon mode
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        lines = !lines;

    // Translation for stars
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        star1.Move(0.0f, 0.01f);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        star1.Move(0.0f, -0.01f);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        star1.Move(-0.01f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        star1.Move(0.01f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        star0.Move(0.0f, 0.01f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        star0.Move(0.0f, -0.01f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        star0.Move(-0.01f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        star0.Move(0.01f, 0.0f);

    // Translation for the moon
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        moon.Move(0.0f, 0.01f);
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        moon.Move(0.0f, -0.01f);

    // Scaling for the moon
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        moon.Scale(1.01f);  // Increase scale
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        moon.Scale(0.99f);  // Decrease scale

    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glfwTerminate();
    return 0;
}
