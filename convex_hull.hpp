#ifndef CONVEX_HULL_HPP
#define CONVEX_HULL_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <algorithm>
using namespace sf;
using namespace std;
vector<Vector3f> calculateConvexHull(const vector<Vector3f>& points);

#endif
