#include "Object.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Object::Object(GLuint shaderProgram, vData data) 
    : shaderProgram(shaderProgram), xPos(0.0f), yPos(0.0f), scale(1.0f), data(data) {

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.n_vertices, data.vertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.n_elements, data.elements, GL_STATIC_DRAW);

    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

void Object::Draw(bool lines) {
    glBindVertexArray(vao);

    glm::mat4 id = glm::mat4(1.0f);
    glm::mat4 transform = glm::translate(id, glm::vec3(xPos, yPos, 0.0f));
    transform = glm::scale(transform, glm::vec3(scale, scale, 1.0f));  // Apply scaling

    GLint uniTransform = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(transform));

    if (lines) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    glDrawElements(GL_TRIANGLES, data.n_elements, GL_UNSIGNED_INT, 0);
}

void Object::Scale(float factor) {
    scale *= factor;
}


void Object::Move(float dx, float dy) {
    xPos += dx;
    yPos += dy;
}
