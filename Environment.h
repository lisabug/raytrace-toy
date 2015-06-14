#ifndef MIRO_ENVIRONMENT_H_INCLUDED
#define MIRO_ENVIRONMENT_H_INCLUDED

#include "Vector3.h"
#include "miro.h"

class Environment
{
public:
    Environment(Vector3 * lights, int width, int height);
    ~Environment();
    void setLights(Vector3 * lights) {m_lights = lights;}
    void setHeight(int height) {m_height = height;}
    void setWidth(int width) {m_width = width;}
    virtual Vector3 getColor(const Vector3 & d) const ;
    virtual Vector3 test(int i, int j) const;

protected:
    Vector3 * m_lights;
    int m_width;
    int m_height;
};

#endif
