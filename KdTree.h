#ifndef MIRO_KDTREE_H_INCLUDED
#define MIRO_KDTREE_H_INCLUDED

#include "Vector3.h"
#include "Accel.h"
#include "BBox.h"

#define MAX_DEPTH 21
#define KT 15
#define KI 20
#define SAHN 10

class KdNode
{
public:
    KdNode() {}
    KdNode(const Objects & objs, const BBox & boundbox, int d);
    ~KdNode() {}

    void build();
    // 划分空间
    void split(BBox & lBbox, BBox & rBbox, const int axis);
    // split for sah
    void split(BBox & lBbox, BBox & rBbox, const int axis, const float splitPosition);
    void allocateObjs(Objects & lObjs, Objects & rObjs, int axis);
    void allocateObjs(Objects & lObjs, Objects & rObjs, int axis, float splitPosition);

    bool isLeaf();
    int getDepth();
    BBox & getBBox();
    KdNode * getLeft() {return leftChild;}
    KdNode * getRight() {return rightChild;}
    Objects & getObjs() {return objects;}
    bool calcSAH(Objects & lObjs, Objects & rObjs, const int axis, float & splitPosition);
    bool traverse(HitInfo & minHit, const Ray& ray, float tMin, float tMax);
    void drawBBox() const;
    static int getNodeNumbers();
    static int getLeafNumbers();
    static int getTraverseNumbers();
    static void resetTraverseNumbers();



protected:
    Objects objects;
    bool leaf;
    KdNode* leftChild;
    KdNode* rightChild;
    int depth;
    BBox bbox;
    //count nodes;
    static int nodeNumbers;
    static int leafNumbers;
    static int traverseNumbers;

};

#endif
// Objects' definition
// typedef std::vector<Object*> Objects;
