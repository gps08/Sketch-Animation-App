#ifndef SKETCH_ANIMATION_SKETCHWIN_H
#define SKETCH_ANIMATION_SKETCHWIN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui_impl_glfw_gl3.h>
#include "ShadersUtil.h"
#include "Canvas.h"
#include "Rectangle.h"
#include "Arrow.h"

using namespace std;

class SketchWin
{

private:
    GLFWwindow* ref; Canvas* canvas; GLuint shaderProgram,height,width;
    int click,from,to; bool wasPressed,show,isAfter; double fx,fy,lx,ly;

public:
    SketchWin(GLuint width,GLuint height):height(height),width(width)
    {
        glfwInit(); wasPressed=show=false; click=1;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_SAMPLES,8);

        ref=glfwCreateWindow(width,height,"Canvas",NULL,NULL);
        if(ref==NULL) {
            cout<<"Failed to create GLFW window"<<endl;
            glfwTerminate(); exit(-1);
        }
        glfwMakeContextCurrent(ref);
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) cout<<"Failed to initialize OpenGL context"<<endl,exit(-1);
        glfwSetWindowUserPointer(ref,this);

        ImGui_ImplGlfwGL3_Init(ref,false);
        glEnable(GL_MULTISAMPLE); glLineWidth(2);

        shaderProgram = ShadersUtil("../res/vshader.vert","../res/fshader.frag").ID;
        canvas= new Canvas(width,height);
    }

    void run()
    {
        while(!glfwWindowShouldClose(ref))
        {
            glClearColor(1,1,1,1); //background color
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(shaderProgram);

            if(click==12){ if(!canvas->nextFrame()) canvas->reset(),click=1; }
            else if(click==7){ if(!canvas->nextFrame2()) canvas->reset(),click=1; }
            else { checkInput(); showTools(); }

            canvas->draw(show);

            glfwSwapBuffers(ref);
            glfwPollEvents();
        }
    }

    void checkInput(){
        if(glfwGetKey(ref,GLFW_KEY_ESCAPE)==GLFW_PRESS) glfwSetWindowShouldClose(ref,true);
        else if(glfwGetMouseButton(ref,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) {
            double x,y; glfwGetCursorPos(ref,&x,&y);
            x=2.0*(x/width)-1; y=2.0*((height-y)/height)-1;
            if(x<-0.8) return; // remove this for the right click
            if(click==1) canvas->addPoint(x,y);
            else if(click==2) canvas->deleteDoodle(x,y);
            else if(click==3) { if(!wasPressed){ fx=x; fy=y;} lx=x;ly=y; new Rectangle(fx,lx,fy,ly); }
            else if(click==4) { canvas->selObject(x,y); show=false; }
            else if(click==5) { if(!wasPressed) from=canvas->getObject(x,y),fx=x,fy=y; lx=x; ly=y; new Arrow(fx,fy,lx,ly,!isAfter); }
            else if(click==6) canvas->addPoint(x,y);
            wasPressed=true;
        }
        else if(wasPressed){
            if(click==1) canvas->addDoodle();
            else if(click==3) canvas->addObject(fx,fy,lx,ly);
            else if(click==4) click=1;
            else if(click==5) canvas->addEdge(from,canvas->getObject(lx,ly),isAfter),show=false,click=1;
            else if(click==6) canvas->addPath(),click=1;
            wasPressed=false;
        }
    }

    void showTools()
    {
        ImGui_ImplGlfwGL3_NewFrame();
        //ImGui::SetNextWindowSize(ImVec2(0,0),ImGuiSetCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(0,0));
        bool show_another_window=canvas->isSelected();
        ImGui::Begin("Tools",&show_another_window,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

        if(ImGui::Button("Pen    ")) click=1;
        else if(ImGui::Button("Rub    ")) click=2;
        else if(ImGui::Button("Object ")) click=3;
        else if(ImGui::Button("Show   ")) click=4,show=true;
        else if(ImGui::Button("AddEdge")) click=5;
        else if(ImGui::Button("Play   ")) click=12;

        if(click==5){
            bool show_another_window=false;
            ImGui::SetNextWindowPos(ImVec2(0,180));
            ImGui::Begin("Graph Tools",&show_another_window,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

            if(ImGui::Button("After")) isAfter=true;
            else if(ImGui::Button("Together")) isAfter=false;

            ImGui::End(); show=true;
        }

        else if(canvas->isSelected()){
            bool show_another_window=false;
            ImGui::SetNextWindowPos(ImVec2(0,180));
            ImGui::Begin("Object Tools",&show_another_window,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

            if (ImGui::Button("Path  ")) click=6;
            else if (ImGui::Button("Check ")) click=7;
            else if (ImGui::Button("Scale ")) click=8;
            else if (ImGui::Button("Rotate")) click=9;
            else if (ImGui::Button("Breath")) click=10;
            else if (ImGui::Button("Buzz  ")) click=11;

            if(click>=8 && click<=11) canvas->enable(click-7),click=1;
            ImGui::End();
        }
        ImGui::End(); ImGui::Render();
    }

    ~SketchWin(){ glfwTerminate(); glDeleteProgram(shaderProgram); }
};

#endif