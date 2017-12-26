#ifndef SKETCH_ANIMATION_RECTANGLE_H
#define SKETCH_ANIMATION_RECTANGLE_H

#include <algorithm>
#include <glad/glad.h>

class Rectangle{
private:
    GLuint VAO,VBO;
public:
    Rectangle(double fx, double lx, double fy, double ly){
        double vertex[] ={
                std::min(fx,lx), std::min(fy,ly),
                std::max(fx,lx), std::min(fy,ly),
                std::max(fx,lx), std::max(fy,ly),
                std::min(fx,lx), std::max(fy,ly),
                std::min(fx,lx), std::min(fy,ly)
        };
        glGenVertexArrays(1,&VAO); glGenBuffers(1,&VBO);
        glBindVertexArray(VAO); glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,10*sizeof(double),vertex,GL_STATIC_DRAW);
        glVertexAttribPointer(0,2,GL_DOUBLE,GL_FALSE,2*sizeof(double),0); glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0); glLineWidth(1); glDrawArrays(GL_LINE_STRIP,0,5); glLineWidth(2);
        glBindVertexArray(0);
    }
};

#endif //SKETCH_ANIMATION_RECTANGLE_H