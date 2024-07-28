#ifndef MODEL_VIEWER_HPP
#define MODEL_VIEWER_HPP

#include <SFML/Graphics.hpp>
#include <vector>

/**
 * @brief Represents a 3D vertex.
 */
struct CustomVertex {
    float x, y, z;
};

/**
 * @brief Represents a face of the model consisting of vertices indices.
 */
struct Face {
    std::vector<int> vertices;
};

extern std::vector<CustomVertex> vertices;
extern std::vector<Face> faces;

bool loadOBJ(const std::string& filename);
sf::FloatRect calculateBoundingBox(const std::vector<CustomVertex>& vertices, float angleX, float angleY, float zoom);
void drawBoundingBox(sf::RenderWindow& window, const sf::FloatRect& boundingBox);
void drawConvexHull(sf::RenderWindow& window, float angleX, float angleY, float zoom);
void drawModelWireframe(sf::RenderWindow& window, float angleX, float angleY, float zoom);
void drawModelSolid(sf::RenderWindow& window, float angleX, float angleY, float zoom);
void drawModelPoints(sf::RenderWindow& window, float angleX, float angleY, float zoom);
void drawNearestNeighbors(sf::RenderWindow& window, float angleX, float angleY, float zoom);

#endif // MODEL_VIEWER_HPP
