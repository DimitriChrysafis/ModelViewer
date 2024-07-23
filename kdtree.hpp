#ifndef KDTREE_HPP
#define KDTREE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

struct KDNode {
    sf::Vector3f point;
    KDNode* left;
    KDNode* right;

    KDNode(const sf::Vector3f& pt) : point(pt), left(nullptr), right(nullptr) {}
};

class KDTree {
public:
    KDTree(const std::vector<sf::Vector3f>& points);
    ~KDTree();

    std::vector<sf::Vector3f> findNearestNeighbors(const sf::Vector3f& target, int k);

private:
    KDNode* build(std::vector<sf::Vector3f>& points, int start, int end, int depth);
    void destroy(KDNode* node);

    template <typename Compare>
    void searchNearest(KDNode* node, const sf::Vector3f& target, int k, int depth, std::priority_queue<std::pair<float, sf::Vector3f>, std::vector<std::pair<float, sf::Vector3f>>, Compare>& best);

    float squaredDistance(const sf::Vector3f& a, const sf::Vector3f& b) const;

    KDNode* root;
};

#endif // KDTREE_HPP
