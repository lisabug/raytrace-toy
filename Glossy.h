#ifndef MIRO_GLOSSY_H_INCLUDED
#define MIRO_GLOSSY_H_INCLUDED

#include "Material.h"

class Glossy : public Material
{
public:
    Glossy(const Vector3 & kd = Vector3(0.5f), const Vector3 & ks = Vector3(0.5f), float g = 3.0f);
    virtual ~Glossy();

    const Vector3 & kd() const {return m_kd;}
    const Vector3 & ks() const {return m_ks;}

    void setKd(const Vector3 &kd) {m_kd = kd;}
    void setKs(const Vector3 &ks) {m_ks = ks;}
    void setG(float g) {m_g = g;}

    virtual void preCalc() {}
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const;

protected:
    Vector3 m_kd;
    Vector3 m_ks;
    float m_g;
};

#endif
