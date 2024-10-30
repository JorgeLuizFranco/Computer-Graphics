#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "objloader.hpp"  // Include your obj loader header

// Shader source for simplicity
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;
uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1.0);
    UV = vertexUV;  // Pass UV coordinates to the fragment shader
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec2 UV;
out vec4 color;

uniform sampler2D myTextureSampler;

void main() {
    color = texture(myTextureSampler, UV);  // Sample the texture using UV coordinates
}
)";

GLuint loadShaders(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void processInput(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp) {
    float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
}

GLuint loadTexture(const char* texturePath) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture\n";
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "OBJ Viewer with Camera", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to open GLFW window.\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Load first object
    std::vector<glm::vec3> vertices1, normals1;
    std::vector<glm::vec2> uvs1;
    bool res1 = loadOBJ("fat-batman/source/untitled.obj", vertices1, uvs1, normals1);
    if (!res1) {
        std::cerr << "Failed to load first OBJ file\n";
        return -1;
    }

    GLuint vertexBuffer1, uvBuffer1;
    glGenBuffers(1, &vertexBuffer1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer1);
    glBufferData(GL_ARRAY_BUFFER, vertices1.size() * sizeof(glm::vec3), &vertices1[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvBuffer1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer1);
    glBufferData(GL_ARRAY_BUFFER, uvs1.size() * sizeof(glm::vec2), &uvs1[0], GL_STATIC_DRAW);

    // Load second object
    std::vector<glm::vec3> vertices2, normals2;
    std::vector<glm::vec2> uvs2;
    bool res2 = loadOBJ("IronMan/IronMan.obj", vertices2, uvs2, normals2);  // Update the path accordingly
    if (!res2) {
        std::cerr << "Failed to load second OBJ file\n";
        return -1;
    }

    GLuint vertexBuffer2, uvBuffer2;
    glGenBuffers(1, &vertexBuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2);
    glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(glm::vec3), &vertices2[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvBuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer2);
    glBufferData(GL_ARRAY_BUFFER, uvs2.size() * sizeof(glm::vec2), &uvs2[0], GL_STATIC_DRAW);

    GLuint shaderProgram = loadShaders(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderProgram);

    GLuint texture1 = loadTexture("fat-batman/textures/Fatman_Batman_BaseColor.png");
    //GLuint texture2 = loadTexture("wolverine/wolwerine.png");  // Load texture for the second object

    glUniform1i(glGetUniformLocation(shaderProgram, "myTextureSampler"), 0);

    // Camera setup
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    GLint mvpLocation = glGetUniformLocation(shaderProgram, "MVP");

    while (!glfwWindowShouldClose(window)) {
        processInput(window, cameraPos, cameraFront, cameraUp);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model1 = glm::mat4(1.0f);  // Model matrix for the first object
        glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)); // Translate second object to the side
        glm::mat4 MVP1 = projection * view * model1;
        glm::mat4 MVP2 = projection * view * model2;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render first object
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &MVP1[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, vertices1.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Render second object
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &MVP2[0][0]);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture2);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer2);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, vertices2.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteBuffers(1, &vertexBuffer1);
    glDeleteBuffers(1, &uvBuffer1);
    glDeleteBuffers(1, &vertexBuffer2);
    glDeleteBuffers(1, &uvBuffer2);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
