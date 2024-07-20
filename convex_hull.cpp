#include "convex_hull.hpp"
#include <cmath>
#include <limits>
#include <algorithm>
#include <unordered_set>

struct Face {
    sf::Vector3f vertices[3];
    sf::Vector3f normal;
};

sf::Vector3f crossProduct(const sf::Vector3f& a, const sf::Vector3f& b) {
    return sf::Vector3f(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
    );
}

float dotProduct(const sf::Vector3f& a, const sf::Vector3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float pointPlaneDistance(const sf::Vector3f& point, const sf::Vector3f& planeNormal, const sf::Vector3f& planePoint) {
    return dotProduct(planeNormal, point - planePoint);
}

sf::Vector3f computeNormal(const sf::Vector3f& p1, const sf::Vector3f& p2, const sf::Vector3f& p3) {
    return crossProduct(p2 - p1, p3 - p1);
}

bool pointInTriangle(const sf::Vector3f& pt, const sf::Vector3f& v1, const sf::Vector3f& v2, const sf::Vector3f& v3) {
    // Compute vectors
    sf::Vector3f v0 = v3 - v1;
    sf::Vector3f v1v = v2 - v1;
    sf::Vector3f v2v = pt - v1;

    // Compute dot products
    float dot00 = dotProduct(v0, v0);
    float dot01 = dotProduct(v0, v1v);
    float dot02 = dotProduct(v0, v2v);
    float dot11 = dotProduct(v1v, v1v);
    float dot12 = dotProduct(v1v, v2v);

    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}

void addFace(std::vector<Face>& faces, const sf::Vector3f& p1, const sf::Vector3f& p2, const sf::Vector3f& p3) {
    Face face;
    face.vertices[0] = p1;
    face.vertices[1] = p2;
    face.vertices[2] = p3;
    face.normal = computeNormal(p1, p2, p3);
    faces.push_back(face);
}

void buildHull(std::vector<sf::Vector3f>& hull, const std::vector<sf::Vector3f>& points, std::vector<Face>& faces) {
    std::unordered_set<int> remainingIndices;
    for (int i = 0; i < points.size(); ++i) {
        remainingIndices.insert(i);
    }

    while (!remainingIndices.empty()) {
        float maxDistance = -std::numeric_limits<float>::max();
        int farthestPointIndex = -1;
        Face farthestFace;

        for (const Face& face : faces) {
            for (int pointIndex : remainingIndices) {
                float distance = pointPlaneDistance(points[pointIndex], face.normal, face.vertices[0]);
                if (distance > maxDistance) {
                    maxDistance = distance;
                    farthestPointIndex = pointIndex;
                    farthestFace = face;
                }
            }
        }

        if (farthestPointIndex == -1) {
            break;
        }

        sf::Vector3f farthestPoint = points[farthestPointIndex];
        hull.push_back(farthestPoint);

        std::vector<Face> newFaces;
        for (const Face& face : faces) {
            if (pointPlaneDistance(farthestPoint, face.normal, face.vertices[0]) > 0) {
                addFace(newFaces, face.vertices[0], face.vertices[1], farthestPoint);
                addFace(newFaces, face.vertices[1], face.vertices[2], farthestPoint);
                addFace(newFaces, face.vertices[2], face.vertices[0], farthestPoint);
            } else {
                newFaces.push_back(face);
            }
        }

        faces = newFaces;
        remainingIndices.erase(farthestPointIndex);
    }
}

std::vector<sf::Vector3f> calculateConvexHull(const std::vector<sf::Vector3f>& points) {
    std::vector<sf::Vector3f> hull;

    if (points.size() < 4) {
        return hull;
    }

    // Find initial tetrahedron
    auto minmaxX = std::minmax_element(points.begin(), points.end(), [](const sf::Vector3f& a, const sf::Vector3f& b) { return a.x < b.x; });
    auto minmaxY = std::minmax_element(points.begin(), points.end(), [](const sf::Vector3f& a, const sf::Vector3f& b) { return a.y < b.y; });
    auto minmaxZ = std::minmax_element(points.begin(), points.end(), [](const sf::Vector3f& a, const sf::Vector3f& b) { return a.z < b.z; });

    sf::Vector3f p1 = *minmaxX.first;
    sf::Vector3f p2 = *minmaxX.second;
    sf::Vector3f p3 = *minmaxY.first;
    sf::Vector3f p4 = *minmaxZ.first;

    hull.push_back(p1);
    hull.push_back(p2);
    hull.push_back(p3);
    hull.push_back(p4);

    std::vector<Face> faces;
    addFace(faces, p1, p2, p3);
    addFace(faces, p1, p2, p4);
    addFace(faces, p1, p3, p4);
    addFace(faces, p2, p3, p4);

    buildHull(hull, points, faces);

    return hull;
}
