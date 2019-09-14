#include <camera.h>


Camera::Camera(int screenwidth, int screenheight, glm::vec3 position, glm::vec3 front, glm::vec3 up):
MovementSpeed(SPEED), Zoom(ZOOM), Sensitivity(SENSITIVITY) {
    screenHeight = screenheight;
    screenWidth = screenwidth;
    lastX = 1.0f * screenWidth / 2;
    lastY = 1.0f * screenHeight / 2;
    Position = position;
    Front = front;
    Up = up;
    Right = glm::normalize(glm::cross(Front, Up));
}

glm::vec3 Camera::getPosition() {
    return Position;
}

void Camera::Pitch(float angle) {
    glm::mat4 r = glm::mat4(1.0f);
    r = glm::rotate(r, glm::radians(angle), Right);
    Up = glm::normalize(glm::vec3(r * glm::vec4(Up, 1.0f)));
    Front = glm::normalize(glm::vec3(r * glm::vec4(Front, 1.0f)));
}
void Camera::Yaw(float angle) {
    glm::mat4 r = glm::mat4(1.0f);
    r = glm::rotate(r, glm::radians(-angle), Up);
    Right = glm::normalize(glm::vec3(r * glm::vec4(Right, 1.0f)));
    Front = glm::normalize(glm::vec3(r * glm::vec4(Front, 1.0f)));
}
void Camera::Roll(float angle) {
    glm::mat4 r = glm::mat4(1.0f);
    r = glm::rotate(r, glm::radians(angle), Front);
    Right = glm::normalize(glm::vec3(r * glm::vec4(Right, 1.0f)));
    Up = glm::normalize(glm::vec3(r * glm::vec4(Up, 1.0f)));
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD) {
        Position += Front * velocity;
    }
    else if (direction == BACKWARD) {
        Position -= Front * velocity;
    }
    else if(direction == LEFT) {
        Position -= Right * velocity;
    }
    else if(direction == RIGHT) {
        Position += Right * velocity;
    }
    else if(direction == ROLL_LEFT)
        Roll(-0.1f);
    else if(direction == ROLL_RIGHT)
        Roll(0.1f);
}
void Camera::ProcessMouseMovement(float xpos, float ypos) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    xoffset *= Sensitivity;
    yoffset *= Sensitivity;
    Yaw(xoffset);
    Pitch(yoffset);
}

void Camera::ProcessMouseScroll(float yoffset) {
    if(Zoom >= 1.0f && Zoom <= 60.0f)
        Zoom -= yoffset/10;
    if(Zoom < 1.0f)
        Zoom = 1.0f;
    if(Zoom > 60.0f)
        Zoom = 60.0f;
}
