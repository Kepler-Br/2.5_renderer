#include "renderengine.h"

std::vector<float> xenfa::RenderEngine::generatePortalVertices(uint wallIndex, uint sectorIndex)
{
    const Wall &wall = walls[wallIndex];
    const Wall &nextWall = walls[wall.nextWallIndex];
    const Sector &sector = sectors[sectorIndex];
    const Sector &nextSector = sectors[wall.nextSectorIndex];
    std::vector<float> vertices;

    if(sector.ceiling - nextSector.ceiling > 0.0f)
    {
        vertices.push_back(wall.point.x);
        vertices.push_back(wall.point.y);
        vertices.push_back(sector.ceiling);

        vertices.push_back(wall.point.x);
        vertices.push_back(wall.point.y);
        vertices.push_back(nextSector.ceiling);

        vertices.push_back(nextWall.point.x);
        vertices.push_back(nextWall.point.y);
        vertices.push_back(nextSector.ceiling);


        vertices.push_back(wall.point.x);
        vertices.push_back(wall.point.y);
        vertices.push_back(sector.ceiling);

        vertices.push_back(nextWall.point.x);
        vertices.push_back(nextWall.point.y);
        vertices.push_back(nextSector.ceiling);

        vertices.push_back(nextWall.point.x);
        vertices.push_back(nextWall.point.y);
        vertices.push_back(sector.ceiling);
    }

    if(sector.floor - nextSector.floor > 0.0f)
    {
        vertices.push_back(wall.point.x);
        vertices.push_back(wall.point.y);
        vertices.push_back(sector.floor);

        vertices.push_back(wall.point.x);
        vertices.push_back(wall.point.y);
        vertices.push_back(nextSector.floor);

        vertices.push_back(nextWall.point.x);
        vertices.push_back(nextWall.point.y);
        vertices.push_back(nextSector.floor);


        vertices.push_back(wall.point.x);
        vertices.push_back(wall.point.y);
        vertices.push_back(sector.floor);

        vertices.push_back(nextWall.point.x);
        vertices.push_back(nextWall.point.y);
        vertices.push_back(nextSector.floor);

        vertices.push_back(nextWall.point.x);
        vertices.push_back(nextWall.point.y);
        vertices.push_back(sector.floor);
    }


    return vertices;
}

std::vector<float> xenfa::RenderEngine::generateWallVertices(int wallIndex, int sectorIndex)
{
    const Wall &wall = walls[wallIndex];
    const Wall &nextWall = walls[wall.nextWallIndex];
    const Sector &sector = sectors[sectorIndex];
    std::vector<float> vertices;
    constexpr uint numVerticleCoordinates = 3*6;
    vertices.reserve(numVerticleCoordinates);

    vertices.push_back(wall.point.x);
    vertices.push_back(wall.point.y);
    vertices.push_back(sector.floor);

    vertices.push_back(nextWall.point.x);
    vertices.push_back(nextWall.point.y);
    vertices.push_back(sector.floor);

    vertices.push_back(wall.point.x);
    vertices.push_back(wall.point.y);
    vertices.push_back(sector.ceiling);


    vertices.push_back(nextWall.point.x);
    vertices.push_back(nextWall.point.y);
    vertices.push_back(sector.floor);

    vertices.push_back(wall.point.x);
    vertices.push_back(wall.point.y);
    vertices.push_back(sector.ceiling);

    vertices.push_back(nextWall.point.x);
    vertices.push_back(nextWall.point.y);
    vertices.push_back(sector.ceiling);


    return vertices;
}

std::vector<float> xenfa::RenderEngine::generatePortalUV(uint wallIndex, uint sectorIndex)
{
    const Wall &wall = walls[wallIndex];
    const Wall &nextWall = walls[wall.nextWallIndex];
    const Sector &sector = sectors[sectorIndex];
    const Sector &nextSector = sectors[wall.nextSectorIndex];
    std::vector<float> UV;

    if(sector.ceiling - nextSector.ceiling > 0.0f)
    {
        UV.push_back(0.0f);
        UV.push_back(0.0f); // sector

        UV.push_back(0.0f);
        UV.push_back(1.0f); // nextSector

        UV.push_back(1.0f);
        UV.push_back(1.0f); // nextSector


        UV.push_back(0.0f);
        UV.push_back(0.0f); // sector

        UV.push_back(1.0f);
        UV.push_back(1.0f); // nextSector

        UV.push_back(1.0f);
        UV.push_back(0.0f); // sector
    }

    if(sector.floor - nextSector.floor > 0.0f)
    {
        UV.push_back(0.0f);
        UV.push_back(0.0f);

        UV.push_back(0.0f);
        UV.push_back(1.0f); // nextSector

        UV.push_back(1.0f);
        UV.push_back(1.0f); // nextSector


        UV.push_back(0.0f);
        UV.push_back(0.0f);

        UV.push_back(1.0f);
        UV.push_back(1.0f); // nextSector

        UV.push_back(1.0f);
        UV.push_back(0.0f);
    }

    return UV;
}

