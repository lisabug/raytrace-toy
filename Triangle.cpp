#include "Triangle.h"
#include "TriangleMesh.h"
#include "Ray.h"

int Triangle::intersectionNumber = 0;

Triangle::Triangle(TriangleMesh * m, unsigned int i) :
	m_mesh(m), m_index(i)
{
}


Triangle::~Triangle()
{
}

int
Triangle::getIntersectionNumber()
{
    return intersectionNumber;
}

void
Triangle::resertIntersectionNumber()
{
    intersectionNumber = 0;
}

void
Triangle::addIntersectionNumber()
{
    intersectionNumber++;
}

void
Triangle::renderGL()
{
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

	glBegin(GL_TRIANGLES);
		glVertex3f(v0.x, v0.y, v0.z);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
	glEnd();
}

Vector3
Triangle::getPoint() const
{
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

    return (v0 + v1 + v2) / 3;
}

std::vector<Vector3>
Triangle::getVertices() const
{
    std::vector<Vector3> vertices;
    TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
	const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
	const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
	const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);

    return vertices;
}



bool
Triangle::intersect(HitInfo& result, const Ray& r,float tMin, float tMax)
{
    TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
    const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
    const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
    const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle

    // init
    float delta = dot(cross(v0-v1, v0-v2), r.d);
    float delta1 = dot(cross(v0-r.o, v0-v2), r.d);
    float delta2 = dot(cross(v0-v1, v0-r.o), r.d);
    float delta3 = dot(cross(v0-v1, v0-v2), v0-r.o);

    float beta = delta1 / delta;
    float gamma = delta2 / delta;
    float t = delta3 / delta;

    if ((beta < 0) || (beta > 1) || (gamma < 0) || (gamma > 1) || ((1-beta-gamma) < 0) || ((1-beta-gamma) > 1) || t < tMin+epsilon || t > tMax)
        return false;

    result.t = t;
    result.P = r.o + result.t * r.d;

    TriangleMesh::TupleI3 tin3 = m_mesh->nIndices()[m_index];
    result.N = m_mesh->normals()[tin3.x] * (1-beta-gamma) + m_mesh->normals()[tin3.y] * beta + m_mesh->normals()[tin3.z] * gamma;
    result.N.normalize();
    result.material = this->m_material;

    this->addIntersectionNumber();

	return true;
}
