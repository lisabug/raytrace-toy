#include "Accel.h"
#include "Ray.h"
#include "Console.h"
#include "BBox.h"

void
Accel::build(Objects * objs)
{
	// construct the bounding volume hierarchy
	m_objects = objs;

    //TODO: add timer
    BBox initBBox = BBox();
    initBBox.expandBBox(*m_objects);
    //initBBox.print();
    kdroot = new KdNode(*m_objects, initBBox, 0);
    kdroot->build();
}


bool
Accel::intersect(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
	// Here you would need to traverse the acceleration data structure to perform ray-intersection
	// acceleration. For now we just intersect every object.

	bool hit = false;
	//HitInfo tempMinHit;
	minHit.t = MIRO_TMAX;

    /*
	for (size_t i = 0; i < m_objects->size(); ++i)
	{
		if ((*m_objects)[i]->intersect(tempMinHit, ray, tMin, tMax))
		{
			hit = true;
			if (tempMinHit.t < minHit.t)
				minHit = tempMinHit;
		}
	}
    */
    hit = kdroot->traverse(minHit, ray, tMin, tMax);

	return hit;
}

