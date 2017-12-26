#ifndef SKETCH_ANIMATION_DOODLE_H
#define SKETCH_ANIMATION_DOODLE_H
#include <vector>
#include <glad/glad.h>
#include <cmath>

class Doodle
{
private:
    std::vector<double> pts,backup; GLuint VAO,VBO,col;
    double abs(double x){ return x<0?(-1.0*x):x; }

public:
    Doodle(std::vector<double> points)
    {
        for(int i=0;i<points.size();i++) pts.push_back(points[i]),backup.push_back(points[i]);
        glGenVertexArrays(1,&VAO); glGenBuffers(1,&VBO); glBindVertexArray(VAO);
        updateBuffer(); glBindVertexArray(0);
    }

    void draw() { glBindVertexArray(VAO); glDrawArrays(GL_LINE_STRIP,0,pts.size()/2); glBindVertexArray(0); }
    void addVal(int i,double val){ pts[i]+=val; }
    void mulVal(int i,double val){ pts[i]*=val; }
    void rotate(int i,double theta){
        using std::cos; using std::sin;
        double x=pts[i],y=pts[i+1];
        pts[i]=x*cos(theta)-y*sin(theta);
        pts[i+1]=x*sin(theta)+y*cos(theta);
    }
    void reset(int i){ pts[i]=backup[i];}

    int search(double x,double y) {
        for(int i=0;i<pts.size()-1;i++)
            if(abs(pts[i]-x)<=0.01 && abs(pts[i+1]-y)<=0.01)
                return i;
        return -1;
    }

    std::vector<int> searchRange(double fx,double lx,double fy,double ly)
    {
        std::vector<int> range; int c=0,index;
        //double ftx = min(fx,lx),fty = min(fy,ly),ltx = max(fx,lx),lty = max(fy,ly);
        for(int i=0;i<pts.size();i=i+2)
        {
            if(fx<=pts[i] && pts[i]<=lx && fy<=pts[i+1] && pts[i+1]<=ly) { if(c==0) index=i; ++c; }
            else if(c!=0) { range.push_back(index); range.push_back(index+c*2); c=0; }
        }
        if(c!=0) { range.push_back(index); range.push_back(index+c*2); }
        return range;
    }

    void updateBuffer(){
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,pts.size()*sizeof(double),&pts[0],GL_STATIC_DRAW);
        glVertexAttribPointer(0,2,GL_DOUBLE,GL_FALSE,2*sizeof(double),0); glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
};

#endif