#include "Object.h"
#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>


Object::Object(GLuint shaderProgram, vData data, float _xPos, float _yPos, float _zPos, float _scale, int axis) 
    : shaderProgram(shaderProgram), angle(0.0f), data(data), xPos(_xPos), yPos(_yPos), zPos(_zPos), scale(_scale), axis(axis) {
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
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

void Object::Draw(bool mesh_active) {
    glBindVertexArray(vao);
    glUseProgram(shaderProgram);

    glm::vec3 rotation_axis(0.0f, 1.0f, 0.0f);
    if (axis == 0) rotation_axis = glm::vec3(1.0, 0.0f, 0.0f);
    if (axis == 2) rotation_axis = glm::vec3(0.0, 0.0f, 1.0f);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(xPos, yPos, zPos));
    transform = glm::rotate(transform, angle, rotation_axis);
    transform = glm::scale(transform, glm::vec3(scale, scale, scale));
    
    GLint uniTransform = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(uniTransform, 1, GL_FALSE, glm::value_ptr(transform));

    if (mesh_active) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDrawElements(GL_TRIANGLES, data.n_elements, GL_UNSIGNED_INT, 0);
}


void Object::LoadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // Process the loaded model (vertices, indices, etc.)
    // ...
}

void Object::Scale(float factor) {
    scale *= factor;
}

void Object::Move(float dx, float dy) {
    xPos += dx;
    yPos += dy;
}

void Object::Rotate(float angle) {
    this->angle += angle;
}
