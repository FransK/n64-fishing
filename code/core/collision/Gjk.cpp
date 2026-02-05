#include "Gjk.h"

using namespace Collision;

Vector3 *Simplex::addPoint(Vector3 *aPoint, Vector3 *bPoint)
{
    if (nPoints == MaxSimplexSize)
    {
        // SHOULD never happen, but just in case
        return 0;
    }

    int index = nPoints;

    objectAPoint[index] = *aPoint;
    points[index] = objectAPoint[index] - *bPoint;
    ++nPoints;

    return &points[index];
}

int Simplex::check(Vector3 *nextDirection)
{
    Vector3 &lastAdded = points[nPoints - 1];
    Vector3 aToOrigin = -lastAdded;

    if (nPoints == 2)
    {
        Vector3 lastAddedToOther = points[0] - lastAdded;
        *nextDirection = tripleProduct(lastAddedToOther, aToOrigin, lastAddedToOther);

        if (nextDirection->magSqrd() <= 0.0000001f)
        {
            *nextDirection = perp(lastAddedToOther);
        }

        return 0;
    }
    else if (nPoints == 3)
    {
        Vector3 ab = points[1] - lastAdded;
        Vector3 ac = points[0] - lastAdded;

        Vector3 normal = cross(ab, ac);
        Vector3 dirCheck = cross(ab, normal);

        if (dot(dirCheck, aToOrigin) > 0.0f)
        {
            *nextDirection = tripleProduct(ab, aToOrigin, ab);

            if (nextDirection->magSqrd() <= 0.0000001f)
            {
                *nextDirection = normal;
            }

            // remove c
            movePoint(0, 1);
            movePoint(1, 2);

            nPoints = 2;

            return 0;
        }

        dirCheck = cross(normal, ac);

        if (dot(dirCheck, aToOrigin) > 0.0f)
        {
            *nextDirection = tripleProduct(ac, aToOrigin, ac);

            if (nextDirection->magSqrd() <= 0.0000001f)
            {
                *nextDirection = normal;
            }

            // remove b
            movePoint(1, 2);
            nPoints = 2;

            return 0;
        }

        if (dot(normal, aToOrigin) > 0.0f)
        {
            *nextDirection = normal;
            return 0;
        }

        // change triangle winding
        // temporarily use unused vertex 4
        movePoint(3, 0);
        movePoint(0, 1);
        movePoint(1, 3);
        *nextDirection = -normal;
    }
    else if (nPoints == 4)
    {
        int lastBehindIndex = -1;
        int lastInFrontIndex = -1;
        int isFrontCount = 0;

        Vector3 normals[3];

        for (int i = 0; i < 3; ++i)
        {
            Vector3 firstEdge = lastAdded - points[i];
            Vector3 secondEdge = (i == 2 ? points[0] : points[i + 1]) - points[i];
            normals[i] = cross(firstEdge, secondEdge);

            if (dot(aToOrigin, normals[i]) > 0.0f)
            {
                ++isFrontCount;
                lastInFrontIndex = i;
            }
            else
            {
                lastBehindIndex = i;
            }
        }

        if (isFrontCount == 0)
        {
            // enclosed the origin
            return 1;
        }
        else if (isFrontCount == 1)
        {
            *nextDirection = normals[lastInFrontIndex];

            if (lastInFrontIndex == 1)
            {
                movePoint(0, 1);
                movePoint(1, 2);
            }
            else if (lastInFrontIndex == 2)
            {
                movePoint(1, 0);
                movePoint(0, 2);
            }

            movePoint(2, 3);
            nPoints = 3;
        }
        else if (isFrontCount == 2)
        {
            if (lastBehindIndex == 0)
            {
                movePoint(0, 2);
            }
            else if (lastBehindIndex == 2)
            {
                movePoint(0, 1);
            }

            movePoint(1, 3);
            nPoints = 2;

            Vector3 ab = points[0] - points[1];

            *nextDirection = tripleProduct(ab, aToOrigin, ab);

            if (nextDirection->magSqrd() <= 0.0000001f)
            {
                *nextDirection = perp(ab);
            }
        }
        else
        {
            // this case shouldn't happen but if it does
            // this is the correct logic
            movePoint(0, 3);
            nPoints = 1;
            *nextDirection = aToOrigin;
        }
    }

    return 0;
}

void Simplex::movePoint(int to, int from)
{
    points[to] = points[from];
    objectAPoint[to] = objectAPoint[from];
}

int GJK::checkForOverlap(Simplex *simplex, const std::unique_ptr<Collider> &a, const std::unique_ptr<Collider> &b, const Vector3 *firstDirection)
{
    Vector3 aPoint;
    Vector3 bPoint;
    Vector3 nextDirection;

    if (firstDirection->isZero())
    {
        aPoint = a->minkowskiSumWorld(Math::Vec3Right);
        nextDirection = -Math::Vec3Right;

        bPoint = b->minkowskiSumWorld(nextDirection);
        simplex->addPoint(&aPoint, &bPoint);
    }
    else
    {
        aPoint = a->minkowskiSumWorld(*firstDirection);
        nextDirection = -*firstDirection;

        bPoint = b->minkowskiSumWorld(nextDirection);
        simplex->addPoint(&aPoint, &bPoint);
    }

    for (int iteration = 0; iteration < MaxGJKIterations; ++iteration)
    {
        Vector3 reverseDirection;
        reverseDirection = -nextDirection;
        aPoint = a->minkowskiSumWorld(nextDirection);
        bPoint = b->minkowskiSumWorld(reverseDirection);

        Vector3 *addedPoint = simplex->addPoint(&aPoint, &bPoint);

        if (!addedPoint) // Too many points
        {
            return 0;
        }

        if (dot(*addedPoint, nextDirection) <= 0.0f)
        {
            return 0;
        }

        if (simplex->check(&nextDirection))
        {
            return 1;
        }
    }

    return 0;
}
