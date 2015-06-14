#include "Lambert.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>
#include <random>

Lambert::Lambert(const Vector3 & kd) :
	m_kd(kd)
{
}

Lambert::~Lambert()
{
}

inline double uniform(double a, double b)
{
    return rand() / (RAND_MAX + 1.0) * (b - a) + a;
}

Vector3
Lambert::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
	Vector3 L = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 Ep = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 Ei = Vector3(0.0f, 0.0f, 0.0f);

	const Vector3 viewDir = -ray.d; // d is a unit vector

	const Lights *lightlist = scene.lights();

	// loop over all of the lights
	Lights::const_iterator lightIter;
	for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
	{
		PointLight* pLight = *lightIter;


		Vector3 l = pLight->position() - hit.P;
		// the inverse-squared falloff
		float falloff = l.length2();

		// normalize the light direction
		l /= sqrt(falloff);

        // find whether occlusion exists between intersection point and light
        HitInfo tmpHit = HitInfo();

        Ray tmpRay = Ray(hit.P, l);
        if(scene.accel()->intersect(tmpHit, tmpRay, 0.0f, FLT_MAX))
            continue;

		// get the irradiance
		Vector3 irradiance = (pLight->color() * pLight->wattage()) * std::max(0.0f, dot(hit.N, l)) / (4.0 * PI * falloff);

		Ep += irradiance * (m_kd / PI);

	}

    bool impSamp = false;
    // Normal sampling
    // get irradiance from imge light
    int nSample = 100;
    float r1, r2;
    float theta, phi;
    Ray tmpRay;
    for (int i = 0; i < nSample; ++i)
    {
        // generate random ray

        //r1 = uniform(0.0, 1.0);
        //r2 = uniform(0.0, 1.0);
        r1 = std::min(drand48(), 1.0-epsilon);
        r2 = drand48();
        theta = acos(r1);
        phi = 2.0*PI*r2;
        Vector3 rayD = Vector3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
        tmpRay = Ray(hit.P, rayD);
        HitInfo tmpHit;
        if(!scene.trace(tmpHit, tmpRay))
        {
            if(!impSamp)
                Ei += kd()*std::max(0.0f,dot(hit.N,rayD))*scene.environment()->getColor(rayD);
            else
                Ei += kd()*scene.environment()->getColor(rayD);
        }
    }
    if(!impSamp)
        Ei = Ei * 2.0 * PI / nSample;
    else
        Ei = Ei * PI / nSample;

	return Ep+Ei;
}