std::vector<float> xenfa::RenderEngine::generateWallUV(int wallIndex, int sectorIndex)
{
    const Wall &wall = walls[wallIndex];
    const Wall &nextWall = walls[wall.nextWallIndex];
    const Sector &sector = sectors[sectorIndex];
    std::vector<float> UV;
    constexpr uint numUVCoordinates = 2*6;
    UV.reserve(numUVCoordinates);

    UV.push_back(0.0f);
    UV.push_back(0.0f);

    UV.push_back(1.0f);
    UV.push_back(0.0f);

    UV.push_back(0.0f);
    UV.push_back(1.0f);


    UV.push_back(1.0f);
    UV.push_back(0.0f);

    UV.push_back(0.0f);
    UV.push_back(1.0f);

    UV.push_back(1.0f);
    UV.push_back(1.0f);
    return UV;
}

std::vector<float> xenfa::RenderEngine::generateSectorVertices(uint sectorIndex, bool generateFloor)
{
    const Sector &sector = sectors[sectorIndex];

    std::vector<float> vertices;
    vertices.reserve(sector.numWalls);

    Wall currentWall = walls[sector.startWall];
    for(uint i = 0; i < sector.numWalls; i++)
    {
        vertices.push_back(currentWall.point.x);
        vertices.push_back(currentWall.point.y);
        if(generateFloor)
            vertices.push_back(sector.floor);
        else
            vertices.push_back(sector.ceiling);
        //        // Lazy way to triangulate. Don't try to understand.
        //        //Works like translating GL_TRIANGLE_FAN into GL_TRIANGLE
        //        if(i%2 == 0 && i != 0)
        //        {
        //            const uint size = vertices.size();
        //            vertices.push_back(vertices[size]-2);
        //            vertices.push_back(vertices[size]-1);
        //        }
        currentWall = walls[currentWall.nextWallIndex];
    }
    return vertices;
}

std::vector<float> xenfa::RenderEngine::generateSectorUV(uint sectorIndex)
{
    const Sector &sector = sectors[sectorIndex];

    std::vector<float> UV;
    UV.reserve(sector.numWalls);

    Wall currentWall = walls[sector.startWall];
    float xMax = -INFINITY;
    float yMax = -INFINITY;
    float xMin = INFINITY;
    float yMin = INFINITY;
    for(uint i = 0; i < sector.numWalls; i++)
    {
        if(xMax < currentWall.point.x)
            xMax = currentWall.point.x;
        if(yMax < currentWall.point.y)
            yMax = currentWall.point.y;
        if(xMin > currentWall.point.x)
            xMin = currentWall.point.x;
        if(yMin > currentWall.point.y)
            yMin = currentWall.point.y;
        UV.push_back(currentWall.point.x);
        UV.push_back(currentWall.point.y);
        currentWall = walls[currentWall.nextWallIndex];
    }

    float kX = 1.0f/(xMax-xMin);
    float kY = 1.0f/(yMax-yMin);
    for(uint i = 0; i < UV.size()-1; i+=2)
    {
        UV[i] = (UV[i]-xMin)*kX;
        UV[i+1] = (UV[i+1]-yMin)*kY;
    }
    return UV;
}



void xenfa::RenderEngine::setupVertexObjects(GLuint &Vao, GLuint &Vbo, GLuint &VboUV,
                                             const std::vector<float> &vertices,
                                             const std::vector<float> &UV)
{
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);
    // VBO for vertices
    {
        // Generate 1 buffer, put the resulting identifier in Vbo
        glGenBuffers(1, &Vbo);
        // The following commands will talk about our 'Vbo' buffer
        glBindBuffer(GL_ARRAY_BUFFER, Vbo);
        // Give our vertices to OpenGL.
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, Vbo);
        glVertexAttribPointer(
                    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );
    }
    // VBO for UV
    {
        // Generate 1 buffer, put the resulting identifier in Vbo
        glGenBuffers(1, &VboUV);
        // The following commands will talk about our 'Vbo' buffer
        glBindBuffer(GL_ARRAY_BUFFER, VboUV);
        // Give our vertices to OpenGL.
        glBufferData(GL_ARRAY_BUFFER, UV.size()*sizeof(float), &UV[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, VboUV);
        glVertexAttribPointer(
                    1,                  // attribute 1.
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );
    }
    glBindVertexArray(0);
}

