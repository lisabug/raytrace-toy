#ifndef MIRO_MIRROR_H_INCLUDED
#define MIRO_MIRROR_H_INCLUDED

#include "Material.h"

class Mirror : public Material
{
public:
    Mirror(const Vector3 & ks = Vector3(0.5));
    virtual ~Mirror();

	const Vector3 & ks() const {return m_ks;}

	void setKs(const Vector3 & ks) {m_ks = ks;}

    virtual void preCalc() {}
    virtual Vector3 shade(const Ray& ray, const HitInfo& info, const Scene& scene) const;

protected:
    Vector3 m_ks;
};

#endif
