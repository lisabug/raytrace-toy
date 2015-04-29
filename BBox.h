#ifndef MIRO_BBOX_H_INCLUDED
#define MIRO_BBOX_H_INCLUDED

#include "Vector3.h"
#include "Object.h"

class BBox
{
public:
    BBox () : minV(0.0f), maxV(0.0f) {}
    ~BBox () {}

    Vector3 getCenter();
    void set(const Vector3 & p, const Vector3 & q);
    void expandBBox(const Objects & objs);
    void expand(const Vector3 & p);
    void print() const;
    void renderGL() const;

    //TODO: intersect
    bool intersect(const Ray & ray, float tMin, float tMax);

    Vector3 minV, maxV;
};


#endif
