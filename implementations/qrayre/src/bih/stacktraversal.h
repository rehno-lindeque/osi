#ifndef __QRAYRE_BIH_STACKTRAVERSAL_H__
#define __QRAYRE_BIH_STACKTRAVERSAL_H__
//////////////////////////////////////////////////////////////////////////////
//
//    STACKTRAVERSAL.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      A non-recursive stack based ray traversal algorithm for Binary
      Interval Hierarchies

      + BIH_CastRay: Cast an infinite ray through the BIH structure and
        determine the closest intersection. (Includes intersection information)

      + BIH_IntersectSegment: Determines whether a line segment intersects



    IMPLEMENTATION:
      + This is implemented as a stack-based loop rather than a recursive
        function in order to avoid method call overhead. Whether this method
        is more efficient than recursion is still under consideration

    ALGORITHMS:
      + Axis ordering
        An axis ordering vector is a vector of binary indexing components
        along the primary axis.
        Each component lists the first intersection index of a plane
        perpendicular to its axis. This is determined according to the
        direction of the ray and is independent of the ray origin. For
        example a ray direction with a negative sign along the x-axis, will
        have an axis ordering index of 1 for the x-axis component. Conversely,
        a positive sign will result in an axis ordering index of 0.

        The reverse axis ordering index of an axis n can be found as follows:
        reverseAxisOrdering(n) = 1 - axisOrdering(n)
        or
        reverseAxisOrdering(n) = 1 ^ axisOrdering(n)

      + Finding the two alternate axis to the BIH interval bounding planes
        The algorithms implement an index counter that cycles through the
        three axis: x -> y -> z -> x -> y -> z -> x -> ... (etc)

        Algorithm 1: Simple bit-wise calculation
        -----------
        xy -> ab
        00 -> 01
        01 -> 10
        10 -> 00

        a = (~x & y)
        b = (~x & ~y)

        axisIndex = ( (((~axisIndex) & 2) >> 1) & (axisIndex & 1) ) | ( ((~axisIndex) & 2) & (((~axisIndex) & 1) << 1) );

        Algorithm 2
        -----------
        (0 + 0 >> 1 + 1) & 3 = 1 & 3 = 1
        (1 + 1 >> 1 + 1) & 3 = 2 & 3 = 2
        (2 + 2 >> 1 + 1) & 3 = 4 & 3 = 0

        axisIndex = (axisIndex + (axisIndex >> 1) + 1) & 3;
*/

/*                                  MACROS                                  */
//#define BIT_INVERSE(x) (x - 1)
#define BIT_INVERSE(x) (x ^ 1)

namespace QRayRE
{
/*                                PROTOTYPES                                */
  inline bool BIH_Ray_Intersect(uint32& primitiveId,
                                float& distance,
                                Vector2f& uv,
                                const AxisAlignedBoundingBoxf& boundingBox,
                                const Vector3f* primitiveData,
                                const BIH_NodeHeader* rootNode,
                                const Vector3f& origin, // ray
                                const Vector3f& direction);

  inline bool BIH_Segment_Test(const Vector3f* source,
                               const Vector3f* primitiveData,
                               const BIH_NodeHeader* rootNode,
                               const Vector3f& origin, // segment
                               const Vector3f& direction,
                               const float length);

