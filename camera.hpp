#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera();

    // Update camera based on input
    void update(sf::RenderWindow& window, float& angleX, float& angleY, float& zoom);

private:
    float movementSpeed; // Speed of movement
    float zoomSpeed;     // Speed of zoom
};

#endif // CAMERA_HPP
