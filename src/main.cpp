#include <iostream>
#include <fstream>

#include "json.h"
#include "vector.h"
#include "world.h"

#include <ArduiPi_OLED_lib.h>
#include <ArduiPi_OLED.h>

#define WIDTH 128
#define HEIGHT 48

ArduiPi_OLED gDisplay;

int main(int argc, const char * argv[]) {
    // Setup display
    if (!gDisplay.init(OLED_I2C_RESET, 3)) {
        std::cerr << "Error: could not initialize display\n";
        return 1;
    }

    gDisplay.begin();
    gDisplay.clearDisplay();
    gDisplay.display();

    //
    // MATH SHIT

    Graphics::BackBuffer backBuffer{WIDTH, HEIGHT};

    World::Camera camera;
    camera.setPosition({3, 3, 3});
    camera.setTarget({0, 0, 0});

    // TODO: Clear this utter mess up pls
    std::ifstream is(argv[1]);
    nlohmann::json j = nlohmann::json::parse(is);

    std::vector<Math::Vector> vertices;
    std::vector<World::Face> faces;

    nlohmann::json jMesh = j["meshes"][0];
    nlohmann::json jVertices = jMesh["vertices"];
    nlohmann::json jIndices = jMesh["indices"];

    int vertexStep = 1;
    int uvCount = jMesh["uvCount"].get<int>();

    switch (uvCount) {
        case 0:
        vertexStep = 6; break;
        case 1:
        vertexStep = 8; break;
        case 2:
        vertexStep = 10; break;
    }

    std::size_t vertexCount = jVertices.size() / vertexStep;
    std::size_t indiceCount = jIndices.size() / 3;

    vertices.reserve(vertexCount);
    faces.reserve(indiceCount);

    for (std::size_t i = 0; i < vertexCount; ++i) {
        float x = jVertices[i * vertexStep].get<float>();
        float y = jVertices[i * vertexStep + 1].get<float>();
        float z = jVertices[i * vertexStep + 2].get<float>();
        vertices.push_back(Math::Vector(x, y, z));
    }

    for (std::size_t i = 0; i < indiceCount; ++i) {
        int a = jIndices[i * 3].get<int>();
        int b = jIndices[i * 3 + 1].get<int>();
        int c = jIndices[i * 3 + 2].get<int>();
        faces.push_back(World::Face{a, b, c});
    }

    // TODO: Write importer class
    // TODO: Improve mesh class

    float scale = std::stof(argv[2]);

    World::Mesh cubeMesh{"Cube", vertices, faces};
    cubeMesh.setPosition({0, 0, 0});
    cubeMesh.setRotation({0, 0, 0});
    cubeMesh.setScale({scale, scale, scale});

    float dx, dy, dz;
    dx = 1;
    dy = -1;
    dz = 1;
    float sx, sy, sz;
    sx = 0.09;
    sy = 0.07;
    sz = 0.05;


    while (true) {
        gDisplay.clearDisplay();

        cubeMesh.rotation().x += 0.1; // pitch
        cubeMesh.rotation().y += 0.125; // yaw
        cubeMesh.rotation().z += 0.15; // roll

        cubeMesh.position().x += dx * sx;
        cubeMesh.position().y += dy * sy;
        cubeMesh.position().z += dz * sz;

        float px, py, pz;
        px = cubeMesh.position().x;
        py = cubeMesh.position().y;
        pz = cubeMesh.position().z;
        if (px <= -1 || px >= 1) { dx *= -1; }
        if (py <= -1 || py >= 1) { dy *= -1; }
        if (pz <= -1 || pz >= 1) { dz *= -1; }

        backBuffer.clear({0, 0, 0, 1});
        camera.render(backBuffer, cubeMesh, {1, 1, 1, 1});

        const std::vector<uint8_t>& buffer = backBuffer.buffer();
        int j = 0;
        for (int i = 0; i < backBuffer.size(); i += 4) {
            float r, g, b, a;
            r = buffer[i];
            g = buffer[i + 1];
            b = buffer[i + 2];
            a = buffer[i + 3];

            uint8_t color = (r == 255 && g == 255 && b == 255 ? WHITE : BLACK);

            int x, y;
            x = j % backBuffer.width();
            y = j / backBuffer.width();

            gDisplay.drawPixel(x, y + 16, color);
            ++j;
        }

        gDisplay.setTextSize(1);
        gDisplay.setTextWrap(0);
        gDisplay.setTextColor(WHITE);
        gDisplay.setCursor(0,0);
        gDisplay.printf("x=%.2f,y=%.2f,z=%.2f", cubeMesh.position().x, cubeMesh.position().y, cubeMesh.position().z);

        gDisplay.display();
        // usleep(100000);
    }

    return 0;
}