  /*inline bool BIH_Plane_SplitPaths(void* leftBinPath,
                                   void* rightBinPath,
                                   const BIH_NodeHeader* rootNode,
                                   const Vector3f& origin, // plane
                                   const Vector3f& direction1,
                                   const Vector3f& direction2);

  inline bool BIH_Sector_InstersectPath(void* binPath,
                                        const BIH_NodeHeader* rootNode,
                                        const Vector3f& origin, // sector
                                        const Vector3f& direction1,
                                        const Vector3f& direction2);

  inline bool BIH_Ray_WalkPathIntersect(uint32& primitiveId,
                                        float& distance,
                                        Vector2f& uv,
                                        const Vector3f* primitiveData,
                                        const BIH_NodeHeader* rootNode,
                                        const void* binPath,
                                        const Vector3f& origin, // ray
                                        const Vector3f& direction);*/

/*                              IMPLEMENTATION                              */
  inline bool BIH_Ray_Intersect(uint32& primitiveId, float& distance, Vector2f& uv, const AxisAlignedBoundingBoxf& boundingBox, const Vector3f* primitiveData, const BIH_NodeHeader* rootNode, const Vector3f& origin, const Vector3f& direction)
  {
    std::stack<BIH_NodeHeader*> nodeStack;
    const BIH_NodeHeader* nodePosition = rootNode;
    AxisAlignedBoundingBoxf nodeBoundingBox = boundingBox;

    bool hit = false;
    float tHit = MAX_FLOAT;

    //// Precomputations
    // Calculate traversal order of the axis
    uint8 axisOrder[3];
    axisOrder[0] = (uint8)(direction(0) < 0.0f);
    axisOrder[1] = (uint8)(direction(1) < 0.0f);
    axisOrder[2] = (uint8)(direction(2) < 0.0f);

    //// Calculate sign or ray direction
    int8 raySign[3];
    raySign[0] = 2 * BIT_INVERSE(axisOrder[0]) - 1;
    raySign[1] = 2 * BIT_INVERSE(axisOrder[1]) - 1;
    raySign[2] = 2 * BIT_INVERSE(axisOrder[2]) - 1;

    // Calculate inverse of the ray direction components
    Vector3f invRayDirection(0.0f, 0.0f, 0.0f);
    if(direction(0) != 0.0f) invRayDirection(0) = 1.0f/direction(0);
    if(direction(1) != 0.0f) invRayDirection(1) = 1.0f/direction(1);
    if(direction(2) != 0.0f) invRayDirection(2) = 1.0f/direction(2);

    //// Calculate initial bounding box intersection (tNear \ tFar values)
    float tNear = 0.0f, tFar = MAX_FLOAT;
    if(boundingBox.contains(origin))
    {
      // Intersect furthest bounding box planes (using reverse axis order)
      if(invRayDirection(0) != 0.0f) tFar =           (boundingBox((1-axisOrder[0])*3 + 0) - origin(0)) * invRayDirection(0);
      if(invRayDirection(0) != 0.0f) tFar = min(tFar, (boundingBox((1-axisOrder[1])*3 + 1) - origin(1)) * invRayDirection(1));
      if(invRayDirection(0) != 0.0f) tFar = min(tFar, (boundingBox((1-axisOrder[2])*3 + 2) - origin(2)) * invRayDirection(2));
    }
    else
    {
      float nearPlaneBoundary, farPlaneBoundary;

      // Intersect nearest & furthest bounding box planes (using axis order & reverse axis order respectively)
      // X-axis
      nearPlaneBoundary = boundingBox(axisOrder[0]*3 + 0);
      farPlaneBoundary  = boundingBox(BIT_INVERSE(axisOrder[0])*3 + 0);
      if(invRayDirection(0) != 0.0f)
      {
        tNear = (nearPlaneBoundary - origin(0)) * invRayDirection(0);
        tFar  = (farPlaneBoundary  - origin(0)) * invRayDirection(0);
      }
      else if(origin(0) < nearPlaneBoundary || origin(0) > farPlaneBoundary)
        return false;

      // Y-axis
      nearPlaneBoundary = boundingBox(axisOrder[1]*3 + 1);
      farPlaneBoundary  = boundingBox(BIT_INVERSE(axisOrder[1])*3 + 1);
      if(invRayDirection(1) != 0.0f)
      {
        tNear = max(tNear, (nearPlaneBoundary - origin(1)) * invRayDirection(1));
        tFar  = min(tFar,  (farPlaneBoundary  - origin(1)) * invRayDirection(1));
      }
      else if(origin(1) < nearPlaneBoundary || origin(1) > farPlaneBoundary)
        return false;

      // Z-axis
      nearPlaneBoundary = boundingBox(axisOrder[2]*3 + 2);
      farPlaneBoundary  = boundingBox(BIT_INVERSE(axisOrder[2])*3 + 2);
      if(invRayDirection(2) != 0.0f)
      {
        tNear = max(tNear, (nearPlaneBoundary - origin(2)) * invRayDirection(2));
        tFar  = min(tFar,  (farPlaneBoundary  - origin(2)) * invRayDirection(2));
      }
      else if(origin(2) < nearPlaneBoundary || origin(2) > farPlaneBoundary)
        return false;

      // Test whether bounding box was hit
      if(tFar < tNear || tNear < 0.0f)
        return false;
    }

    //// Traverse spatial stucture
    // Initialize stack
    struct StackElement
    {
      const BIH_NodeHeader* nodePosition;
      float tFar;
      float tNear;
    };

    StackElement stack[50];
    stack[0].nodePosition = nodePosition;
    stack[0].tFar = tFar;
    stack[0].tNear = tNear;
    uint stackIndex = 1;

    // Initialize variables
    bool traverseDown = true; // move down the spatial tree
    float t;

    while(true)
    {
      if(traverseDown)
      {
        if(nodePosition->nodeType == LEAF)
        {
          //// Traverse leaf node
          BIH_LeafNodeHeader& leafNode = *(BIH_LeafNodeHeader*)nodePosition;

          // Intersect primitives (triangles)
          for(uint c = 0; c < leafNode.nPrimitives; ++c)
          {
            const Vector3f* tri = primitiveData + leafNode.primitiveOffset*3 + c*3;
            Vector2f intersectUV;
            float intersectDistance;

            //if(GuigueDevillers::intersectTriangle<float, 0.000001f, true>(intersectDistance, intersectUV, origin, direction, tri[0], tri[1],tri[2]))
            if(GuigueDevillers::intersectTriangle<float, true>(intersectDistance, intersectUV, origin, direction, tri[0], tri[1],tri[2]))
            {
              if(intersectDistance < tHit)
              {
                uv = intersectUV;
                distance = tHit = intersectDistance;
                primitiveId = leafNode.primitiveOffset*3 + c*3; //(temporary until decent primitive id can be set)
                hit = true;
              }
            }
          }

          // Traverse upwards
          traverseDown = false;
        }
        else
        {
          const BIH_NodeHeader& node = *nodePosition;

          //// Traverse near child node
          // Get the nearest child node's interval bounding plane (note: this is not necessarily the nearest bounding plane!)
          float boundaryPlane = node.boundaries[axisOrder[node.nodeType]];

          // Intersect boundary plane
          t = 0.0f;
          if(invRayDirection(node.nodeType) != 0.0f)
            t = (boundaryPlane - origin(node.nodeType)) * invRayDirection(node.nodeType);
          else if(origin(node.nodeType) <= boundaryPlane) // note: if direction(node.nodeType) == 0.0f, then the left node will always be considered the "near" node thus we can use the <= operator
            t = MAX_FLOAT;

          // Push node onto the stack
          stack[stackIndex].nodePosition = nodePosition;
          stack[stackIndex].tFar = tFar;
          stack[stackIndex].tNear = tNear;
          ++stackIndex;

          if(t >= tNear && tHit > tNear) // near node is hit
          {
            // Set tFar for the child node
            if(t < tFar)
              tFar = t;

            // Move nodePosition to the near child node (if axis order == 0: left child node, if axis order == 1: right child)
            nodePosition += node.rightOffset * axisOrder[node.nodeType] + 1 * BIT_INVERSE(axisOrder[node.nodeType]);
          }
          else // near node is missed
          {
            // Traverse upwards
            traverseDown = false;
          }
        }
      }
      else // traverse upwards
      {
        // Pop stack (go to parent node)
        if(stackIndex == 0)
          break;

        --stackIndex;
        nodePosition = stack[stackIndex].nodePosition;
        tFar = stack[stackIndex].tFar;
        tNear = stack[stackIndex].tNear;

        const BIH_NodeHeader& node = *nodePosition;

        //// Traverse far child node
        // Get the furthest child node's interval bounding plane (note: this is not necessarily the furthest bounding plane!)
        float boundaryPlane = node.boundaries[BIT_INVERSE(axisOrder[node.nodeType])];

        // Intersect boundary plane
        t = MAX_FLOAT;
        if(invRayDirection(node.nodeType) != 0.0f)
          t = (boundaryPlane - origin(node.nodeType)) * invRayDirection(node.nodeType);
        else if(origin(node.nodeType) >= boundaryPlane) // note: if direction(node.nodeType) == 0.0f, then the right node will always be considered the "far" node thus we can use the >= operator
          t = 0.0f;

        if(t <= tFar && (tHit > tNear && tHit > t)) // far node is hit
        {
          // Set tNear for the child node (if t < tNear or far node boundary is closer than near node boundary)
          if(t > tNear)
            tNear = t;

          // Move nodePosition to the far child node (if axis order == 1: left child node, if axis order == 0: right child)
          nodePosition += node.rightOffset * BIT_INVERSE(axisOrder[node.nodeType]) + 1 * axisOrder[node.nodeType];

          // Traverse downwards
          traverseDown = true;
        }
        // (else far node is missed, so continue traversing upwards)
      }
    }

    return hit;
  }

