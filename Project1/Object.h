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
    GLfloat xPos, yPos, zPos, scale, angle;  // Add scale
    vData data;
    int axis = 1;
    Object(GLuint shaderProgram, vData data, float _xPos=0.0f, float _yPos=0.0f, float _zPos=0.0f, float _scale=1.0f, int axis=1);
    void Draw(bool mesh_active);
    void Move(float dx, float dy);
    void Scale(float factor);   // Add Scale method
    void Rotate(float angle);
};

#endif // STAR_H
