#ifndef MIRO_ACCEL_H_INCLUDED
#define MIRO_ACCEL_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "KdTree.h"

class Accel
{
public:
	void build(Objects * objs);
	bool intersect(HitInfo& minHit, const Ray& ray, float tMin = 0.0f, float tMax = MIRO_TMAX) const;
    KdNode * getKdTree() {return kdroot;}

protected:
	Objects * m_objects;
    KdNode * kdroot;

};

#endif // MIRO_ACCEL_H_INCLUDED
