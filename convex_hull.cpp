#include "convex_hull.hpp"
#include <cmath>
#include <limits>
#include <algorithm>
#include <vector>

using namespace std;
using namespace sf;

// Function to compute the cross product of vectors OA and OB
// a positive cross product indicates a counter-clockwise turn
// a negative cross product indicates a clockwise one
// and zero indicates that the points are collinear.
float cross(const Vector3f& O, const Vector3f& A, const Vector3f& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// THIS IS BROKEN HAHAHAHA IT CALCUJLATES THE 2d HULL
vector<Vector3f> calculateConvexHull(const vector<Vector3f>& points) {
    vector<Vector3f> hull;

    // if there are fewer than 3 points, a convex hull is not possible
    // cuz like, triangle???
    if (points.size() < 3) {
        return hull;
    }

    // read the variable name
    Vector3f leftmost = *min_element(points.begin(), points.end(), [](const Vector3f& a, const Vector3f& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    Vector3f current = leftmost;
    do {
        // add the current point to the hull
        hull.push_back(current);
        // start with an arbitrary point as the next candidate
        Vector3f next = points[0];

        for (size_t i = 1; i < points.size(); ++i) {
            // if next is the same as current or cross product indicates a counter-clockwise turn, update next
            if (next == current || cross(current, next, points[i]) > 0) {
                next = points[i];
            }
        }

        // MOVE TO THE NEXT POINT IN THE HULL WHAT ELSE COULD THIS LINE DO
        current = next;
        // continue until we return to the starting point
    } while (current != leftmost);

    return hull;
}
