#include "KdTree.h"
#include "Ray.h"

using namespace std;

void
KdNode::build()
{
    int axis = depth % 3;
    cout << "****************" << endl;
    this->bbox.print();
    cout << "****************" << endl;

    if (depth >= MAX_DEPTH || objects.size() < 10)
    {
        this->leaf = true;
        cout << "leaf depth " << this->depth << " instance size " << this->objects.size() << endl;
        this->leftChild = NULL;
        this->rightChild = NULL;
        return;
    }

    BBox lBbox = BBox();
    BBox rBbox = BBox();
    cout << "split bbox" << endl;
    split(lBbox, rBbox, axis);
    //lBbox.print();
    //rBbox.print();

    Objects lObjs = Objects();
    Objects rObjs = Objects();
    cout << "allocate objs" << endl;
    allocateObjs(lObjs, rObjs, axis);
    cout << "leftObjes size " << lObjs.size() << endl;
    cout << "rightObjes size " << rObjs.size() << endl;

    this->leftChild = new KdNode(lObjs, lBbox, depth+1);
    this->leftChild->build();
    //std::cout << "leftChild bbox" << std::endl;
    //this->leftChild->bbox.print();
    this->rightChild = new KdNode(rObjs, rBbox, depth+1);
    this->rightChild->build();
    //std::cout << "rightChild bbox" << std::endl;
    //this->rightChild->bbox.print();
}

//TODO: should use SAE, now is splited by midpoint
//TODO: by SAE, we can get the greater position to split BBox
//TODO: add position parameter!
void
KdNode::split(BBox & lBbox, BBox & rBbox, int axis)
{
    Vector3 mid = this->bbox.getCenter();
    Vector3 lmax = this->bbox.maxV;
    Vector3 rmin = this->bbox.minV;

    switch (axis)
    {
        case 0:
            cout << "------X axis------" << endl;
            lmax.x = mid.x;
            rmin.x = mid.x;
            break;
        case 1:
            cout << "------Y axis------" << endl;
            lmax.y = mid.y;
            rmin.y = mid.y;
            break;
        case 2:
            cout << "------Z axis------" << endl;
            lmax.z = mid.z;
            rmin.z = mid.z;
            break;
    }
    lBbox.set(this->bbox.minV, lmax);
    rBbox.set(rmin, this->bbox.maxV);
}

//TODO: add split position parameter
void
KdNode::allocateObjs(Objects & lObjs, Objects & rObjs, int axis)
{
    switch (axis)
    {
        case 0:
            for (int i = 0; i < (this->objects).size(); ++i)
            {
                bool left = false;
                bool right = false;
                for(int j = 0; j < ((this->objects)[i])->getVertices().size(); ++j)
                {
                    if((((this->objects)[i])->getVertices()[j]).x < this->bbox.getCenter().x)
                        left = true;
                    else
                        right = true;
                }
                if (left)
                    lObjs.push_back((this->objects)[i]);
                if (right)
                    rObjs.push_back((this->objects)[i]);
                //((this->objects)[i])->getPoint().x < this->bbox.getCenter().x ? lObjs.push_back((this->objects)[i]) : rObjs.push_back((this->objects)[i]);
            }

            break;
        case 1:
            for (int i = 0; i < (this->objects).size(); ++i)
            {
                bool left = false;
                bool right = false;
                for(int j = 0; j < ((this->objects)[i])->getVertices().size(); ++j)
                {
                    if((((this->objects)[i])->getVertices()[j]).y < this->bbox.getCenter().y)
                        left = true;
                    else
                        right = true;
                }
                if (left)
                    lObjs.push_back((this->objects)[i]);
                if (right)
                    rObjs.push_back((this->objects)[i]);
                //((this->objects)[i])->getPoint().y < this->bbox.getCenter().y ? lObjs.push_back((this->objects)[i]) : rObjs.push_back((this->objects)[i]);
            }
            break;
        case 2:
            for (int i = 0; i < (this->objects).size(); ++i)
            {
                bool left = false;
                bool right = false;
                for(int j = 0; j < ((this->objects)[i])->getVertices().size(); ++j)
                {
                    if((((this->objects)[i])->getVertices()[j]).z < this->bbox.getCenter().z)
                        left = true;
                    else
                        right = true;
                }
                if (left)
                    lObjs.push_back((this->objects)[i]);
                if (right)
                    rObjs.push_back((this->objects)[i]);
                //((this->objects)[i])->getPoint().z < this->bbox.getCenter().z ? lObjs.push_back((this->objects)[i]) : rObjs.push_back((this->objects)[i]);
            }
            break;
    }
}

bool
KdNode::isLeaf()
{
    return this->leaf;
}

int
KdNode::getDepth()
{
    return this->depth;
}

BBox&
KdNode::getBBox()
{
    return this->bbox;
}

bool
KdNode::traverse(HitInfo &minHit, const Ray& ray, float tMin, float tMax)
{
    bool hit = false;

    if ( this == NULL)
        return false;

    if(!(this->getBBox().intersect(ray, tMin, tMax)))
    {
        return false;
    }
    // Leaf node
    if(this->isLeaf())
    {
        HitInfo tmpHit;

        Objects objs = this->getObjs();
        for (int i = 0; i < objs.size(); ++i)
        {
            if(objs[i]->intersect(tmpHit, ray, tMin, tMax))
            {
                hit = true;
                if(tmpHit.t < minHit.t)
                {
                    minHit = tmpHit;
                }
            }
        }
        return hit;
    }

    bool left = this->leftChild->traverse(minHit, ray, tMin, tMax);
    bool right = this->rightChild->traverse(minHit, ray, tMin, tMax);
    return left || right;
}


void
KdNode::drawBBox() const
{
    this->bbox.renderGL();
    if (this->leftChild != NULL)
        leftChild->drawBBox();
    if (this->rightChild != NULL)
        rightChild->drawBBox();
}

