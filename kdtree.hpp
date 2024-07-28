#ifndef KDTREE_HPP
#define KDTREE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace sf;
using namespace std;

struct KDNode {
    Vector3f point;
    KDNode* left;
    KDNode* right;

    KDNode(const Vector3f& pt) : point(pt), left(nullptr), right(nullptr) {}
};

class KDTree {
public:
    KDTree(const vector<Vector3f>& points);
    ~KDTree();

    vector<Vector3f> findNearestNeighbors(const Vector3f& target, int k);

private:
    KDNode* build(vector<Vector3f>& points, int start, int end, int depth);
    void destroy(KDNode* node);

    template <typename Compare>
    void searchNearest(KDNode* node, const Vector3f& target, int k, int depth, priority_queue<pair<float, Vector3f>, vector<pair<float, Vector3f>>, Compare>& best);

    float squaredDistance(const Vector3f& a, const Vector3f& b) const;

    KDNode* root;
};

#endif