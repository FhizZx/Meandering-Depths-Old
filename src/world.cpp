#include <world.h>

World::World(int threshold) {
    World::threshold = threshold;
}


void World::decreaseThreshold() {
    threshold = threshold - 1;
    start = 1;
    Update({currentX,currentY, currentZ});
}

void World::increaseThreshold() {
    threshold = threshold + 1;
    start = 1;
    Update({currentX,currentY, currentZ});
}


void World::Update(glm::vec3 position) {
    int px = int(floor(position.x / CHUNK_SIZE));
    int py = int(floor(position.y / CHUNK_SIZE));
    int pz = int(floor(position.z / CHUNK_SIZE));
    int MOD = 2 * VIEW_DISTANCE + 1;
    if (px != currentX || py != currentY || pz != currentZ || start == 1) {
        start = 0;
        currentX = px;
        currentY = py;
        currentZ = pz;
        for (int i = px - VIEW_DISTANCE; i <= px + VIEW_DISTANCE; i++)
            for (int j = py - VIEW_DISTANCE; j <= py + VIEW_DISTANCE; j++)
                for (int k = pz - VIEW_DISTANCE; k <= pz + VIEW_DISTANCE; k++) {
                    int ip = i % MOD;
                    if (ip < 0)
                        ip+= MOD;
                    int jp = j % MOD;
                    if (jp < 0)
                        jp+= MOD;
                    int kp = k % MOD;
                    if (kp < 0)
                        kp+= MOD;
                    activeChunks[ip][jp][kp].SetChunk(threshold, i, j, k, 0);

                }
        std::cout << px << " " << py << " " << pz << '\n';
    }
}

void World::Render() {
    for (int i = 0; i < 2 * VIEW_DISTANCE + 1; i++)
        for (int j = 0; j < 2 * VIEW_DISTANCE + 1; j++)
            for (int k = 0; k < 2 * VIEW_DISTANCE + 1; k++)
                activeChunks[i][j][k].Render();
}
