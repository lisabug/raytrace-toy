#include "BBox.h"
#include <vector>
#include "Ray.h"

Vector3
BBox::getCenter()
{
    return (minV + maxV)/2;
}

void
BBox::set(const Vector3 & p, const Vector3 & q)
{
    minV = p;
    maxV = q;
}

void
BBox::expand(const Vector3 & p)
{
    if(minV.x > p.x)
        minV.x = p.x;
    if(maxV.x < p.x)
        maxV.x = p.x;
    if(minV.y > p.y)
        minV.y = p.y;
    if(maxV.y < p.y)
        maxV.y = p.y;
    if(minV.z > p.z)
        minV.z = p.z;
    if(maxV.z < p.z)
        maxV.z = p.z;
}

void
BBox::expandBBox(const Objects & objs)
{
    if (objs.size() == 0)
        return;
    for (int i = 0; i < objs.size(); ++i)
    {
        std::vector<Vector3> vertices = objs[i]->getVertices();
        for (int j = 0; j < vertices.size(); ++j)
            this->expand(vertices[j]);
    }
}

void
BBox::print() const
{
    std::cout<< "bbox min " << minV << " | max " << maxV << std::endl;

}

static inline void drawLine(const std::vector<Vector3> & v, int i, int j);
inline void drawLine(const std::vector<Vector3> & v, int i, int j)
{
    glVertex3f(v[i].x, v[i].y, v[i].z);
    glVertex3f(v[j].x, v[j].y, v[j].z);
}

void
BBox::renderGL() const
{
    glBegin(GL_LINES);

    std::vector<Vector3> v(8);

    Vector3 whl = maxV - minV;

    v[0] = minV + Vector3(0,0,1) * whl.z;
    v[1] = maxV - Vector3(0,1,0) * whl.y;
    v[2] = maxV - Vector3(1,0,0) * whl.x;
    v[3] = maxV;
    v[4] = minV;
    v[5] = minV + Vector3(1,0,0) * whl.x;
    v[6] = minV + Vector3(0,1,0) * whl.y;
    v[7] = maxV - Vector3(0,0,1) * whl.z;
    //for(int i = 0; i < 8; ++i)
    //   std::cout << v[i] << std::endl;

    drawLine(v, 1, 0);
    drawLine(v, 2, 0);
    drawLine(v, 3, 1);
    drawLine(v, 3, 2);
    drawLine(v, 4, 0);
    drawLine(v, 5, 1);
    drawLine(v, 5, 4);
    drawLine(v, 6, 2);
    drawLine(v, 6, 4);
    drawLine(v, 7, 3);
    drawLine(v, 7, 5);
    drawLine(v, 7, 6);

    glEnd();
}

bool
BBox::intersect(const Ray & ray, float tMin, float tMax)
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    if (ray.d.x >= 0)
    {
        tmin = (minV.x - ray.o.x) / ray.d.x;
        tmax = (maxV.x - ray.o.x) / ray.d.x;
    }
    else
    {
        tmin = (maxV.x - ray.o.x) / ray.d.x;
        tmax = (minV.x - ray.o.x) / ray.d.x;
    }
    if (ray.d.y >= 0)
    {
        tymin = (minV.y - ray.o.y) / ray.d.y;
        tymax = (maxV.y - ray.o.y) / ray.d.y;
    }
    else
    {
        tymin = (maxV.y - ray.o.y) / ray.d.y;
        tymax = (minV.y - ray.o.y) / ray.d.y;
    }
    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if(tymin > tmin)
        tmin = tymin;
    if(tymax < tmax)
        tmax =tymax;
    if (ray.d.z >= 0)
    {
        tzmin = (minV.z - ray.o.z) / ray.d.z;
        tzmax = (maxV.z - ray.o.z) / ray.d.z;
    }
    else
    {
        tzmin = (maxV.z - ray.o.z) / ray.d.z;
        tzmax = (minV.z - ray.o.z) / ray.d.z;
    }
    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
    //std::cout << "true" << std::endl;
    return ((tmin < tMax) && (tmax > tMin+epsilon));
    //return true;

}
