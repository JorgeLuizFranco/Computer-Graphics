#ifndef OBJ_H
#define OBJ_H

#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct vData {
    GLfloat *vertices;
    GLuint *elements;
    size_t n_vertices;
    size_t n_elements;
};
struct Object {
    GLuint vao, vbo, ebo;
    GLuint shaderProgram;
    GLfloat xPos, yPos, scale;  // Add scale
    vData data;
    Object(GLuint shaderProgram, vData data);
    void Draw(bool lines);
    void Move(float dx, float dy);
    void Scale(float factor);   // Add Scale method
};

#endif // STAR_H
