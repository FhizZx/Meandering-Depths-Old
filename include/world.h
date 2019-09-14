#ifndef WORLD_H
#define WORLD_H

#define VIEW_DISTANCE 1

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chunk.h>
#include <cmath>

class World{
    public:
        World(int threshold);
        void Update(glm::vec3 position);
        void Render();
        void decreaseThreshold();
        void increaseThreshold();
    private:
        bool start = 1;
        int currentX = 0;
        int currentY = 0;
        int currentZ = 0;
        int threshold;
        Chunk activeChunks[2 * VIEW_DISTANCE + 1][2 * VIEW_DISTANCE + 1][2 * VIEW_DISTANCE + 1];

};


#endif // WORLD_H

