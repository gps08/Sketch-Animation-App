#include "Motion.h"

#ifndef SKETCH_ANIMATION_ANIMATOR_H
#define SKETCH_ANIMATION_ANIMATOR_H

#endif //SKETCH_ANIMATION_ANIMATOR_H

class Node{
private:
    std::vector<int> Ids; Node *next;
public:
    Node(int id){ Ids.push_back(id); next=NULL; }
    Node* getNext() { return next; }

    void addMotion(int id){ Ids.push_back(id); }
    bool find(int id){ for(int i:Ids) if(i==id) return true; return false; }

    std::vector<int> getIds(){ return Ids; }
    void setNext(Node* next){ this->next=next; }
    bool nextFrame(std::vector<Motion>* motions){ bool ret=false; for(int i:Ids) ret|=(*motions)[i].nextFrame(); cout<<"return "<<ret<<endl; return ret;  }
};