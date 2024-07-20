#ifndef MODEL_VIEWER_HPP
#define MODEL_VIEWER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

// Structure for a vertex
struct Vertex {
    float x, y, z;
};

// Structure for a face
struct Face {
    std::vector<int> vertices;
};

// Global variables to hold the vertices and faces of the model
extern std::vector<Vertex> vertices;
extern std::vector<Face> faces;

// Function to load OBJ file
bool loadOBJ(const std::string& filename);

// Function to rotate a vertex
sf::Vector3f rotateVertex(const sf::Vector3f& vertex, float angleX, float angleY);

// Functions to draw the model
void drawModelWireframe(sf::RenderWindow& window, float angleX, float angleY, float zoom);
void drawModelSolid(sf::RenderWindow& window, float angleX, float angleY, float zoom);

sf::FloatRect calculateBoundingBox(const std::vector<Vertex>& vertices, float angleX, float angleY, float zoom);
void drawBoundingBox(sf::RenderWindow& window, const sf::FloatRect& boundingBox);


void drawConvexHull(sf::RenderWindow& window, float angleX, float angleY, float zoom);

std::vector<sf::Vector3f> calculateConvexHull(const std::vector<sf::Vector3f>& points);

#endif // MODEL_VIEWER_HPP
