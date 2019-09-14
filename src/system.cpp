#include <system.h>


System::System() {
}
System::~System() {
}
void System::Init() {

}
void System::ProcessInput(GLfloat currentframe, Camera *camera, World *world) {
    GLfloat deltaTime = currentframe - lastFrame;
    lastFrame = currentframe;
    if (Keys[GLFW_KEY_W] == true)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (Keys[GLFW_KEY_S] == true)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (Keys[GLFW_KEY_A] == true)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (Keys[GLFW_KEY_D] == true)
        camera->ProcessKeyboard(RIGHT, deltaTime);
    if (Keys[GLFW_KEY_Q] == true)
        camera->ProcessKeyboard(ROLL_LEFT, deltaTime);
    if (Keys[GLFW_KEY_E] == true)
        camera->ProcessKeyboard(ROLL_RIGHT, deltaTime);
    if (Keys[GLFW_KEY_UP] == true)
        world->increaseThreshold();
    if (Keys[GLFW_KEY_DOWN] == true)
        world->decreaseThreshold();
}
void System::Update() {
}
void System::Render() {

}

