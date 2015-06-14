#include "Environment.h"

Environment::Environment(Vector3 * lights, int width, int height):
    m_lights(lights), m_width(width), m_height(height) {}
Environment::~Environment()
{
    delete m_lights;
}


Vector3
Environment::getColor(const Vector3 & d) const
{
    // the situation d is NaN
    if(d.x!=d.x||d.y!=d.y||d.z!=d.z)
        return Vector3(0.0f,0.0f,0.0f);
    // Need calculate cos
    double u, v, r;

    r = 1.0/PI*acos(d.z)/sqrt(d.x*d.x+d.y*d.y);
    u = d.x * r;
    v = d.y * r;

    //std::cout << "u:" << u << "v:" << v << std::endl;;
    int img_x = (int)((u+1.0)/2.0*m_width);
    int img_y = (int)((v+1.0)/2.0*m_height);
    if(img_x == m_width) std::cout<<"x:yes"<<std::endl;
    if(img_y == m_height) std::cout<<"y:yes"<<std::endl;
    //std::cout << "d:" << d << std::endl;
    //std::cout << "x:" << img_x << "y:" << img_y << std::endl;;
    // might have problem


    if(m_lights != NULL)
        return m_lights[(img_y)*m_width+img_x];
    else
        return Vector3(0.0f,0.0f,0.0f);

    //return Vector3(1,1,1);
}

Vector3
Environment::test(int i, int j) const
{
    i = (int)(i / 512.0 * m_width);
    j = (int)(j / 512.0 * m_height);
    return m_lights[j*m_width + i];
}




