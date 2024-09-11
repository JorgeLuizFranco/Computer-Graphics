#ifndef STAR_H
#define STAR_H

#include <GL/glew.h>

struct Star {
    GLuint vao, vbo, ebo;
    GLuint shaderProgram;
    GLfloat xPos, yPos;

    Star(GLuint shaderProgram);
    void Draw();
    void Move(float dx, float dy);
};

#endif // STAR_H
