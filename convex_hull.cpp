#include "convex_hull.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

float cross(const sf::Vector3f& O, const sf::Vector3f& A, const sf::Vector3f& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

std::vector<sf::Vector3f> calculateConvexHull(const std::vector<sf::Vector3f>& points) {
    std::vector<sf::Vector3f> hull;

    if (points.size() < 3) {
        return hull;
    }

    // Find the leftmost point
    sf::Vector3f leftmost = *std::min_element(points.begin(), points.end(), [](const sf::Vector3f& a, const sf::Vector3f& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    sf::Vector3f current = leftmost;
    do {
        hull.push_back(current);
        sf::Vector3f next = points[0];

        for (size_t i = 1; i < points.size(); ++i) {
            if (next == current || cross(current, next, points[i]) > 0) {
                next = points[i];
            }
        }

        current = next;
    } while (current != leftmost);

    return hull;
}
