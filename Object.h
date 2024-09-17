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
    GLfloat xPos, yPos;
    vData data;
    Object(GLuint shaderProgram, vData data);
    void Draw(bool lines);
    void Move(float dx, float dy);
};

#endif // STAR_H
