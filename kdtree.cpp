#include "kdtree.hpp"

using namespace sf;
using namespace std;

// constructor to init KDTree with a vector of points
KDTree::KDTree(const std::vector<::Vector3f>& points) {
    // copy the points so u dont mod the OG
    vector<Vector3f> pointsCopy = points;
    root = build(pointsCopy, 0, pointsCopy.size(), 0);
}

// Destructor to clean up all nodes in the KDTree
KDTree::~KDTree() {
    destroy(root); // Recursively delete all nodes
}

KDNode* KDTree::build(vector<Vector3f>& points, int start, int end, int depth) {
    if (start >= end) {
        return nullptr;
        // BASE CASE
    }

    // Determine the axis to split on
    int axis = depth % 3;
    int medianIdx = start + (end - start) / 2;

    // Partitions the points around the median
    nth_element(points.begin() + start, points.begin() + medianIdx, points.begin() + end, [axis](const ::Vector3f& a, const ::Vector3f& b) {
        if (axis == 0) {
            return a.x < b.x;
        }
        if (axis == 1) {
            return a.y < b.y;
        }
        return a.z < b.z;
    });

    // create a new node with the median point and recursively build the left and right subtrees
    KDNode* node = new KDNode(points[medianIdx]);
    node->left = build(points, start, medianIdx, depth + 1);
    node->right = build(points, medianIdx + 1, end, depth + 1);

    return node;
}

// recursively destroys the KDTree
void KDTree::destroy(KDNode* node) {
    if (node == nullptr) {
        return; // BASE CASE
    }

    destroy(node->left);
    destroy(node->right);
    delete node;
}

// this right here is the CORE
// this function finds the k nearest neighbors to the target point
// it uses a priority queue to keep track of the best k points
// it also uses a helper function to recursively search the KDTree
vector<::Vector3f> KDTree::findNearestNeighbors(const Vector3f& target, int k) {
    // Custom comparator to sort by distance in ascending order
    auto compare = [](const pair<float, Vector3f>& a, const pair<float, Vector3f>& b) {
        return a.first < b.first;
    };

    // Priority queue to keep track of the nearest neighbors
    priority_queue<pair<float, Vector3f>, vector<pair<float, Vector3f>>, decltype(compare)> best(compare);

    // Start the nearest neighbor search
    searchNearest(root, target, k, 0, best);

    // collect queieued points into a vector
    vector<Vector3f> result;
    while (!best.empty()) {
        result.push_back(best.top().second); // add the point with the smallest distance
        best.pop(); // remove it after
    }

    reverse(result.begin(), result.end()); // reverse to have closest points first
    // u must reverse it i am forgot many time
    return result;
}

template <typename Compare>
void KDTree::searchNearest(KDNode* node, const ::Vector3f& target, int k, int depth, priority_queue<pair<float, ::Vector3f>, vector<pair<float, Vector3f>>, Compare>& best) {
    if (node == nullptr) {
        return; // Base case: no node to process
    }

    // calculate the distance from the target point to the current node cuz thats important
    float dist = squaredDistance(node->point, target);

    // if this node is among the k closest, add it to the priority queue
    if (best.size() < k || dist < best.top().first) {
        best.push({dist, node->point});
        if (best.size() > k) {
            best.pop(); // remove the farthest neighbor if we have more than k
        }
    }
    // Determine which side of the split to search first
    int axis = depth % 3;
    float diff = 0;
    if (axis == 0) {
        diff = target.x - node->point.x;
    }
    if (axis == 1) {
        diff = target.y - node->point.y;
    }
    if (axis == 2) {
        diff = target.z - node->point.z;
    }

    KDNode* near = diff < 0 ? node->left : node->right;
    KDNode* far = diff < 0 ? node->right : node->left;

    // recursively search the side closer to the target
    searchNearest(near, target, k, depth + 1, best);


    // if the other side could potentially contain closer points, search it as well
    if (best.size() < k || abs(diff) * abs(diff) < best.top().first) {
        searchNearest(far, target, k, depth + 1, best);
    }
}

// !!! WHAT DOES THIS DO???!!!
float KDTree::squaredDistance(const Vector3f& a, const Vector3f& b) const {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}
