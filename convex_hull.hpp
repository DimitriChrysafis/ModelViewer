#ifndef CONVEX_HULL_HPP
#define CONVEX_HULL_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <set>

std::vector<sf::Vector3f> calculateConvexHull(const std::vector<sf::Vector3f>& points);

#endif // CONVEX_HULL_HPP
