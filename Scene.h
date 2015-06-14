#ifndef MIRO_SCENE_H_INCLUDED
#define MIRO_SCENE_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "PointLight.h"
#include "Accel.h"
#include "Environment.h"

class Camera;
class Image;

class Scene
{
public:
	void addObject(Object* pObj)   {m_objects.push_back(pObj);}
	const Objects* objects() const {return &m_objects;}

	void addLight(PointLight* pObj) {m_lights.push_back(pObj);}
	const Lights* lights() const    {return &m_lights;}

    const Accel* accel() const {return &m_accel;}


    void setEnvironment(Environment * environment) {m_environment = environment;}
    const Environment* environment() const {return m_environment;}

	void preCalc();
	void openGL(Camera *cam);

	void raytraceImage(Camera *cam, Image *img);
	bool trace(HitInfo& minHit, const Ray& ray, float tMin = 0.0f, float tMax = MIRO_TMAX) const;

protected:
	Objects m_objects;
	Accel m_accel;
	Lights m_lights;
    Environment * m_environment;
};

extern Scene * g_scene;

#endif // MIRO_SCENE_H_INCLUDED
