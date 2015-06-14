#ifndef MIRO_GLASS_H_INCLUDED
#define MIRO_GLASS_H_INCLUDED

#include "Material.h"

class Glass : public Material
{
public:
    Glass(const Vector3 & kt = Vector3(1), float index = 1.0f);
    virtual ~Glass();

    const Vector3 & kt() const {return m_kt;}
    const float index() const {return m_index;}

    void setKt(const Vector3 & kt) {m_kt = kt;}

    virtual void preCalc() {}
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene) const;

protected:
    Vector3 m_kt;
    float m_index;
};

#endif
