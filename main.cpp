#include <SFML/Graphics.hpp>
#include <iostream>
#include "model_viewer.hpp"
#include "camera.hpp"
#include "kdtree.hpp"
#include <cmath>

using namespace std;
using namespace sf;

int main() {
    // oh yeah change ur file path lol
    if (!loadOBJ("/Users/dimitrichrysafis/Documents/GitHub/ModelViewer/demos/cow.obj")) {
        return -1;
    }
    // i am not adding comments
    RenderWindow win(VideoMode(640, 480), "View in 3d");
    win.setFramerateLimit(60);

    // i am not adding comments to this code becasue if you dont get it you should not be here
    float xRot = 0.0f;
    float yRot = 0.0f;
    float zoom = 1.0f;
    bool wireframeMode = false;
    bool showBox = false;
    bool showHull = false;
    bool pointMode = false;
    bool showNeighbors = false;
    bool spinX = false;
    bool spinY = false;
    Camera cam;

    // just add a box
    FloatRect boundingBox = calculateBoundingBox(vertices, xRot, yRot, 1.0f);
    float zoomX = win.getSize().x / boundingBox.width;
    float zoomY = win.getSize().y / boundingBox.height;
    zoom = min(zoomX, zoomY) * 0.5f;

    while (win.isOpen()) {
        Event event;
        while (win.pollEvent(event)) {
            if (event.type == Event::Closed) {
                win.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Z) {
                    wireframeMode = !wireframeMode;
                }
                if (event.key.code == Keyboard::B) {
                    showBox = !showBox;
                }
                if (event.key.code == Keyboard::C) {
                    showHull = !showHull;
                }
                if (event.key.code == Keyboard::T) {
                    pointMode = !pointMode;
                }
                if (event.key.code == Keyboard::N) {
                    showNeighbors = !showNeighbors;
                }
                if (event.key.code == Keyboard::Y) {
                    spinX = !spinX;
                }
                if (event.key.code == Keyboard::X) {
                    spinY = !spinY;
                }
            }
            if (event.type == Event::MouseWheelScrolled) {
                zoom += event.mouseWheelScroll.delta * 0.1f;
                if (zoom < 0.1f) zoom = 0.1f;
            }
        }

        cam.update(win, xRot, yRot, zoom);


        if (spinX) {
            xRot += 0.1f;
        }
        if (spinY) {
            yRot += 0.1f;
        }

        if (showBox) {
            boundingBox = calculateBoundingBox(vertices, xRot, yRot, zoom);
        }

        win.clear();
        if (pointMode) {
            drawModelPoints(win, xRot, yRot, zoom);
        } else if (wireframeMode) {
            drawModelWireframe(win, xRot, yRot, zoom);
        } else {
            drawModelSolid(win, xRot, yRot, zoom);
        }
        if (showBox) {
            drawBoundingBox(win, boundingBox);
        }
        if (showHull) {
            drawConvexHull(win, xRot, yRot, zoom);
        }
        if (showNeighbors) {
            drawNearestNeighbors(win, xRot, yRot, zoom);
        }
        win.display();
    }

    return 0;
}
