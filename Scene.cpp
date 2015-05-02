#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"
#include "Triangle.h"

Scene * g_scene = 0;

void
Scene::openGL(Camera *cam)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam->drawGL();

	// draw objects
	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		m_objects[i]->renderGL();
	}
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    KdNode * root = this->m_accel.getKdTree();
    //root->getBBox().renderGL();
    root->drawBBox();


	glutSwapBuffers();
}

void
Scene::preCalc()
{
	Objects::iterator it;
	for (it = m_objects.begin(); it != m_objects.end(); it++)
	{
		Object* pObject = *it;
		pObject->preCalc();
	}
	Lights::iterator lit;
	for (lit = m_lights.begin(); lit != m_lights.end(); lit++)
	{
		PointLight* pLight = *lit;
		pLight->preCalc();
	}

	m_accel.build(&m_objects);

}

void
Scene::raytraceImage(Camera *cam, Image *img)
{
	Ray ray;
	HitInfo hitInfo;
	Vector3 shadeResult;
    int rayNumber = 0;

	// loop over all pixels in the image
	for (int j = 0; j < img->height(); ++j)
	{
		for (int i = 0; i < img->width(); ++i)
		{
			ray = cam->eyeRay(i, j, img->width(), img->height());
            rayNumber++;
			if (trace(hitInfo, ray))
			{
				shadeResult = hitInfo.material->shade(ray, hitInfo, *this);
				img->setPixel(i, j, shadeResult);
			}
		}
		img->drawScanline(j);
		glFinish();
		printf("Rendering Progress: %.3f%%\r", j/float(img->height())*100.0f);
		fflush(stdout);
	}

	printf("Rendering Progress: 100.000%\n");
    //output the intersection number
    printf("Number of rays: %d\n", rayNumber);
    printf("Number of ray traversal: %d\n", KdNode::getTraverseNumbers());
    printf("Number of triangle intersection: %d\n", Triangle::getIntersectionNumber());
    KdNode::resetTraverseNumbers();
    Triangle::resertIntersectionNumber();  // reset the intersection number for next rendering.
	debug("done Raytracing!\n");
}

bool
Scene::trace(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
	return m_accel.intersect(minHit, ray, tMin, tMax);
}
