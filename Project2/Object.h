#ifndef OBJ_H
#define OBJ_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct vData {
    GLfloat *vertices;
    GLuint *elements;
    size_t n_vertices;
    size_t n_elements;
};

class Object {
public:
    GLuint vao, vbo, ebo;
    GLuint shaderProgram;
    GLfloat xPos, yPos, zPos, scale, angle;
    vData data;
    int axis;

    Object(GLuint shaderProgram, vData data, float _xPos=0.0f, float _yPos=0.0f, float _zPos=0.0f, float _scale=1.0f, int axis=1);
    void Draw(bool mesh_active);
    void Move(float dx, float dy);
    void Scale(float factor);
    void Rotate(float angle);
    void LoadModel(const std::string& path); // New method for loading models
};

#endif // OBJ_H
