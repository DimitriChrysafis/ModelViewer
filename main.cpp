#include <SFML/Graphics.hpp>
#include "model_viewer.hpp"
#include "camera.hpp" // Include the camera header
#include <cmath>

int main() {
    if (!loadOBJ("/Users/dimitrichrysafis/Documents/GitHub/ViewerlModel/demos/cow.obj")) {
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

    Camera camera; // Create an instance of Camera

    // Calculate initial bounding box and determine initial zoom level
    sf::FloatRect boundingBox = calculateBoundingBox(vertices, angleX, angleY, 1.0f);
    float initialZoomX = window.getSize().x / (boundingBox.width + 40); // Adding some margin
    float initialZoomY = window.getSize().y / (boundingBox.height + 40); // Adding some margin
    zoom = std::min(initialZoomX, initialZoomY);

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
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                zoom += event.mouseWheelScroll.delta * 0.1f;
                if (zoom < 0.1f) zoom = 0.1f;
            }
        }

        // Update camera based on input
        camera.update(window, angleX, angleY, zoom);

        // Calculate the bounding box if needed
        if (showBoundingBox) {
            boundingBox = calculateBoundingBox(vertices, angleX, angleY, zoom);
        }

        window.clear();
        if (wireframe) {
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
        window.display();
    }

    return 0;
}