void xenfa::RenderEngine::generateWallsVertexBuffers()
{
    wallMeshes.resize(walls.size());
    for(uint sectorIndex = 0; sectorIndex < sectors.size(); sectorIndex++)
    {
        Sector currentSector = sectors[sectorIndex];
        uint currentWallIndex = currentSector.startWall;
        Wall currentWall = walls[currentWallIndex];
        for(uint i = 0; i <= currentSector.numWalls; i++)
        {
            Wall nextWall = walls[currentWall.nextWallIndex];

            std::vector<float> vertices;
            std::vector<float> UV;
            if(currentWall.nextSectorIndex == -1)
            {
                vertices = generateWallVertices(currentWallIndex, sectorIndex);
                UV = generateWallUV(currentWallIndex, sectorIndex);
            }
            else
            {
                vertices = generatePortalVertices(currentWallIndex, sectorIndex);
                UV = generatePortalUV(currentWallIndex, sectorIndex);
            }
            GLuint vertexbuffer;
            GLuint uvBuffer;
            GLuint vertexArrayID;
            setupVertexObjects(vertexArrayID, vertexbuffer, uvBuffer,
                               vertices, UV);

            wallMeshes[currentWallIndex].vertexArray = vertexArrayID;
            wallMeshes[currentWallIndex].vertexBuffer = vertexbuffer;
            wallMeshes[currentWallIndex].numVertices = vertices.size();
            wallMeshes[currentWallIndex].uvBuffer = uvBuffer;
            currentWallIndex = currentWall.nextWallIndex;
            currentWall = nextWall;
        }

    }
}

void xenfa::RenderEngine::generateSectorVertexBuffers()
{
    floorMeshes.resize(sectors.size());
    ceilingMeshes.resize(sectors.size());
    for(uint sectorIndex = 0; sectorIndex < sectors.size(); sectorIndex++)
    {
        std::vector<float> floorVertices = generateSectorVertices(sectorIndex, true);
        std::vector<float> floorUV = generateSectorUV(sectorIndex);

        std::vector<float> ceilingVertices = generateSectorVertices(sectorIndex, false);
        std::vector<float> ceilingUV = generateSectorUV(sectorIndex);

        GLuint vertexArrayID;
        GLuint vertexbuffer;
        GLuint uvBuffer;

        setupVertexObjects(vertexArrayID, vertexbuffer, uvBuffer, floorVertices, floorUV);
        floorMeshes[sectorIndex].vertexArray = vertexArrayID;
        floorMeshes[sectorIndex].vertexBuffer = vertexbuffer;
        floorMeshes[sectorIndex].uvBuffer = uvBuffer;
        floorMeshes[sectorIndex].numVertices = sectors[sectorIndex].numWalls;


        setupVertexObjects(vertexArrayID, vertexbuffer, uvBuffer, ceilingVertices, ceilingUV);
        ceilingMeshes[sectorIndex].vertexArray = vertexArrayID;
        ceilingMeshes[sectorIndex].vertexBuffer = vertexbuffer;
        ceilingMeshes[sectorIndex].uvBuffer = uvBuffer;
        ceilingMeshes[sectorIndex].numVertices = sectors[sectorIndex].numWalls;

    }
}

void xenfa::RenderEngine::render()
{
    camera.lookAt(player.position, glm::vec3(player.angleCos*player.yawSin,
                                             player.angleSin*player.yawSin,
                                             player.yawCos)+player.position, glm::vec3(0.0f, 0.0f, -1.0f));
    camera.update();
    shader.use();
    GLuint MatrixID = glGetUniformLocation(shader.shaderProgram, "VP");
//    GLuint textureID = glGetUniformLocation(shader.shaderProgram, "tex");
    glm::mat4 MVP = camera.getVP();
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

//    glUniform1i(textureID, texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //     1st attribute buffer : vertices
    for(int i = 0; i < wallMeshes.size(); i++)
    {
        const MeshGL &wallGL = wallMeshes[i];
        const Wall &wall = walls[i];
        GLuint textureIndex = textureContainer.getTexture(wall.textureIndex);
        glBindTexture(GL_TEXTURE_2D, textureIndex);
        glBindVertexArray(wallGL.vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, wallGL.numVertices);
    }
    for(uint i = 0; i < floorMeshes.size(); i++)
    {
        const MeshGL &sectorFloor = floorMeshes[i];
        const Sector &sector = sectors[i];
        GLuint textureIndex = textureContainer.getTexture(sector.floorTextureIndex);
        glBindTexture(GL_TEXTURE_2D, textureIndex);
        glBindVertexArray(sectorFloor.vertexArray);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sectorFloor.numVertices);
    }
    for(uint i = 0; i < ceilingMeshes.size(); i++)
    {
        const MeshGL &sectorCeiling = ceilingMeshes[i];
        const Sector &sector = sectors[i];
        GLuint textureIndex = textureContainer.getTexture(sector.ceilingTextureIndex);
        glBindTexture(GL_TEXTURE_2D, textureIndex);
        glBindVertexArray(sectorCeiling.vertexArray);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sectorCeiling.numVertices);
    }
    //    glDisableVertexAttribArray(0);
}
