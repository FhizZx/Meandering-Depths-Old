#include <chunk.h>

int noiseValue(int i, int j, int k);
float redNoise(float x, float y, float z);
float greenNoise(float x, float y, float z);
float blueNoise(float x, float y, float z);
Chunk::Chunk() {
}

void Chunk::SetChunk(int thresHold, int x, int y, int z, int lod) {
    meshData.clear();
    threshold = thresHold;
    LoD = lod;
    xCoord = x;
    yCoord = y;
    zCoord = z;
    Generate();
    CreateMesh();
}

void Chunk::Generate() {
    for (int i = 0; i < CHUNK_SIZE + 1; i++) {
        for (int j = 0; j < CHUNK_SIZE + 1; j++) {
            for (int k = 0; k < CHUNK_SIZE + 1; k++) {
                vertexData[i][j][k] = noiseValue(i + xCoord * CHUNK_SIZE, j + yCoord * CHUNK_SIZE, k + zCoord * CHUNK_SIZE);
            }
        }
    }
}

void Chunk::CreateMesh() {
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_SIZE; k++) {
                MarchingCubes(i, j, k);
            }
        }
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
     duration<double, std::milli> time_span = t2 - t1;
    std::cout << time_span.count()<< " ms\n";
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*meshData.size(), &meshData.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 6*sizeof(float),  (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(VAO);
}

void Chunk::MarchingCubes(int x, int y, int z) {
    int cubeIndex = 0;
    for (int i = 0; i <= 1; i++)
        for (int j = 0; j <= 1; j++)
            for (int k = 0; k <= 1; k++) {
                int vertInd = i * 4 + j * 2 + k;
                vertInd = fixNumbers[vertInd];
                if (vertexData[x+i][y+j][z+k] < threshold)
                    cubeIndex |= 1 << vertInd;
            }
    int t = 0;
    while (marTri[cubeIndex][t] != -1) {
        glm::vec3 triangle[3];
        for (int i = 0; i < 3; i++) {
            int vertex0 = edgeCorners[marTri[cubeIndex][t + i]][0];
            int vertex1 = edgeCorners[marTri[cubeIndex][t + i]][1];
            int x0 = (vertex0 >> 2) & 1;
            int y0 = (vertex0 >> 1) & 1;
            int z0 = vertex0 & 1;

            int x1 = (vertex1 >> 2) & 1;
            int y1 = (vertex1 >> 1) & 1;
            int z1 = vertex1 & 1;
            if (vertexData[x + x0][y + y0][z + z0] > vertexData[x + x1][y + y1][z + z1]) {
                int aux = x0;
                x0 = x1;
                x1 = aux;
                aux = y0;
                y0 = y1;
                y1 = aux;
                aux = z0;
                z0 = z1;
                z1 = aux;
            }
            if(vertexData[x + x1][y + y1][z + z1] == vertexData[x + x0][y + y0][z + z0])
                std::cout << "problem ";
            float interp = 1.0f * (threshold - vertexData[x + x0][y + y0][z + z0]) / (vertexData[x + x1][y + y1][z + z1] - vertexData[x + x0][y + y0][z + z0]);
            triangle[i].x = (xCoord * CHUNK_SIZE + x + x0 + interp * (x1 - x0));
            triangle[i].y = (yCoord * CHUNK_SIZE + y + y0 + interp * (y1 - y0));
            triangle[i].z = (zCoord * CHUNK_SIZE + z + z0 + interp * (z1 - z0));
        }
        glm::vec3 edge1 = triangle[1] - triangle[0];
        glm::vec3 edge2 = triangle[2] - triangle[0];
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
        for (int i = 0; i < 3; i++) {
            meshData.push_back(triangle[i].x);
            meshData.push_back(triangle[i].y);
            meshData.push_back(triangle[i].z);
            meshData.push_back(normal.x);
            meshData.push_back(normal.y);
            meshData.push_back(normal.z);
        }

        t += 3;
    }

}

void Chunk::Render() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, meshData.size()/2);
}
