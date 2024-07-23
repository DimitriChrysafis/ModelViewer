#include <SFML/Graphics.hpp>
#include "model_viewer.hpp"
#include "camera.hpp"
#include "kdtree.hpp"
#include <cmath>

int main() {
    if (!loadOBJ("/Users/dimitrichrysafis/Documents/GitHub/ViewerlModel/demos/cloud.obj")) {
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(640, 480), "3D Thing");
    window.setFramerateLimit(60);

    float angleX = 0.0f;
    float angleY = 0.0f;
    float zoom = 1.0f;
    bool wireframe = false;
    bool showBoundingBox = false;
    bool showConvexHull = false;
    bool pointsMode = false;
    bool showNearestNeighbors = false;

    Camera camera;

    sf::FloatRect boundingBox = calculateBoundingBox(vertices, angleX, angleY, 1.0f);
    float initialZoomX = window.getSize().x / (boundingBox.width );
    float initialZoomY = window.getSize().y / (boundingBox.height );
    zoom = std::min(initialZoomX, initialZoomY) * 0.5f; // Adjust the multiplier as needed


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Z) {
                    wireframe = !wireframe;
                }
                if (event.key.code == sf::Keyboard::B) {
                    showBoundingBox = !showBoundingBox;
                }
                if (event.key.code == sf::Keyboard::C) {
                    showConvexHull = !showConvexHull;
                }
                if (event.key.code == sf::Keyboard::T) {
                    pointsMode = !pointsMode;
                }
                if (event.key.code == sf::Keyboard::N) {
                    showNearestNeighbors = !showNearestNeighbors;
                }
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                zoom += event.mouseWheelScroll.delta * 0.1f;
                if (zoom < 0.1f) zoom = 0.1f;
            }
        }

        camera.update(window, angleX, angleY, zoom);

        if (showBoundingBox) {
            boundingBox = calculateBoundingBox(vertices, angleX, angleY, zoom);
        }

        window.clear();
        if (pointsMode) {
            drawModelPoints(window, angleX, angleY, zoom);
        } else if (wireframe) {
            drawModelWireframe(window, angleX, angleY, zoom);
        } else {
            drawModelSolid(window, angleX, angleY, zoom);
        }
        if (showBoundingBox) {
            drawBoundingBox(window, boundingBox);
        }
        if (showConvexHull) {
            drawConvexHull(window, angleX, angleY, zoom);
        }
        if (showNearestNeighbors) {
            drawNearestNeighbors(window, angleX, angleY, zoom);
        }
        window.display();
    }

    return 0;
}