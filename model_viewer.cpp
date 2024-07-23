#include "convex_hull.hpp"
#include "model_viewer.hpp"
#include "kdtree.hpp"

float distance(const sf::Vector3f& a, const sf::Vector3f& b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2) + std::pow(a.z - b.z, 2));
}
std::vector<Vertex> vertices;
std::vector<Face> faces;

bool loadOBJ(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "v") {
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (type == "f") {
            Face face;
            std::string vertex;
            while (iss >> vertex) {
                size_t pos = vertex.find('/');
                int index = std::stoi(vertex.substr(0, pos)) - 1;
                face.vertices.push_back(index);
            }
            faces.push_back(face);
        }
    }

    return true;
}


sf::Vector3f rotateVertex(const sf::Vector3f& vertex, float angleX, float angleY) {
    sf::Vector3f rotatedVertex = vertex;

    // Rotate around Y axis
    float cosAngleY = cos(angleY);
    float sinAngleY = sin(angleY);
    rotatedVertex.x = vertex.x * cosAngleY - vertex.z * sinAngleY;
    rotatedVertex.z = vertex.x * sinAngleY + vertex.z * cosAngleY;

    // Rotate around X axis
    float cosAngleX = cos(angleX);
    float sinAngleX = sin(angleX);
    rotatedVertex.y = vertex.y * cosAngleX - rotatedVertex.z * sinAngleX;
    rotatedVertex.z = vertex.y * sinAngleX + rotatedVertex.z * cosAngleX;

    return rotatedVertex;
}

sf::FloatRect calculateBoundingBox(const std::vector<Vertex>& vertices, float angleX, float angleY, float zoom) {
    // Initialize bounding box with extreme values
    sf::FloatRect boundingBox;
    boundingBox.left = std::numeric_limits<float>::max();
    boundingBox.top = std::numeric_limits<float>::max();
    boundingBox.width = std::numeric_limits<float>::lowest();
    boundingBox.height = std::numeric_limits<float>::lowest();

    for (const auto& vertex : vertices) {
        // Apply rotation and zoom to the vertex
        sf::Vector3f rotatedVertex = rotateVertex({vertex.x * zoom, vertex.y * zoom, vertex.z * zoom}, angleX, angleY);
        float x = 320 + rotatedVertex.x * 100;
        float y = 240 - rotatedVertex.y * 100;

        // Update bounding box dimensions
        if (x < boundingBox.left) boundingBox.left = x;
        if (y < boundingBox.top) boundingBox.top = y;
        if (x > boundingBox.width) boundingBox.width = x;
        if (y > boundingBox.height) boundingBox.height = y;
    }

    // Calculate width and height of the bounding box
    boundingBox.width -= boundingBox.left;
    boundingBox.height -= boundingBox.top;

    return boundingBox;
}


void drawBoundingBox(sf::RenderWindow& window, const sf::FloatRect& boundingBox) {
    sf::VertexArray lines(sf::LinesStrip, 5);

    lines[0].position = sf::Vector2f(boundingBox.left, boundingBox.top);
    lines[1].position = sf::Vector2f(boundingBox.left + boundingBox.width, boundingBox.top);
    lines[2].position = sf::Vector2f(boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height);
    lines[3].position = sf::Vector2f(boundingBox.left, boundingBox.top + boundingBox.height);
    lines[4].position = sf::Vector2f(boundingBox.left, boundingBox.top);

    for (int i = 0; i < 5; ++i) {
        lines[i].color = sf::Color::Green;
    }

    window.draw(lines);
}

void drawConvexHull(sf::RenderWindow& window, float angleX, float angleY, float zoom) {
    std::vector<sf::Vector3f> modelVertices;
    for (const auto& vertex : vertices) {
        modelVertices.push_back({ vertex.x * zoom, vertex.y * zoom, vertex.z * zoom });
    }

    auto convexHull = calculateConvexHull(modelVertices);

    if (convexHull.size() < 3) return;

    sf::VertexArray lines(sf::Lines);
    for (size_t i = 0; i < convexHull.size(); ++i) {
        sf::Vector3f rv0 = rotateVertex(convexHull[i], angleX, angleY);
        sf::Vector3f rv1 = rotateVertex(convexHull[(i + 1) % convexHull.size()], angleX, angleY);

        lines.append(sf::Vertex(sf::Vector2f(320 + rv0.x * 100, 240 - rv0.y * 100), sf::Color::Blue));
        lines.append(sf::Vertex(sf::Vector2f(320 + rv1.x * 100, 240 - rv1.y * 100), sf::Color::Blue));
    }
    window.draw(lines);
}


