#include "kdtree.hpp"

KDTree::KDTree(const std::vector<sf::Vector3f>& points) {
    std::vector<sf::Vector3f> pointsCopy = points;
    root = build(pointsCopy, 0);
}

KDTree::~KDTree() {
    destroy(root);
}

KDNode* KDTree::build(std::vector<sf::Vector3f>& points, int depth) {
    if (points.empty()) return nullptr;

    int axis = depth % 3;
    std::sort(points.begin(), points.end(), [axis](const sf::Vector3f& a, const sf::Vector3f& b) {
        if (axis == 0) return a.x < b.x;
        if (axis == 1) return a.y < b.y;
        return a.z < b.z;
    });

    size_t medianIdx = points.size() / 2;
    KDNode* node = new KDNode(points[medianIdx]);

    std::vector<sf::Vector3f> leftPoints(points.begin(), points.begin() + medianIdx);
    std::vector<sf::Vector3f> rightPoints(points.begin() + medianIdx + 1, points.end());

    node->left = build(leftPoints, depth + 1);
    node->right = build(rightPoints, depth + 1);

    return node;
}

void KDTree::destroy(KDNode* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

std::vector<sf::Vector3f> KDTree::findNearestNeighbors(const sf::Vector3f& target, int k) {
    std::vector<std::pair<float, sf::Vector3f>> best;
    searchNearest(root, target, k, 0, best);

    std::vector<sf::Vector3f> result;
    for (const auto& pair : best) {
        result.push_back(pair.second);
    }

    return result;
}

void KDTree::searchNearest(KDNode* node, const sf::Vector3f& target, int k, int depth, std::vector<std::pair<float, sf::Vector3f>>& best) {
    if (node == nullptr) return;

    float dist = std::sqrt(std::pow(node->point.x - target.x, 2) + std::pow(node->point.y - target.y, 2) + std::pow(node->point.z - target.z, 2));
    if (best.size() < k || dist < best.back().first) {
        best.push_back({dist, node->point});
        std::sort(best.begin(), best.end(), [](const std::pair<float, sf::Vector3f>& a, const std::pair<float, sf::Vector3f>& b) {
            return a.first < b.first;
        });
        if (best.size() > k) best.pop_back();
    }

    int axis = depth % 3;
    float diff = 0;
    if (axis == 0) diff = target.x - node->point.x;
    if (axis == 1) diff = target.y - node->point.y;
    if (axis == 2) diff = target.z - node->point.z;

    KDNode* near = diff < 0 ? node->left : node->right;
    KDNode* far = diff < 0 ? node->right : node->left;

    searchNearest(near, target, k, depth + 1, best);
    if (best.size() < k || std::abs(diff) < best.back().first) {
        searchNearest(far, target, k, depth + 1, best);
    }
}
