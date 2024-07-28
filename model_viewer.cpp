#include "model_viewer.hpp"
#include <cmath>
#include <limits>
#include <fstream>
#include <sstream>
#include "kdtree.hpp"
#include <iostream>
#include "convex_hull.hpp"

using namespace std;
using namespace sf;

/**
 * @brief calculates the euclidean distance between two 3D points. i never actually use this haha
 */
float distance(const Vector3f& a, const Vector3f& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

vector<CustomVertex> vertices;
vector<Face> faces;

// load the obj and store the vertices and faces in the vectors
bool loadOBJ(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open the file!" << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string type;
        iss >> type;
        if (type == "v") {
            CustomVertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (type == "f") {
            Face face;
            string vertex;
            while (iss >> vertex) {
                size_t pos = vertex.find('/');
                int index = stoi(vertex.substr(0, pos)) - 1;
                face.vertices.push_back(index);
            }
            faces.push_back(face);
        }
    }

    return true;
}

Vector3f rotateVertex(const Vector3f& vertex, float angleX, float angleY) {
    Vector3f rotatedVertex = vertex;

    // Rotate around y axis, it says y axis just yeah
    float cosAngleY = cos(angleY);
    float sinAngleY = sin(angleY);
    rotatedVertex.x = vertex.x * cosAngleY - vertex.z * sinAngleY;
    rotatedVertex.z = vertex.x * sinAngleY + vertex.z * cosAngleY;

    // ooh wanna guess what this rotates about?
    // triginometry is fun
    float cosAngleX = cos(angleX);
    float sinAngleX = sin(angleX);
    rotatedVertex.y = vertex.y * cosAngleX - rotatedVertex.z * sinAngleX;
    rotatedVertex.z = vertex.y * sinAngleX + rotatedVertex.z * cosAngleX;

    return rotatedVertex;
}

/**
 * @brief Calculates the bounding box of the model.
 *
 * rhe bounding box is calculated after applying rotation and zoom to each vertex.
 * you guys know that old usaco problem where its like bessie and some billboard
 * https://usaco.org/index.php?page=viewproblem2&cpid=759
 * this is like that but in 3d
 *
 * @return The bounding box as a FloatRect.
 */
FloatRect calculateBoundingBox(const vector<CustomVertex>& vertices, float angleX, float angleY, float zoom) {
    FloatRect boundingBox;
    boundingBox.left = numeric_limits<float>::max();
    boundingBox.top = numeric_limits<float>::max();
    boundingBox.width = numeric_limits<float>::lowest();
    boundingBox.height = numeric_limits<float>::lowest();

    for (const auto& vertex : vertices) {
        // Apply rotation and zoom to the vertex (VERY IMPRTANT)
        Vector3f rotatedVertex = rotateVertex({vertex.x * zoom, vertex.y * zoom, vertex.z * zoom}, angleX, angleY);
        float x = 320 + rotatedVertex.x * 100;
        float y = 240 - rotatedVertex.y * 100;

        if (x < boundingBox.left) boundingBox.left = x;
        if (y < boundingBox.top) boundingBox.top = y;
        if (x > boundingBox.width) boundingBox.width = x;
        if (y > boundingBox.height) boundingBox.height = y;
    }

    boundingBox.width -= boundingBox.left;
    boundingBox.height -= boundingBox.top;

    return boundingBox;
}

// wanna guess what this does? van goh. art
void drawBoundingBox(RenderWindow& window, const FloatRect& boundingBox) {
    VertexArray lines(LinesStrip, 5);

    lines[0].position = Vector2f(boundingBox.left, boundingBox.top);
    lines[1].position = Vector2f(boundingBox.left + boundingBox.width, boundingBox.top);
    lines[2].position = Vector2f(boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height);
    lines[3].position = Vector2f(boundingBox.left, boundingBox.top + boundingBox.height);
    lines[4].position = Vector2f(boundingBox.left, boundingBox.top);

    for (int i = 0; i < 5; ++i) {
        lines[i].color = Color::Green;
    }

    window.draw(lines);
}

// i put the logic for the convex hull in a separate file but this draws it
void drawConvexHull(RenderWindow& window, float angleX, float angleY, float zoom) {
    vector<Vector3f> modelVertices;
    for (const auto& vertex : vertices) {
        modelVertices.push_back({vertex.x * zoom, vertex.y * zoom, vertex.z * zoom});
    }

    auto convexHull = calculateConvexHull(modelVertices);

    if (convexHull.size() < 3) return;

    VertexArray lines(Lines);
    for (size_t i = 0; i < convexHull.size(); ++i) {
        Vector3f rv0 = rotateVertex(convexHull[i], angleX, angleY);
        Vector3f rv1 = rotateVertex(convexHull[(i + 1) % convexHull.size()], angleX, angleY);

        lines.append(Vertex(Vector2f(320 + rv0.x * 100, 240 - rv0.y * 100), Color::Blue));
        lines.append(Vertex(Vector2f(320 + rv1.x * 100, 240 - rv1.y * 100), Color::Blue));
    }
    window.draw(lines);
}

// draws the model in wireframe mode (easy
void drawModelWireframe(RenderWindow& window, float angleX, float angleY, float zoom) {
    VertexArray lines(Lines);
    for (const auto& face : faces) {
        for (size_t i = 0; i < face.vertices.size(); ++i) {
            const auto& v0 = vertices[face.vertices[i]];
            const auto& v1 = vertices[face.vertices[(i + 1) % face.vertices.size()]];

            Vector3f rv0 = rotateVertex({v0.x * zoom, v0.y * zoom, v0.z * zoom}, angleX, angleY);
            Vector3f rv1 = rotateVertex({v1.x * zoom, v1.y * zoom, v1.z * zoom}, angleX, angleY);

            lines.append(Vertex(Vector2f(320 + rv0.x * 100, 240 - rv0.y * 100), Color::White));
            lines.append(Vertex(Vector2f(320 + rv1.x * 100, 240 - rv1.y * 100), Color::White));
        }
    }
    window.draw(lines);
}

// draws the model in solid mode (also easy)
void drawModelSolid(RenderWindow& window, float angleX, float angleY, float zoom) {
    VertexArray triangles(Triangles);
    for (const auto& face : faces) {
        if (face.vertices.size() < 3) continue;
        for (size_t i = 1; i < face.vertices.size() - 1; ++i) {
            const auto& v0 = vertices[face.vertices[0]];
            const auto& v1 = vertices[face.vertices[i]];
            const auto& v2 = vertices[face.vertices[i + 1]];

            Vector3f rv0 = rotateVertex({v0.x * zoom, v0.y * zoom, v0.z * zoom}, angleX, angleY);
            Vector3f rv1 = rotateVertex({v1.x * zoom, v1.y * zoom, v1.z * zoom}, angleX, angleY);
            Vector3f rv2 = rotateVertex({v2.x * zoom, v2.y * zoom, v2.z * zoom}, angleX, angleY);

            triangles.append(Vertex(Vector2f(320 + rv0.x * 100, 240 - rv0.y * 100), Color::Red));
            triangles.append(Vertex(Vector2f(320 + rv1.x * 100, 240 - rv1.y * 100), Color::Red));
            triangles.append(Vertex(Vector2f(320 + rv2.x * 100, 240 - rv2.y * 100), Color::Red));
        }
    }
    window.draw(triangles);
}

/*
 * I AM NOT COMMENTING ON THE DRAWING OF POINTS I QUIT
 */
void drawModelPoints(RenderWindow& window, float angleX, float angleY, float zoom) {
    VertexArray points(Points);
    for (const auto& vertex : vertices) {
        Vector3f rv = rotateVertex({vertex.x * zoom, vertex.y * zoom, vertex.z * zoom}, angleX, angleY);
        points.append(Vertex(Vector2f(320 + rv.x * 100, 240 - rv.y * 100), Color::White));
    }
    window.draw(points);
}

void drawNearestNeighbors(RenderWindow& window, float angleX, float angleY, float zoom) {
    vector<Vector3f> modelVertices;
    for (const auto& vertex : vertices) {
        modelVertices.push_back({vertex.x * zoom, vertex.y * zoom, vertex.z * zoom});
    }

    KDTree kdtree(modelVertices);

    for (const auto& vertex : modelVertices) {
        auto neighbors = kdtree.findNearestNeighbors(vertex, 5);

        VertexArray lines(Lines);
        for (const auto& neighbor : neighbors) {
            Vector3f rv0 = rotateVertex(vertex, angleX, angleY);
            Vector3f rv1 = rotateVertex(neighbor, angleX, angleY);

            lines.append(Vertex(Vector2f(320 + rv0.x * 100, 240 - rv0.y * 100), Color::Cyan));
            lines.append(Vertex(Vector2f(320 + rv1.x * 100, 240 - rv1.y * 100), Color::Cyan));
        }
        window.draw(lines);
    }
}
