#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <shader.h>
#include <chunk.h>
#include <camera.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <system.h>
#include <OpenSimplexNoise.hpp>
#include <world.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define WORLD_SIZE 2


OpenSimplexNoise noise;
int noiseValue(int i, int j, int k) {
    float freq1 = 0.03f;
    float freq2 = 0.15f;
    float freq3 = 1.1f;
    float freq4 = 0.2f;
    float amp1 = 0.76f;
    float amp2 = 0.21f;
    float amp3 = 0.02f;
    float amp4 = 0.20f;
    float largecavern = amp1 * (0.1 + noise.Evaluate(freq1* i,freq1*2 * j,freq1 * k));
    float tunnels = amp2 * (noise.Evaluate(freq2* i,freq2*3 * j,freq2 * k) + 0.2);
    float roughness = amp3 * noise.Evaluate(freq3* i,freq3 * j,freq3 * k);
    float stalacmites = amp4 * noise.Evaluate(freq4* i,freq4*0.00003f * j,freq4 * k);
    float result = 0;
    result += tunnels + largecavern + roughness + stalacmites;
    result = int((1 + result)/2 * 255);
    return result;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
System Game;
Camera Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
World testWorld = World(156);
int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Meandering Depths", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); //set viewport to window size
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    int work_grp_cnt[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

    int work_grp_size[3], work_grp_inv;

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
    printf("max local work group invocations %i\n", work_grp_inv);
    printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",work_grp_size[0], work_grp_size[1], work_grp_size[2]);
    printf("max global (total) work group size x:%i y:%i z:%i\n",work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);


    Game.Init();

    float cubeVertices[] = {
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f
    };

    unsigned int cubeIndices[] = {
        0, 1, 3,
        1, 2, 3,
        0, 4, 1,
        1, 4, 5,
        1, 5, 2,
        2, 5, 6,
        0, 3, 4,
        3, 7, 4,
        2, 7, 3,
        2, 6, 7,
        4, 7, 5,
        5, 7, 6
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    Shader shaderProgram("shaders/vertexShader.vs", "shaders/fragmentShader1.fs");
    Shader lightSourceProgram("shaders/lightSourceVertexShader.vs", "shaders/fragmentShader2.fs");
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);




    while(!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();

        glfwPollEvents();

        Game.ProcessInput(currentFrame, &Camera, &testWorld);

        Game.Update();
        glm::vec3 playerPos = Camera.getPosition();
        glm::vec3 playerFront = Camera.getFront();
        testWorld.Update(Camera.getPosition());
        glm::vec3 skycolour = glm::vec3(0.1f, 0.1f, 0.13f);
        glClearColor(skycolour.x, skycolour.y, skycolour.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        view = Camera.getViewMatrix();
        projection = glm::perspective(Camera.getZoom(), (float)1.0f * SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f);

        lightSourceProgram.use();
        glm::mat4 trans2 = glm::mat4(1.0f);
        glm::vec3 lightSourcePos = playerPos + playerFront * 2.0f;
        trans2 = glm::translate(trans2, lightSourcePos);
        trans2 = glm::scale(trans2, glm::vec3(0.1f, 0.1f, 0.1f));

        lightSourceProgram.setMatrix("transform", trans2);
        lightSourceProgram.setMatrix("model", model);
        lightSourceProgram.setMatrix("view", view);
        lightSourceProgram.setMatrix("projection", projection);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        shaderProgram.use();
        shaderProgram.setVectorFloat("viewPos",playerPos.x, playerPos.y, playerPos.z);
        shaderProgram.setVectorFloat("lightPos", lightSourcePos.x, lightSourcePos.y, lightSourcePos.z);
        shaderProgram.setVectorFloat("skyColour",skycolour.x, skycolour.y, skycolour.z);
        shaderProgram.setMatrix("transform", trans);
        shaderProgram.setMatrix("model", model);
        shaderProgram.setMatrix("view", view);
        shaderProgram.setMatrix("projection", projection);

        Game.Render();
        testWorld.Render();
        glfwSwapBuffers(window);
    }


    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            Game.setKey(key, true);
        else if (action == GLFW_RELEASE)
            Game.setKey(key, false);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Camera.ProcessMouseMovement(xpos, ypos);
}