  inline bool BIH_Segment_Test(const Vector3f* source, const Vector3f* primitiveData, const BIH_NodeHeader* rootNode, const Vector3f& origin, const Vector3f& direction, const float length)
  {
    // todo: optimize this

    std::stack<BIH_NodeHeader*> nodeStack;
    const BIH_NodeHeader* nodePosition = rootNode;

    bool hit = false;

    //// Precomputations
    // Calculate traversal order of the axis
    uint8 axisOrder[3];
    axisOrder[0] = (uint8)(direction(0) < 0.0f);
    axisOrder[1] = (uint8)(direction(1) < 0.0f);
    axisOrder[2] = (uint8)(direction(2) < 0.0f);

    //// Calculate sign or ray direction
    int8 raySign[3];
    raySign[0] = 2 * BIT_INVERSE(axisOrder[0]) - 1;
    raySign[1] = 2 * BIT_INVERSE(axisOrder[1]) - 1;
    raySign[2] = 2 * BIT_INVERSE(axisOrder[2]) - 1;

    // Calculate inverse of the ray direction components
    Vector3f invRayDirection(0.0f, 0.0f, 0.0f);
    if(direction(0) != 0.0f) invRayDirection(0) = 1.0f/direction(0);
    if(direction(1) != 0.0f) invRayDirection(1) = 1.0f/direction(1);
    if(direction(2) != 0.0f) invRayDirection(2) = 1.0f/direction(2);

    //// Calculate initial bounding box intersection (tNear \ tFar values)
    float tNear = 0.0f, tFar = length;

    //// Traverse spatial stucture
    // Initialize stack
    struct StackElement
    {
      const BIH_NodeHeader* nodePosition;
      float tFar;
      float tNear;
    };

    StackElement stack[50];
    stack[0].nodePosition = nodePosition;
    stack[0].tFar = tFar;
    stack[0].tNear = tNear;
    uint stackIndex = 1;

    // Initialize variables
    bool traverseDown = true; // move down the spatial tree
    float t;

    while(true)
    {
      if(traverseDown)
      {
        if(nodePosition->nodeType == LEAF)
        {
          //// Traverse leaf node
          BIH_LeafNodeHeader& leafNode = *(BIH_LeafNodeHeader*)nodePosition;

          // Intersect primitives (triangles)
          for(uint c = 0; c < leafNode.nPrimitives; ++c)
          {
            const Vector3f* tri = primitiveData + leafNode.primitiveOffset*3 + c*3;
            Vector2f intersectUV;
            float intersectDistance;

            if(tri != source
              //&& SeguraFeito::intersectTriangle<float>(intersectDistance, origin, direction, tri[0], tri[1],tri[2]))
              //&& GuigueDevillers::intersectTriangle<float, 0.000001f, true>(intersectDistance, intersectUV, origin, direction, tri[0], tri[1],tri[2]))
              && GuigueDevillers::intersectTriangle<float, true>(intersectDistance, intersectUV, origin, direction, tri[0], tri[1],tri[2]))
              if(intersectDistance < length && intersectDistance > 0.0f)
                return true;
          }

          // Traverse upwards
          traverseDown = false;
        }
        else
        {
          const BIH_NodeHeader& node = *nodePosition;

          //// Traverse near child node
          // Get the nearest child node's interval bounding plane (note: this is not necessarily the nearest bounding plane!)
          float boundaryPlane = node.boundaries[axisOrder[node.nodeType]];

          // Intersect boundary plane
          t = 0.0f;
          if(invRayDirection(node.nodeType) != 0.0f)
            t = (boundaryPlane - origin(node.nodeType)) * invRayDirection(node.nodeType);
          else if(origin(node.nodeType) <= boundaryPlane) // note: if direction(node.nodeType) == 0.0f, then the left node will always be considered the "near" node thus we can use the <= operator
            t = MAX_FLOAT;

          // Push node onto the stack
          stack[stackIndex].nodePosition = nodePosition;
          stack[stackIndex].tFar = tFar;
          stack[stackIndex].tNear = tNear;
          ++stackIndex;

          if(t >= tNear) // near node is hit
          {
            // Set tFar for the child node
            if(t < tFar)
              tFar = t;

            // Move nodePosition to the near child node (if axis order == 0: left child node, if axis order == 1: right child)
            nodePosition += node.rightOffset * axisOrder[node.nodeType] + 1 * BIT_INVERSE(axisOrder[node.nodeType]);
          }
          else // near node is missed
          {
            // Traverse upwards
            traverseDown = false;
          }
        }
      }
      else // traverse upwards
      {
        // Pop stack (go to parent node)
        if(stackIndex == 0)
          break;

        --stackIndex;
        nodePosition = stack[stackIndex].nodePosition;
        tFar = stack[stackIndex].tFar;
        tNear = stack[stackIndex].tNear;

        const BIH_NodeHeader& node = *nodePosition;

        //// Traverse far child node
        // Get the furthest child node's interval bounding plane (note: this is not necessarily the furthest bounding plane!)
        float boundaryPlane = node.boundaries[BIT_INVERSE(axisOrder[node.nodeType])];

        // Intersect boundary plane
        t = MAX_FLOAT;
        if(invRayDirection(node.nodeType) != 0.0f)
          t = (boundaryPlane - origin(node.nodeType)) * invRayDirection(node.nodeType);
        else if(origin(node.nodeType) >= boundaryPlane) // note: if direction(node.nodeType) == 0.0f, then the right node will always be considered the "far" node thus we can use the >= operator
          t = 0.0f;

        if(t <= tFar) // far node is hit
        {
          // Set tNear for the child node (if t < tNear or far node boundary is closer than near node boundary)
          if(t > tNear)
            tNear = t;

          // Move nodePosition to the far child node (if axis order == 1: left child node, if axis order == 0: right child)
          nodePosition += node.rightOffset * BIT_INVERSE(axisOrder[node.nodeType]) + 1 * axisOrder[node.nodeType];

          // Traverse downwards
          traverseDown = true;
        }
        // (else far node is missed, so continue traversing upwards)
      }
    }

    return false;
  }

}

/*                                  MACROS                                  */
#undef BIT_INVERSE

#endif
