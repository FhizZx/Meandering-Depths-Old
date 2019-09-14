#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const float SPEED = 6;
const float ZOOM = 45.0f;
const float SENSITIVITY = 0.1f;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ROLL_LEFT,
    ROLL_RIGHT
};


class Camera{

    public:
        glm::mat4 getViewMatrix();
        glm::vec3 getPosition();
        Camera(int screenwidth, int screenheight, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

        float getZoom() {return Zoom;}
        glm::vec3 getFront() {return Front;}
        void ProcessKeyboard(Camera_Movement direction, float deltaTime);
        void ProcessMouseMovement(float xpos, float ypos);
        void ProcessMouseScroll(float yoffset);
    private:
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 Position;
        float MovementSpeed, Zoom, Sensitivity;
        int screenWidth;
        int screenHeight;
        float lastX;
        float lastY;


        void Pitch(float angle);
        void Yaw(float angle);
        void Roll(float angle);
};




#endif // CAMERA_H
