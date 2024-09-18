#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "Object.h"

// Shaders
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec3 position;
    in vec3 color;
    flat out vec3 Color;

    uniform mat4 transform;
    void main()
    {
        Color = color;
        gl_Position = transform * vec4(position, 1.0);
    }
)glsl";

const GLchar* fragmentSource = R"glsl(
    #version 150 core
    flat in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
    }
)glsl";

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

GLfloat bush_vertices[] = {
    0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.93f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.43f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.68f, 0.68f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.25f, 0.43f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.25f, 0.93f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.25f, 0.93f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.25f, 0.43f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.68f, 0.68f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.43f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.93f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f
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

GLfloat house_vertices[] = {
    -0.25f, -0.25f, -0.25f,1.0f, 0.0f, 0.0f, 
    0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 
    0.25f,  0.25f, -0.25f, 1.0f, 0.0f, 0.0f, 
    -0.25f,  0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
    -0.25f, -0.25f,  0.25f, 1.0f, 0.0f, 0.0f,
    0.25f, -0.25f,  0.25f, 1.0f, 0.0f, 0.0f,
    0.25f,  0.25f,  0.25f, 1.0f, 0.0f, 0.0f,
    -0.25f,  0.25f,  0.25f, 1.0f, 0.0f, 0.0f,
    0.35f,  0.25f,  0.35f, 0.5f, 0.5f, 0.0f,
    -0.35f,  0.25f,  0.35f, 0.5f, 0.5f, 0.0f,
    -0.35f,  0.25f,  -0.35f, 0.5f, 0.5f, 0.0f,
    0.35f,  0.25f,  -0.35f, 0.5f, 0.5f, 0.0f,
    0.0f,  0.5f,  0.0f, 0.5f, 0.5f, 0.0f,
    -0.05f, -0.25f, -0.25f,1.0f, 0.0f, 0.0f, 
    0.05f, -0.25f, -0.25f,1.0f, 0.0f, 0.0f, 
    0.05f, 0.05f, -0.25f,0.5f, 0.5f, 0.5f, 
    -0.05f, 0.05f, -0.25f,0.5f, 0.5f, 0.5f, 
};

GLuint house_elements[] = {
    1, 2, 14,
    15, 14, 2,
    16, 15, 2,
    16, 2, 3,
    13, 16, 3,
    13, 3, 0,
    4, 5, 6,
    6, 7, 4,
    0, 3, 7,
    7, 4, 0,
    1, 5, 6,
    6, 2, 1,
    0, 1, 5,
    5, 4, 0,
    3, 2, 6,
    6, 7, 3,
    6, 8, 7,
    7, 8, 9,
    3, 7, 9,
    3, 9, 10,
    10, 11, 3,
    11, 2, 3,
    11, 6, 2,
    11, 8, 6,
    12, 10 ,11,
    12, 11, 8,
    12, 8, 9,
    12, 9, 10,
    13, 14, 15,
    13, 15, 16
};
vData houseData{house_vertices, house_elements, sizeof(house_vertices), sizeof(house_elements)};

GLfloat ship_vertices[] = {
    0.0f, 0.5f, 0.0f, 0.1f, 1.0f, 0.1f,
    0.2f, 0.20f, 0.0f, 0.8f, 0.8f, 0.8f, 
    0.1f, 0.20f, 0.17f, 0.8f, 0.8f, 0.8f, 
    -0.1f, 0.20f, 0.17f, 0.8f, 0.8f, 0.8f, 
    -0.2f, 0.20f, 0.0f, 0.8f, 0.8f, 0.8f, 
    -0.1f, 0.20f, -0.17f, 0.8f, 0.8f, 0.8f, 
    0.1f, 0.20f, -0.17f, 0.8f, 0.8f, 0.8f, 
    0.5f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 
    0.25f, 0.0f, 0.43f, 0.8f, 0.8f, 0.8f, 
    -0.25f, 0.0f, 0.43f, 0.8f, 0.8f, 0.8f, 
    -0.5f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 
    -0.25f, 0.0f, -0.43f, 0.8f, 0.8f, 0.8f, 
    0.25f, 0.0f, -0.43f, 0.8f, 0.8f, 0.8f, 
    0.0f, -0.15f, 0.0f, 0.3f, 0.2f, 0.4f
};

GLuint ship_elements[] = {
    1, 2, 0,
    2, 3, 0,
    3, 4, 0,
    4, 5, 0,
    5, 6, 0, 
    6, 1, 0, 

    1, 7, 8,
    1, 2, 8,
    2, 8, 9,
    2, 3, 9,
    3, 9, 10,
    3, 4, 10,
    4, 10, 11,
    4, 5, 11,
    5, 11, 12,
    5, 6, 12,
    6, 12, 7,
    6, 1, 7,

    7, 8, 13,
    8, 9, 13,
    9, 10, 13,
    10, 11, 13,
    11, 12, 13,
    7, 12, 13
};

vData shipData{ship_vertices, ship_elements, sizeof(ship_vertices), sizeof(ship_elements)};

GLfloat cloud_vertices[] = {
    0.2f, 0.2f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.18f, 0.28f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.14f, 0.34f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.08f, 0.38f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.0f, 0.4f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.08f, 0.38f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.14f, 0.34f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.18f, 0.28f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.2f, 0.2f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.28f, 0.18f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.34f, 0.14f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.38f, 0.08f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.4f, 0.0f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.38f, -0.08f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.34f, -0.14f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.28f, -0.18f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.2f, -0.2f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.18f, -0.28f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.14f, -0.34f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.08f, -0.38f, 0.0f, 0.4f, 0.4f, 0.4f, 
    -0.0f, -0.4f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.08f, -0.38f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.14f, -0.34f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.18f, -0.28f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.2f, -0.2f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.28f, -0.18f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.34f, -0.14f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.38f, -0.08f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.4f, -0.0f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.38f, 0.08f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.34f, 0.14f, 0.0f, 0.4f, 0.4f, 0.4f, 
    0.28f, 0.18f, 0.0f, 0.4f, 0.4f, 0.4f
};

GLuint cloud_elements[] = {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    0, 5, 6,
    0, 6, 7,
    0, 7, 8,
    8, 9, 10,
    8, 10, 11,
    8, 11, 12,
    8, 12, 13,
    8, 13, 14,
    8, 14, 15,
    8, 15, 16,
    16, 17, 18,
    16, 18, 19,
    16, 19, 20,
    16, 20, 21,
    16, 21, 22,
    16, 22, 23,
    16, 23, 24,
    24, 25, 26,
    24, 26, 27,
    24, 27, 28,
    24, 28, 29,
    24, 29, 30,
    24, 30, 31,
    24, 31, 0,
    0, 24, 16,
    0, 8, 16
};
vData cloudData{cloud_vertices, cloud_elements, sizeof(cloud_vertices), sizeof(cloud_elements)};

GLfloat background_vertices[] = {
    -1.0f, -1.0f, 0.0f, 0.6f, 0.3f, 0.1f,
    -1.0f, -0.3f, 0.0f, 0.6f, 0.3f, 0.1f,
    1.0f, -1.0f, 0.0f, 0.6f, 0.3f, 0.1f,
    1.0f, -0.3f, 0.0f, 0.6f, 0.3f, 0.1f,
}; 
GLuint background_elements[] = {
    0, 1, 2,
    1, 2, 3
};
vData backgroundData{background_vertices, background_elements, sizeof(background_vertices), sizeof(background_elements)};

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
    glEnable(GL_DEPTH_TEST);

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

    // Create objects
    Object star0(shaderProgram, starData, -0.05f, 0.80f, 0.00f, 0.16f, 2);
    Object star1(shaderProgram, starData, -0.31f, 0.37f, 0.00f, 0.11f, 2);
    Object bush0(shaderProgram, bushData, -0.79, -0.74, 0.00, 0.14, 2);
    Object bush1(shaderProgram, bushData, 0.26, -0.86, 0.00, 0.13, 2);
    Object bush2(shaderProgram, bushData, 0.70, -0.40, 0.00, 0.12, 2);
    Object house(shaderProgram, houseData, -0.05f, -0.28f, 0.00f, 0.81f);
    Object ship(shaderProgram, shipData, 0.60, 0.53, 0.00, 0.26);
    Object cloud0(shaderProgram, cloudData, -0.42f, 0.71f, 0.0f, 0.31f, 2);
    Object cloud1(shaderProgram, cloudData, -0.80f, 0.41f, 0.00f, 0.31f, 2);
    Object cloud2(shaderProgram, cloudData, 0.21f, 0.50f, 0.00f, 0.31f, 2);
    Object background(shaderProgram, backgroundData, 0.0f, 0.0f, 0.99f);

    bool mesh_active = false;
    std::vector<Object*> objs;

    objs.push_back(&star0);
    objs.push_back(&star1);
    objs.push_back(&bush0);
    objs.push_back(&bush1);
    objs.push_back(&bush2);
    objs.push_back(&cloud0);
    objs.push_back(&cloud1);
    objs.push_back(&cloud2);
    objs.push_back(&house);
    objs.push_back(&ship);

    Object *obj = &ship;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 
        background.Draw(mesh_active);
        // Draw objects
        for (auto v : objs) {
            v->Draw(mesh_active);
        }

        objs[0]->Rotate(0.7f);

        objs[1]->Move(0.001f, -0.001f);

        for (auto key = GLFW_KEY_0; key  <= GLFW_KEY_9; key++) {
            if (glfwGetKey(window, key) == GLFW_PRESS) {
                obj = objs[key-GLFW_KEY_0];
            }
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            mesh_active = !mesh_active;

        // rotation
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
            obj->Rotate(0.1f);
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            obj->Rotate(-0.1f);

        // scaling
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
            obj->Scale(0.95f);
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
            obj->Scale(1.05f);

        // translation
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            obj->Move(0.0f, 0.01f);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            obj->Move(0.0f, -0.01f);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            obj->Move(-0.01f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            obj->Move(0.01f, 0.0f);

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
