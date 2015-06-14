#include "Glossy.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

Glossy::Glossy(const Vector3 &kd, const Vector3 &ks, float g):
    m_kd(kd), m_ks(ks), m_g(g)
{}
Glossy::~Glossy() {}

Vector3
Glossy::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);

    const Lights *lightlist = scene.lights();
    const Vector3 viewDir = -ray.d;

    Lights::const_iterator lightIter;
    for(lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
    {
        PointLight* pLight = *lightIter;

        Vector3 l = pLight->position() - hit.P;
        float falloff = l.length2();
        l /= sqrt(falloff);
        // find whether occlusion exists between intersenction point and lightsource
        HitInfo tmpHit;
        Ray tmpRay = Ray(hit.P, l);
        if(scene.accel()->intersect(tmpHit, tmpRay, 0.0f, FLT_MAX))
            continue;
        // get diffuse irradiance
        Vector3 irradiance = (pLight->color() * pLight->wattage() * std::max(0.0f, dot(hit.N, l))) / (4.0*PI*falloff);
        L += irradiance * (m_kd / PI);

        // evaluate glossy
        Vector3 h = l + viewDir;
        h /= sqrt(h.length2());
        float NdotH = dot(hit.N, h);
        irradiance = (pLight->color()*pLight->wattage()*pow(std::max(0.0f,NdotH), m_g)) / (4.0*PI*falloff);
        L += irradiance * m_ks;
    }
}
