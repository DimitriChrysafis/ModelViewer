#include <SFML/Graphics.hpp>
#include "model_viewer.hpp"
#include "camera.hpp" // Include the camera header


int main() {
    if (!loadOBJ("/Users/dimitrichrysafis/Documents/GitHub/ViewerlModel/demos/xyzrgb_dragon.obj")) {
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(640, 480), "3D Thing");
    window.setFramerateLimit(60);


    sf::Vector3f offset(0.0f, 0.0f, 0.0f);

    float angleX = 0.0f;
    float angleY = 0.0f;
    float zoom = 1.0f;
    bool wireframe = false;
    bool showBoundingBox = false;
    bool showConvexHull = false;

    Camera camera;

    sf::FloatRect boundingBox = calculateBoundingBox(vertices, angleX, angleY, zoom);
    zoom = std::min(window.getSize().x / boundingBox.width, window.getSize().y / boundingBox.height) * 0.9f;

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

        // Recalculate bounding box to adjust zoom if needed
        if (showBoundingBox) {
            boundingBox = calculateBoundingBox(vertices, angleX, angleY, zoom);
            zoom = std::min(window.getSize().x / boundingBox.width, window.getSize().y / boundingBox.height) * 0.9f;
        }

        window.clear();
        if (wireframe) {
            drawModelWireframe(window, angleX, angleY, zoom, offset);
        } else {
            drawModelSolid(window, angleX, angleY, zoom, offset);
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