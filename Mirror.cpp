#include "Mirror.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

#define MAX_RELF_DEPTH 8

Mirror::Mirror(const Vector3 & ks): m_ks(ks) {}
Mirror::~Mirror() {}

Vector3
Mirror::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);

    // calculate reflection ray
    Vector3 tmpD = Vector3();
    tmpD = -2*(dot(ray.d, hit.N)*hit.N) + ray.d;
    tmpD = tmpD / sqrt(tmpD.length2());
    Ray reflRay = Ray(hit.P, tmpD, ray.reflDepth+1);

    HitInfo tmpHit;
    if (reflRay.reflDepth < MAX_RELF_DEPTH && scene.trace(tmpHit, reflRay))
        L += ks() * tmpHit.material->shade(reflRay, tmpHit, scene);
    else if (reflRay.reflDepth < MAX_RELF_DEPTH)
        L += scene.environment()->getColor(reflRay.d);
    return L;
}
