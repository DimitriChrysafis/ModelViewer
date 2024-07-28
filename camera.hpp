#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera();

    void update(sf::RenderWindow& window, float& angleX, float& angleY, float& zoom);

private:
    float movementSpeed;
    float zoomSpeed;
};

#endif
