#ifndef MODEL_VIEWER_HPP
#define MODEL_VIEWER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

struct Vertex {
    float x, y, z;
};

struct Face {
    std::vector<int> vertices;
};

extern std::vector<Vertex> vertices;
extern std::vector<Face> faces;

bool loadOBJ(const std::string& filename);

sf::Vector3f rotateVertex(const sf::Vector3f& vertex, float angleX, float angleY);

// Function to apply vertical squish
void applyVerticalSquish(float factor);

void drawModelWireframe(sf::RenderWindow& window, float angleX, float angleY, float zoom);
void drawModelSolid(sf::RenderWindow& window, float angleX, float angleY, float zoom);
void drawModelPoints(sf::RenderWindow& window, float angleX, float angleY, float zoom);

sf::FloatRect calculateBoundingBox(const std::vector<Vertex>& vertices, float angleX, float angleY, float zoom);
void drawBoundingBox(sf::RenderWindow& window, const sf::FloatRect& boundingBox);

void drawConvexHull(sf::RenderWindow& window, float angleX, float angleY, float zoom);
void drawNearestNeighbors(sf::RenderWindow& window, float angleX, float angleY, float zoom);

std::vector<sf::Vector3f> calculateConvexHull(const std::vector<sf::Vector3f>& points);

#endif // MODEL_VIEWER_HPP
