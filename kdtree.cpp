#include "kdtree.hpp"

KDTree::KDTree(const std::vector<sf::Vector3f>& points) {
    std::vector<sf::Vector3f> pointsCopy = points;
    root = build(pointsCopy, 0, pointsCopy.size(), 0);
}

KDTree::~KDTree() {
    destroy(root);
}

KDNode* KDTree::build(std::vector<sf::Vector3f>& points, int start, int end, int depth) {
    if (start >= end) return nullptr;

    int axis = depth % 3;
    int medianIdx = start + (end - start) / 2;

    std::nth_element(points.begin() + start, points.begin() + medianIdx, points.begin() + end, [axis](const sf::Vector3f& a, const sf::Vector3f& b) {
        if (axis == 0) return a.x < b.x;
        if (axis == 1) return a.y < b.y;
        return a.z < b.z;
    });

    KDNode* node = new KDNode(points[medianIdx]);
    node->left = build(points, start, medianIdx, depth + 1);
    node->right = build(points, medianIdx + 1, end, depth + 1);

    return node;
}

void KDTree::destroy(KDNode* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

std::vector<sf::Vector3f> KDTree::findNearestNeighbors(const sf::Vector3f& target, int k) {
    auto compare = [](const std::pair<float, sf::Vector3f>& a, const std::pair<float, sf::Vector3f>& b) {
        return a.first < b.first;
    };

    std::priority_queue<std::pair<float, sf::Vector3f>, std::vector<std::pair<float, sf::Vector3f>>, decltype(compare)> best(compare);

    searchNearest(root, target, k, 0, best);

    std::vector<sf::Vector3f> result;
    while (!best.empty()) {
        result.push_back(best.top().second);
        best.pop();
    }

    std::reverse(result.begin(), result.end());
    return result;
}

template <typename Compare>
void KDTree::searchNearest(KDNode* node, const sf::Vector3f& target, int k, int depth, std::priority_queue<std::pair<float, sf::Vector3f>, std::vector<std::pair<float, sf::Vector3f>>, Compare>& best) {
    if (node == nullptr) return;

    float dist = squaredDistance(node->point, target);

    if (best.size() < k || dist < best.top().first) {
        best.push({dist, node->point});
        if (best.size() > k) best.pop();
    }

    int axis = depth % 3;
    float diff = 0;
    if (axis == 0) diff = target.x - node->point.x;
    if (axis == 1) diff = target.y - node->point.y;
    if (axis == 2) diff = target.z - node->point.z;

    KDNode* near = diff < 0 ? node->left : node->right;
    KDNode* far = diff < 0 ? node->right : node->left;

    searchNearest(near, target, k, depth + 1, best);
    if (best.size() < k || std::abs(diff) * std::abs(diff) < best.top().first) {
        searchNearest(far, target, k, depth + 1, best);
    }
}

float KDTree::squaredDistance(const sf::Vector3f& a, const sf::Vector3f& b) const {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}
