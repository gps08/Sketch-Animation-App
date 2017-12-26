#ifndef SKETCH_ANIMATION_ARROW_H
#define SKETCH_ANIMATION_ARROW_H

#include <glad/glad.h>
#include <cmath>

class Arrow{
private:
    void drawLine(double fx,double fy,double lx,double ly){
        double vertex[] ={fx,fy,lx,ly}; GLuint VAO,VBO;
        glGenVertexArrays(1,&VAO); glGenBuffers(1,&VBO);
        glBindVertexArray(VAO); glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,4*sizeof(double),vertex,GL_STATIC_DRAW);
        glVertexAttribPointer(0,2,GL_DOUBLE,GL_FALSE,2*sizeof(double),0); glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0); glLineWidth(1); glDrawArrays(GL_LINE_STRIP,0,2); glLineWidth(2);
        glBindVertexArray(0);
    }
public:
    Arrow(double fx,double fy,double lx,double ly,bool doubleHead=false){
        drawLine(fx,fy,lx,ly);
        double tx=lx-(lx-fx)*0.05,ty=ly-(ly-fy)*0.05;
        double p1x=(tx-ty-lx+ly)/sqrt(2)+lx,p1y=(tx+ty-lx-ly)/sqrt(2)+ly;
        double p2x=(tx+ty-lx-ly)/sqrt(2)+lx,p2y=(-tx+ty+lx-ly)/sqrt(2)+ly;
        drawLine(lx,ly,p1x,p1y); drawLine(lx,ly,p2x,p2y); //drawLine(p1x,p1y,p2x,p2y);
        if(doubleHead){
            double tmpX=fx,tmpY=fy; fx=lx; fy=ly; lx=tmpX; ly=tmpY;
            double tx=lx-(lx-fx)*0.05,ty=ly-(ly-fy)*0.05;
            double p1x=(tx-ty-lx+ly)/sqrt(2)+lx,p1y=(tx+ty-lx-ly)/sqrt(2)+ly;
            double p2x=(tx+ty-lx-ly)/sqrt(2)+lx,p2y=(-tx+ty+lx-ly)/sqrt(2)+ly;
            drawLine(lx,ly,p1x,p1y); drawLine(lx,ly,p2x,p2y); //drawLine(p1x,p1y,p2x,p2y);
        }
    }
};

#endif //SKETCH_ANIMATION_ARROW_H