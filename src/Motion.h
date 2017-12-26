#ifndef SKETCH_ANIMATION_MOTION_H
#define SKETCH_ANIMATION_MOTION_H

#include "Object.h"

class Motion
{

private:
    Object obj; std::vector<double> path; int currPos,buzzed;
    double cx,cy,scaleFactor,oldFactor,oldFactor2,oldTheta,targetTheta,targetBreathe,sumx,sumy;
    bool translate,scale,rotate,buzz,breathe,breatheIn;

public:
    Motion(Object obj,double cx,double cy): obj(obj),cx(cx),cy(cy) {
        sumx=sumy=currPos=0; oldFactor=oldFactor2=1; oldTheta=0; buzzed=0; breatheIn=true;
        translate=scale=rotate=buzz=breathe=false;
    }

    bool nextFrame(){
        bool isChanged=false;
        if(translate && currPos!=path.size()){
            obj.translate(path[currPos],path[currPos+1]);
            sumx+=path[currPos]; sumy+=path[currPos+1]; currPos+=2; isChanged=true;
        }

        if(scale and oldFactor+0.05<=scaleFactor){
            obj.scale((oldFactor+0.05)/oldFactor,cx+sumx,cy+sumy); oldFactor+=0.05;
            isChanged=true;
        }

        double stepSize=0.017453292519943295*2;
        if(rotate and oldTheta+stepSize<=targetTheta){
            //obj.rotate(stepSize,cx+sumx,cy+sumy); oldTheta+=stepSize; // one degree = 2*pi/360
            obj.rotate(stepSize,cx,cy); oldTheta+=stepSize; // one degree = 2*pi/360
            isChanged=true;
        }

        if(buzz and buzzed!=100){
            double off=0.02; buzzed++; isChanged=true;
            if(buzzed%2) obj.translate(off,off); else obj.translate(-off,-off);
        }

        if(breathe){
            if(breatheIn){
                double stepSize=0.01;
                if(oldFactor2+stepSize>targetBreathe) breatheIn=false;
                obj.scale((oldFactor2+stepSize)/oldFactor2,cx,cy); oldFactor2+=stepSize;
                isChanged=true;
            }
            else{
                double stepSize=-0.01;
                if(oldFactor2+stepSize>=1){ obj.scale((oldFactor2+stepSize)/oldFactor2,cx,cy); oldFactor2+=stepSize; isChanged=true; }
            }
        }
        return isChanged;
    }

    void enableTranslate(std::vector<double> path){
        for(int i=0;i<path.size();i+=2) path[i]-=cx,path[i+1]-=cy; // relative to the center
        for(int i=0;i<path.size()-3;i+=2) path[i]=path[i+2]-path[i],path[i+1]=path[i+3]-path[i+1]; // removing absolute val
        this->path=vector<double>(path.begin(),path.end()-2); translate=true;
        cout<<"Path added with "<<path.size()<<" points\n";
    }

    void enableScale(double scaleFactor){ this->scaleFactor=scaleFactor; scale=true; }
    void enableRotate(double theta){ targetTheta=(22.0*theta)/(7.0*180.0); rotate=true; }
    void enableBreathe(double scaleFactor){ targetBreathe=scaleFactor; breathe=true; }
    void enableBuzz(){ buzz=true; }

    void reset(){ sumx=sumy=oldTheta=buzzed=0; oldFactor=oldFactor2=1; currPos=0; breatheIn=true; obj.reset(); }

    double getCx(){ return cx; }
    double getCy(){ return cy; }
};

#endif //SKETCH_ANIMATION_MOTION_H