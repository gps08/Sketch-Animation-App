#ifndef SKETCH_ANIMATION_CANVAS_H
#define SKETCH_ANIMATION_CANVAS_H

#include <glad/glad.h>
#include <vector>
#include "Doodle.h"
#include "Object.h"
#include "Node.h"
#include "Arrow.h"

class Canvas
{

private:
    GLuint VAO,VBO,W,H; std::vector<double> points;
    std::vector<Doodle> doodles; std::vector<Object> objects;
    std::vector<Motion> motions;
    Node* head,*curr; int selectedObject;

    void updateBuffer(){
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,points.size()*sizeof(double),&points[0],GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }

public:
    Canvas(GLuint width,GLuint height) {
        glGenVertexArrays(1,&VAO); glGenBuffers(1,&VBO);
        glBindVertexArray(VAO); glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glVertexAttribPointer(0,2,GL_DOUBLE,GL_FALSE,2*sizeof(double),0); glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0); glBindVertexArray(0);
        W = width; H = height; selectedObject=-1; head=curr=NULL;
    }

    void draw(bool show) {
        glBindVertexArray(VAO); glDrawArrays(GL_LINE_STRIP,0,points.size()/2); glBindVertexArray(0);
        for(Doodle i:doodles) i.draw();
        if(show){
            for(Object i:objects) i.drawRectangle();
            Node* temp=head; while(temp!=NULL){
                vector<int> tmp=temp->getIds();
                for(int i=0;i+1<tmp.size();i++) new Arrow(motions[tmp[i]].getCx(),motions[tmp[i]].getCy(),motions[tmp[i+1]].getCx(),motions[tmp[i+1]].getCy(),true);
                if(temp->getNext()!=NULL){
                    int to=temp->getNext()->getIds()[0];
                    new Arrow(motions[tmp[0]].getCx(),motions[tmp[0]].getCy(),motions[to].getCx(),motions[to].getCy());
                }
                temp=temp->getNext();
            }
        }
    }

    void addPoint(double x, double y) {
        if(points.size()>1 and points[points.size()-2]==x and points[points.size()-1]==y) return;
        points.push_back(x); points.push_back(y); updateBuffer();
    }

    void addDoodle() {
        Doodle t(points); doodles.push_back(t);
        cout<<"Added new doodle with "<<points.size()<<" points\n";
        points.clear(); updateBuffer();
    }

    void addPath() {
        if(points.empty()) return;
        std::vector<double> t(points); points.clear(); motions[selectedObject].enableTranslate(t); updateBuffer();
    }

    void deleteDoodle(double x, double y) {
        for(int i=0;i<doodles.size();i++) if(doodles[i].search(x,y)>=0){ doodles.erase(doodles.begin()+i); if(selectedObject==i) selectedObject=-1; break; }
    }

    void addObject(double fx,double fy,double lx,double ly){
        double sx=min(fx,lx),ex=max(fx,lx),sy=min(fy,ly),ey=max(fy,ly);
        Object t(sx,sy,ex,ey,doodles); objects.push_back(t); motions.push_back(Motion(t,(sx+ex)/2,(sy+ey)/2));
        cout<<"Object Added with ("<<sx<<","<<sy<<"),("<<ex<<","<<ey<<")\n";
        selectedObject=objects.size()-1;
        // Sort objects acc to area
    }

    bool nextFrame(){
        if(curr==NULL) return false;
        if(!curr->nextFrame(&motions)) curr=curr->getNext();
        return true;
    }

    bool nextFrame2(){ return motions[selectedObject].nextFrame(); }

    void reset(){
        curr=head; for(int i=0;i<motions.size();i++) motions[i].reset();
    }

    int getObject(double x,double y){
        for(int i=0;i<objects.size();i++) if(objects[i].isValid(x,y)) return i;
        return -1;
    }

    void selObject(double x, double y) {
        selectedObject=getObject(x,y);
        cout<<"Object selected "<<selectedObject<<endl;
    }

    void addEdge(int from,int to,bool isAfter){
        if(from==-1 or to==-1) return;
        if(head==NULL) head=new Node(from),curr=head; Node* temp=head;
        while(temp!=NULL){
            if(temp->find(from)){
                if(isAfter) temp->setNext(new Node(to));
                else temp->addMotion(to); break;
            }
            temp=temp->getNext();
        }
        cout<<"Added edge from "<<from<<" to "<<to<<" type:"<<(isAfter?" after":" together")<<"\n";
    }

    void enable(int choice){
        // 1-scale 2-rotate 3-breathe 4-buzz
        switch(choice){
            case 1: motions[selectedObject].enableScale(2); cout<<"Enabled Scaling\n"; return;
            case 2: motions[selectedObject].enableRotate(360); cout<<"Enabled Rotate\n"; return;
            case 3: motions[selectedObject].enableBreathe(2); cout<<"Enabled Breathe\n"; return;
            case 4: motions[selectedObject].enableBuzz(); cout<<"Enabled Buzz\n"; return;
        }
    }
    bool isSelected() { return selectedObject!=-1; }
};

#endif