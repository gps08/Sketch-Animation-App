#ifndef SKETCH_ANIMATION_OBJECT_H
#define SKETCH_ANIMATION_OBJECT_H

#include <iostream>
#include <vector>
#include "Doodle.h"
#include "Rectangle.h"

struct node{ int start,end,index; };

class Object {

private:
    std::vector<node> selection;
    std::vector<Doodle> doodles;
    double fx,fy,lx,ly;

public:
    Object(double fx, double fy, double lx, double ly, std::vector<Doodle> d) : doodles(d), fx(fx), fy(fy), lx(lx), ly(ly)
    {
        for(int i=0;i<d.size();i++) {
            std::vector<int> range=d[i].searchRange(fx,lx,fy,ly);
            for(int j=0;j<range.size();j=j+2) {
                node n; n.index=i; n.start=range[j];
                n.end=range[j+1]; selection.push_back(n);
            }
        }
    }

    void translate(double x,double y){
        for(node i:selection){
            for(int j=i.start;j<i.end;j+=2)
                doodles[i.index].addVal(j,x),doodles[i.index].addVal(j+1,y);
            doodles[i.index].updateBuffer();
        }
    }

    void scale(double factor,double cx,double cy){
        for(node i:selection){
            for(int j=i.start;j<i.end;j+=2){
                doodles[i.index].addVal(j,-cx),doodles[i.index].addVal(j+1,-cy);
                doodles[i.index].mulVal(j,factor),doodles[i.index].mulVal(j+1,factor);
                doodles[i.index].addVal(j,cx),doodles[i.index].addVal(j+1,cy);
            }
            doodles[i.index].updateBuffer();
        }
    }

    void rotate(double theta,double cx,double cy){
        for(node i:selection){
            for(int j=i.start;j<i.end;j+=2){
                doodles[i.index].addVal(j,-cx),doodles[i.index].addVal(j+1,-cy);
                doodles[i.index].rotate(j,theta);
                doodles[i.index].addVal(j,cx),doodles[i.index].addVal(j+1,cy);
            }
            doodles[i.index].updateBuffer();
        }
    }

    void drawRectangle() {new Rectangle(fx,lx,fy,ly); }

    bool isValid(double x, double y){
        if(x>fx && lx>x && y>fy && ly>y) return true;
        return false;
    }

    void reset(){
        for(node i:selection){
            for(int j=i.start;j<i.end;j+=2) doodles[i.index].reset(j),doodles[i.index].reset(j+1);
            doodles[i.index].updateBuffer();
        }
    }
};

#endif //SKETCH_ANIMATION_OBJECT_H