void drawModelWireframe(sf::RenderWindow& window, float angleX, float angleY, float zoom) {
    sf::VertexArray lines(sf::Lines);
    for (const auto& face : faces) {
        for (size_t i = 0; i < face.vertices.size(); ++i) {
            const auto& v0 = vertices[face.vertices[i]];
            const auto& v1 = vertices[face.vertices[(i + 1) % face.vertices.size()]];

            sf::Vector3f rv0 = rotateVertex({ v0.x * zoom, v0.y * zoom, v0.z * zoom }, angleX, angleY);
            sf::Vector3f rv1 = rotateVertex({ v1.x * zoom, v1.y * zoom, v1.z * zoom }, angleX, angleY);

            lines.append(sf::Vertex(sf::Vector2f(320 + rv0.x * 100, 240 - rv0.y * 100), sf::Color::White));
            lines.append(sf::Vertex(sf::Vector2f(320 + rv1.x * 100, 240 - rv1.y * 100), sf::Color::White));
        }
    }
    window.draw(lines);
}

void drawModelSolid(sf::RenderWindow& window, float angleX, float angleY, float zoom) {
    sf::VertexArray triangles(sf::Triangles);
    for (const auto& face : faces) {
        if (face.vertices.size() < 3) continue;
        for (size_t i = 1; i < face.vertices.size() - 1; ++i) {
            const auto& v0 = vertices[face.vertices[0]];
            const auto& v1 = vertices[face.vertices[i]];
            const auto& v2 = vertices[face.vertices[i + 1]];

            sf::Vector3f rv0 = rotateVertex({ v0.x * zoom, v0.y * zoom, v0.z * zoom }, angleX, angleY);
            sf::Vector3f rv1 = rotateVertex({ v1.x * zoom, v1.y * zoom, v1.z * zoom }, angleX, angleY);
            sf::Vector3f rv2 = rotateVertex({ v2.x * zoom, v2.y * zoom, v2.z * zoom }, angleX, angleY);

            triangles.append(sf::Vertex(sf::Vector2f(320 + rv0.x * 100, 240 - rv0.y * 100), sf::Color::Red));
            triangles.append(sf::Vertex(sf::Vector2f(320 + rv1.x * 100, 240 - rv1.y * 100), sf::Color::Red));
            triangles.append(sf::Vertex(sf::Vector2f(320 + rv2.x * 100, 240 - rv2.y * 100), sf::Color::Red));
        }
    }
    window.draw(triangles);
}

void drawModelPoints(sf::RenderWindow& window, float angleX, float angleY, float zoom) {
    sf::VertexArray points(sf::Points);
    for (const auto& vertex : vertices) {
        sf::Vector3f rv = rotateVertex({ vertex.x * zoom, vertex.y * zoom, vertex.z * zoom }, angleX, angleY);
        points.append(sf::Vertex(sf::Vector2f(320 + rv.x * 100, 240 - rv.y * 100), sf::Color::White));
    }
    window.draw(points);
}

void drawNearestNeighbors(sf::RenderWindow& window, float angleX, float angleY, float zoom) {
    std::vector<sf::Vector3f> modelVertices;
    for (const auto& vertex : vertices) {
        modelVertices.push_back({ vertex.x * zoom, vertex.y * zoom, vertex.z * zoom });
    }

    KDTree kdtree(modelVertices);

    for (const auto& vertex : modelVertices) {
        auto neighbors = kdtree.findNearestNeighbors(vertex, 5);

        sf::VertexArray lines(sf::Lines);
        for (const auto& neighbor : neighbors) {
            sf::Vector3f rv0 = rotateVertex(vertex, angleX, angleY);
            sf::Vector3f rv1 = rotateVertex(neighbor, angleX, angleY);

            lines.append(sf::Vertex(sf::Vector2f(320 + rv0.x * 100, 240 - rv0.y * 100), sf::Color::Cyan));
            lines.append(sf::Vertex(sf::Vector2f(320 + rv1.x * 100, 240 - rv1.y * 100), sf::Color::Cyan));
        }
        window.draw(lines);
    }
}