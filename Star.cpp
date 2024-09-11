#include "Star.h"

Star::Star(GLuint shaderProgram) : shaderProgram(shaderProgram), xPos(0.0f), yPos(0.0f) {
    GLfloat vertices[] = {
        // Position          // Color
        xPos, yPos + 0.5f,  1.0f, 1.0f, 0.0f, // Top
        xPos + 0.2f, yPos + 0.2f, 1.0f, 1.0f, 0.0f, // Inner top-right
        xPos + 0.5f, yPos,  1.0f, 1.0f, 0.0f, // Right
        xPos + 0.2f, yPos - 0.2f, 1.0f, 1.0f, 0.0f, // Inner bottom-right
        xPos, yPos - 0.5f,  1.0f, 1.0f, 0.0f, // Bottom
        xPos - 0.2f, yPos - 0.2f, 1.0f, 1.0f, 0.0f, // Inner bottom-left
        xPos - 0.5f, yPos,  1.0f, 1.0f, 0.0f, // Left
        xPos - 0.2f, yPos + 0.2f, 1.0f, 1.0f, 0.0f  // Inner top-left
    };

    GLuint elements[] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 7
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

void Star::Draw() {
    glBindVertexArray(vao);

    // Update vertex positions if needed (e.g., for movement)
    GLfloat vertices[] = {
        xPos, yPos + 0.5f,  1.0f, 1.0f, 0.0f, // Top
        xPos + 0.2f, yPos + 0.2f, 1.0f, 1.0f, 0.0f, // Inner top-right
        xPos + 0.5f, yPos,  1.0f, 1.0f, 0.0f, // Right
        xPos + 0.2f, yPos - 0.2f, 1.0f, 1.0f, 0.0f, // Inner bottom-right
        xPos, yPos - 0.5f,  1.0f, 1.0f, 0.0f, // Bottom
        xPos - 0.2f, yPos - 0.2f, 1.0f, 1.0f, 0.0f, // Inner bottom-left
        xPos - 0.5f, yPos,  1.0f, 1.0f, 0.0f, // Left
        xPos - 0.2f, yPos + 0.2f, 1.0f, 1.0f, 0.0f  // Inner top-left
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
}

void Star::Move(float dx, float dy) {
    xPos += dx;
    yPos += dy;
}
