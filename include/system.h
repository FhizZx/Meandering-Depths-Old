#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <settings.h>
#include <camera.h>

#include <world.h>

class System{

    public:
        System();
        ~System();
        void Init();
        void ProcessInput(GLfloat currentframe, Camera *camera, World *world);
        void Update();
        void Render();
        void setKey(int i, GLboolean t){ Keys[i] = t;}
    private:
        GLboolean Keys[1024];
        GLfloat lastFrame;
};

#endif // SYSTEM_H
