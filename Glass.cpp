#include "Glass.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

#define MAX_RELF_DEPTH 8

Glass::Glass(const Vector3 & kt, float index): m_kt(kt), m_index(index) {}
Glass::~Glass() {}

Vector3
Glass::shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const
{
    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);


    // Calculate reflection ray
    Vector3 tmpReflD = Vector3();
    tmpReflD = -2*(dot(ray.d,hit.N)*hit.N)+ray.d;
    //tmpReflD = tmpReflD / sqrt(tmpReflD.length2());
    tmpReflD.normalize();
    Ray reflRay = Ray(hit.P, tmpReflD, ray.reflDepth+1);
    HitInfo tmpReflHit;

    // Calculate refraction ray
    Vector3 n = Vector3(hit.N);
    Vector3 w = Vector3(ray.d);
    float eta1, eta2;
    bool exist = false;
    if(dot(w,n)<0)
    {
        //std::cout << "enter" << std::endl;
        eta1 = 1.0f;
        eta2 = this->index();
        if((1-pow(eta1/eta2,2)*(1-pow(dot(w,n),2))) < 0)
        {

            if (reflRay.reflDepth < MAX_RELF_DEPTH && scene.trace(tmpReflHit, reflRay))
                L += tmpReflHit.material->shade(reflRay, tmpReflHit, scene);
            else if (reflRay.reflDepth < MAX_RELF_DEPTH)
                L += scene.environment()->getColor(reflRay.d);
            return L;
        }
    }
    else
    {
        //std::cout << "out" << std::endl;
        eta1 = this->index();
        eta2 = 1.0f;
        n = -n;
        exist = true;
    }

    Vector3 tmpD;
    tmpD = eta1/eta2 * (w - dot(w,n)*n) - sqrt(1-pow(eta1/eta2,2)*(1-pow(dot(w,n),2)))*n;
    if(exist) n = -n;

    tmpD.normalize();
    Ray refrRay = Ray(hit.P, tmpD);
    HitInfo tmpRefrHit;
    if(dot(w,n) < 0)
    {
        double cosi, coso;
        cosi = dot(ray.d, n) / (sqrt(ray.d.length2()) * sqrt(n.length2()));
        coso = dot(tmpD, n) / (sqrt(tmpD.length2()) * sqrt(n.length2()));
        double ps = (eta1*cosi-eta2*coso)/(eta1*cosi+eta2*coso);
        double pt = (eta1*coso-eta2*cosi)/(eta1*coso+eta2*cosi);
        double R = (ps*ps + pt*pt)/2.0;

        if (reflRay.reflDepth < MAX_RELF_DEPTH && scene.trace(tmpReflHit, reflRay))
            L += R*tmpReflHit.material->shade(reflRay, tmpReflHit, scene);
        else if (reflRay.reflDepth < MAX_RELF_DEPTH)
            L += R*scene.environment()->getColor(reflRay.d);
        if (scene.trace(tmpRefrHit, refrRay))
            L += (1-R)*tmpRefrHit.material->shade(refrRay, tmpRefrHit, scene);
        else if (refrRay.reflDepth < MAX_RELF_DEPTH)
            L += (1-R)*scene.environment()->getColor(refrRay.d);
    }
    else
    {
        if (scene.trace(tmpRefrHit, refrRay))
            L += tmpRefrHit.material->shade(refrRay, tmpRefrHit, scene);
        else
            L += scene.environment()->getColor(refrRay.d);
    }

    return L;


}
