#include "camera.hpp"

Camera::Camera()
        : movementSpeed(0.1f), zoomSpeed(0.2f) // Initialize movement and zoom speeds
{}

void Camera::update(sf::RenderWindow& window, float& angleX, float& angleY, float& zoom) {
    // Camera rotation with arrow keys
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        angleY -= movementSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        angleY += movementSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        angleX -= movementSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        angleX += movementSpeed;
    }


}
