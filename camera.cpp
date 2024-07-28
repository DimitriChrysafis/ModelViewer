#include "camera.hpp"
using namespace sf;
Camera::Camera()
        : movementSpeed(0.1f), zoomSpeed(0.2f)
{}
// this is so basic im not bothering to add comments
void Camera::update(RenderWindow& window, float& angleX, float& angleY, float& zoom) {
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        angleY -= movementSpeed;
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        angleY += movementSpeed;
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        angleX -= movementSpeed;
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        angleX += movementSpeed;
    }


}
