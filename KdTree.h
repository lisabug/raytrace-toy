#ifndef MIRO_KDTREE_H_INCLUDED
#define MIRO_KDTREE_H_INCLUDED

#include "Vector3.h"
#include "Accel.h"
#include "BBox.h"

#define MAX_DEPTH 21

class KdNode
{
public:
    KdNode(const Objects & objs, const BBox & boundbox, int d): objects(objs), bbox(boundbox), depth(d), leaf(false){}
    ~KdNode() {}

    void build();
    // 划分空间
    void split(BBox & lBbox, BBox & rBbox, int axis);
    void allocateObjs(Objects & lObjs, Objects & rObjs, int axis);

    bool isLeaf();
    int getDepth();
    BBox & getBBox();
    KdNode * getLeft() {return leftChild;}
    KdNode * getRight() {return rightChild;}
    Objects & getObjs() {return objects;}
    bool traverse(HitInfo & minHit, const Ray& ray, float tMin, float tMax);
    void drawBBox() const;


protected:
    Objects objects;
    bool leaf;
    KdNode* leftChild;
    KdNode* rightChild;
    int depth;
    BBox bbox;

};

#endif
// Objects' definition
// typedef std::vector<Object*> Objects;
