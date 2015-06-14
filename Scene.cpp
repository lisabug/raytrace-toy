#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"
#include "Triangle.h"
#include "Vector3.h"

Scene * g_scene = 0;

inline Vector3 lWhite(Vector3 origin);
inline Vector3 lWhite(Vector3 origin)
{
    float threshold = 200.f;
    float x = origin.x > threshold? 255.0f : origin.x;
    float y = origin.y > threshold? 255.0f : origin.y;
    float z = origin.z > threshold? 255.0f : origin.z;
    return Vector3(x,y,z);
}

inline Vector3 toneMapping(Vector3 origin, bool gamma);
inline Vector3 toneMapping(Vector3 origin, bool gamma)
{
    float g = 1.8;
    Vector3 tmpVec = origin/(Vector3(1.0f)+origin);
    if(gamma)
    {
        tmpVec.x = pow(tmpVec.x, 1.0/g);
        tmpVec.y = pow(tmpVec.y, 1.0/g);
        tmpVec.z = pow(tmpVec.z, 1.0/g);
    }
    else
        return tmpVec;
}


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
	Vector3 shadeResult = Vector3(0.0f);
    int rayNumber = 0;
    //Should use MC here?

    if(cam->lens())
    {
        // Utilize center direction to calculate cosine
        Vector3 centerD = cam->eyeRay(img->width()/2, img->height()/2, img->width(), img->height()).d;
        float cosine;
        Vector3 focalPlanePoint;
        Vector3 lensPlanePoint;
        Ray lensRay;
        int lensPointNumber = 5;
        float focusedDistance = 8.0; // distance from lens to focused object

        float setFilmDistance = focusedDistance*cam->focalDistance()/(focusedDistance-cam->focalDistance());
        //std::cout << setFilmDistance << std::endl;
        cam->setDistance(setFilmDistance);

        // loop over all pixels in the image
        for (int j = 0; j < img->height(); ++j)
        {
            for (int i = 0; i < img->width(); ++i)
            {
                ray = cam->eyeRay(i, j, img->width(), img->height());
                cosine = dot(ray.d,centerD);
                focalPlanePoint = Vector3(ray.o + focusedDistance/cosine*ray.d);
                // Generate random point in lens plane
                for (int k = 0; k < lensPointNumber; ++k)
                {
                    lensPlanePoint = cam->randLensPoint();
                    lensRay = Ray(lensPlanePoint, (focalPlanePoint-lensPlanePoint).normalized());
                    //rayNumber++;
                    if (trace(hitInfo, lensRay))
                    {
                        shadeResult += hitInfo.material->shade(lensRay, hitInfo, *this);
                    }
                    else
                    {
                        shadeResult += this->environment()->getColor(lensRay.d);
                        //shadeResult = this->environment()->test(i,j);
                    }
                }
                shadeResult = shadeResult / (float)lensPointNumber;
                shadeResult = toneMapping(shadeResult, true);
                img->setPixel(i, j, shadeResult);
                shadeResult.set(0);
            }
            img->drawScanline(j);
            glFinish();
            printf("Rendering Progress: %.3f%%\r", j/float(img->height())*100.0f);
            fflush(stdout);
        }
    }
    else
    {
        // loop over all pixels in the image
        for (int j = 0; j < img->height(); ++j)
        {
            for (int i = 0; i < img->width(); ++i)
            {
                ray = cam->eyeRay(i, j, img->width(), img->height());
                    //rayNumber++;
                if (trace(hitInfo, ray))
                {
                    shadeResult = hitInfo.material->shade(ray, hitInfo, *this);
                    shadeResult = toneMapping(shadeResult, true);
                }
                else
                {
                    shadeResult = this->environment()->getColor(ray.d);
                    shadeResult = toneMapping(shadeResult, true);
                }
                img->setPixel(i, j, shadeResult);
            }
            img->drawScanline(j);
            glFinish();
            printf("Rendering Progress: %.3f%%\r", j/float(img->height())*100.0f);
            fflush(stdout);
        }

    }
	printf("Rendering Progress: 100.000%\n");
    //output the intersection number
    /*
    printf("Number of rays: %d\n", rayNumber);
    printf("Number of ray traversal: %d\n", KdNode::getTraverseNumbers());
    printf("Number of triangle intersection: %d\n", Triangle::getIntersectionNumber());
    */
    KdNode::resetTraverseNumbers();
    Triangle::resertIntersectionNumber();  // reset the intersection number for next rendering.
	debug("done Raytracing!\n");
}

bool
Scene::trace(HitInfo& minHit, const Ray& ray, float tMin, float tMax) const
{
	return m_accel.intersect(minHit, ray, tMin, tMax);
}
