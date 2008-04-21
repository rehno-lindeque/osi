#ifdef __NATIVEGE_GEOMETRYPOOL_H__
#ifndef __NATIVEGE_GEOMETRYPOOL_HXX__
#define __NATIVEGE_GEOMETRYPOOL_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    GEOMETRYPOOL.HXX
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace NativeGE
{
  inline void NativeGeometryPool::BIH_getSplittingPlane(BIH_SplittingPlane& splittingPlane, const AxisAlignedBoundingBox& boundingBox)
  {
    if(boundingBox.x2-boundingBox.x1 > boundingBox.y2-boundingBox.y1)
    {
      if(boundingBox.x2-boundingBox.x1 > boundingBox.z2-boundingBox.z1)
      { splittingPlane.axis = X_AXIS; splittingPlane.position = (boundingBox.x2+boundingBox.x1) / 2.0f; }
      else
      { splittingPlane.axis = Z_AXIS; splittingPlane.position = (boundingBox.z2+boundingBox.z1) / 2.0f; }
    }
    else
    {
      if(boundingBox.y2-boundingBox.y1 > boundingBox.z2-boundingBox.z1)
      { splittingPlane.axis = Y_AXIS; splittingPlane.position = (boundingBox.y2+boundingBox.y1) / 2.0f; }
      else
      { splittingPlane.axis = Z_AXIS; splittingPlane.position = (boundingBox.z2+boundingBox.z1) / 2.0f; }
    }
  }

  template<NativeGeometryPool::BIH_UNPACK_FUNCTION unpack>
  inline void NativeGeometryPool::BIH_UnpackTriangle(BIH_CoarseNode& node, const Vector3f& v1, const Vector3f& v2, const Vector3f& v3)
  {
    Vector3f* unpackPosition;

    if(unpack == LEFT_COMPONENTS)
    {
      // Place in left buffer
      unpackPosition = (Vector3f*)node.leftConstructionPosition;
      node.leftConstructionPosition += 3*3;
    }
    if(unpack == RIGHT_COMPONENTS)
    {
      // Place in right buffer
      unpackPosition = (Vector3f*)node.rightConstructionPosition;
      node.rightConstructionPosition += 3*3;
    }
    else // (unpack == MIDDLE_COMPONENTS)
    {
      // Test whether primitive is contained in the left \ right of the middle node
      float minValue, maxValue;
      minValue = maxValue = v1(node.splittingPlane.axis);
      minValue = min(minValue, v2(node.splittingPlane.axis));
      minValue = min(minValue, v3(node.splittingPlane.axis));
      maxValue = max(maxValue, v2(node.splittingPlane.axis));
      maxValue = max(maxValue, v3(node.splittingPlane.axis));

      float positiveAxisDistance = maxValue - node.splittingPlane.position;
      float negativeAxisDistance = node.splittingPlane.position - minValue;

      if(positiveAxisDistance <= 0.0f && negativeAxisDistance >= 0.0f)
      {
        // Place on the left side of the middle buffer
        unpackPosition = (Vector3f*)node.middleLeftConstructionPosition;
        node.middleLeftConstructionPosition += 3*3;

        // Update left boundary
        node.nodeHeader->leftBoundary = max(node.nodeHeader->leftBoundary, maxValue);

        // Update primitive counts
        ++node.nLeftPrimitives;
        --node.nMiddlePrimitives;
      }
      else if(positiveAxisDistance > 0.0f && negativeAxisDistance <= 0.0f)
      {
        // Place on the right side of the buffer
        node.middleRightConstructionPosition -= 3*3;
        unpackPosition = (Vector3f*)node.middleRightConstructionPosition;

        // Update right boundary
        node.nodeHeader->rightBoundary = min(node.nodeHeader->rightBoundary, minValue);

        // Update primitive counts
        ++node.nRightPrimitives;
        --node.nMiddlePrimitives;
      }
      else
      {
        /* Old: It is not necessary or wise to have a middle buffer because primitives
                will be resorted in subsequent subdivisions anyway. Instead, test on which side
                the triangle occupies the most area.
           Note: For now we're just doing a simple approximation by testing which side has the most distance
                 to a vertex. */
        /*
        // Place in temporary "middle middle" buffer
        unpackPosition = (Vector3f*)node.middleMiddleConstructionPosition;
        node.middleMiddleConstructionPosition += 3*3;

        // Update left & right boundary
        node.nodeHeader->leftBoundary = max(node.nodeHeader->leftBoundary, maxValue);
        node.nodeHeader->rightBoundary = min(node.nodeHeader->rightBoundary, minValue);*/

        if(positiveAxisDistance <= negativeAxisDistance)
        {
          // Place on the left side of the middle buffer
          unpackPosition = (Vector3f*)node.middleLeftConstructionPosition;
          node.middleLeftConstructionPosition += 3*3;

          // Update left boundary
          node.nodeHeader->leftBoundary = max(node.nodeHeader->leftBoundary, maxValue);

          // Update primitive counts
          ++node.nLeftPrimitives;
          /*note: keeping this "middle count" incase it's usefull later on
          --node.nMiddlePrimitives;*/
        }
        else
        {
          // Place on the right side of the buffer
          node.middleRightConstructionPosition -= 3*3;
          unpackPosition = (Vector3f*)node.middleRightConstructionPosition;

          // Update right boundary
          node.nodeHeader->rightBoundary = min(node.nodeHeader->rightBoundary, minValue);

          // Update primitive counts
          ++node.nRightPrimitives;
          /*note: keeping this "middle count" incase it's usefull later on
          --node.nMiddlePrimitives;*/
        }
      }
    }

    // Unpack primitive
    unpackPosition[0] = v1;
    unpackPosition[1] = v2;
    unpackPosition[2] = v3;
  }

  template<NativeGeometryPool::BIH_UNPACK_FUNCTION unpack>
  inline void NativeGeometryPool::BIH_UnpackInstance(BIH_CoarseNode& node, GeometryInstance* geometryInstance)
  {
    const BIH_UNPACK_FUNCTION unpackFunction = unpack; // Note: this is needed for the MSVC compiler to work correctly

    BaseGE::Geometry* geometry = geometryInstance->getGeometry();
    std::vector<BaseGE::Components*>* componentsList = geometry->getComponentsList();

    // Unpack components
    for(std::vector<BaseGE::Components*>::iterator iComponents = componentsList->begin(); iComponents != componentsList->end(); ++iComponents)
    {
      // Get vertex positions and indices
      BaseGE::Components* components = *iComponents;
      BaseGE::Elements* vertexIndexElements = components->getVertexIndices();
      BaseGE::Elements* vertexPositionElements = components->getVertexPositions();

      if(vertexPositionElements == 0)
        continue;

      // Map elements
      // Note: the mapped buffer is also used as a temporary buffer for middle components (i.e. components intersecting the splitting plane)
      uint16* vertexIndices = 0;
      if(vertexIndexElements)
        vertexIndices = (uint16*)vertexIndexElements->mapData(OS_UINT16, false);
      Vector3f* vertexPositions = (Vector3f*)vertexPositionElements->mapData(OS_VEC3_FLOAT, false);

      // Unpack primitives
      switch(components->componentType)
      {
      case GE_POINT:
        //todo
        break;
      case GE_LINE:
        //todo
        break;
      case GE_TRIANGLE:
        {
          if(vertexIndices != 0)
          {
            OSuint nPrimitives = vertexIndexElements->getLength() / 3;
            for(OSuint cPrimitive = 0; cPrimitive < nPrimitives; ++cPrimitive)
            {
              // Unpack indexed triangle
              uint16 i1 = vertexIndices[cPrimitive*3+0];
              uint16 i2 = vertexIndices[cPrimitive*3+1];
              uint16 i3 = vertexIndices[cPrimitive*3+2];
              BIH_UnpackTriangle<unpackFunction>(node, vertexPositions[i1], vertexPositions[i2], vertexPositions[i3]);
            }
          }
          else
          {
            OSuint nPrimitives = vertexPositionElements->getLength() / 3;
            for(OSuint cPrimitive = 0; cPrimitive < nPrimitives; ++cPrimitive)
            {
              // Unpack unindexed triangle
              BIH_UnpackTriangle<unpackFunction>(node, vertexPositions[cPrimitive*3+0], vertexPositions[cPrimitive*3+1], vertexPositions[cPrimitive*3+2]);
            }
          }
          break;
        }
      case GE_QUAD:
        //todo
        break;
      case GE_POLYGON:
        //todo
        break;
      }

      // Unmap elements
      vertexIndexElements->unmapData(vertexIndices);
      vertexPositionElements->unmapData(vertexPositions);
    }
  }

  template<NativeGeometryPool::BIH_UNPACK_FUNCTION unpack>
  inline void NativeGeometryPool::BIH_UnpackComponents(BIH_CoarseNode& node)
  {
    const BIH_UNPACK_FUNCTION unpackFunction = unpack; // Note: this is needed for the MSVC compiler to work correctly

    // Get the geometry instances for the node to unpack
    std::list<OSobject>* geometryInstances;
    if(unpack == LEFT_COMPONENTS)
      geometryInstances = &node.leftGeometryInstances;
    else if(unpack == MIDDLE_COMPONENTS)
      geometryInstances = &node.middleGeometryInstances;
    else
      geometryInstances = &node.rightGeometryInstances;

    // Unpack instances' components
    for(std::list<OSobject>::iterator i = geometryInstances->begin(); i != geometryInstances->end(); ++i)
      BIH_UnpackInstance<unpackFunction>(node, cast_id<GeometryInstance>(*i));
  }

  inline void NativeGeometryPool::BIH_InsertInstance(BIH_CoarseNode& node, GeometryInstance* geometryInstance)
  {
    // Calculate the geometry instance bounding box
    AxisAlignedBoundingBox boundingBox = geometryInstance->calculateBoundingBox();

    // Get the bounding boxes' value index
    uint aabbIndex = node.splittingPlane.axis;

    // Get bounding-box min\max values
    float minValue = boundingBox(aabbIndex);
    float maxValue = boundingBox(aabbIndex+3);

    // Test whether the bounding box intersects the splitting plane
    if(minValue < node.splittingPlane.position && maxValue > node.splittingPlane.position)
    {
      // Place geometry instance into the middle ("between") node
      node.middleGeometryInstances.push_back(cast_object(geometryInstance));
      node.nMiddlePrimitives += geometryInstance->countPrimitives();
    }
    else if(minValue < node.splittingPlane.position)
    {
      // Place geometry instance into the left node
      node.leftGeometryInstances.push_back(cast_object(geometryInstance));
      node.nLeftPrimitives += geometryInstance->countPrimitives();

      // Update left boundary
      node.nodeHeader->leftBoundary = max(node.nodeHeader->leftBoundary, maxValue);
    }
    else
    {
      // Place geometry instance into the right node
      node.rightGeometryInstances.push_back(cast_object(geometryInstance));
      node.nRightPrimitives += geometryInstance->countPrimitives();

      // Update right boundary
      node.nodeHeader->rightBoundary = min(node.nodeHeader->rightBoundary, minValue);
    }
  }

  inline void NativeGeometryPool::swapTriangles(float* triangle1, float* triangle2)
  {
    float temp[3*3];
    memcpy(temp,      triangle1, 3*3*sizeof(float));
    memcpy(triangle1, triangle2, 3*3*sizeof(float));
    memcpy(triangle2, temp,      3*3*sizeof(float));
  }

  inline void NativeGeometryPool::BIH_BuildFineNode(BIH_FineNode& node)
  {
    //// Initialize variables
    const uint MIN_PRIMITIVES_THRESHOLD = 4;
    uint nPrimitives = ((OSuint)(node.rightConstructionPosition - node.leftConstructionPosition)) / 9;

    //// Assign header
    node.nodeHeader = (BIH_NodeHeader*)structConstructionPosition;
    structConstructionPosition = ((BIH_NodeHeader*)structConstructionPosition) + 1;

    //// Construct leaf node
    // Test whether the number of primitives in node is less than the minimum threshold
    if(nPrimitives <= MIN_PRIMITIVES_THRESHOLD)
    {
      node.leafHeader->nodeType = LEAF;
      node.leafHeader->nPrimitives = nPrimitives;
      node.leafHeader->primitiveOffset = ((OSuint)(node.leftConstructionPosition-(float*)primitiveData)) / 9;
      return;
    }

    //// Construct split node
    // Get the splitting plane
    BIH_getSplittingPlane(node.splittingPlane, node.boundingBox);

    // Initialize node structure header
    node.nodeHeader->leftBoundary  = -MAX_FLOAT;
    node.nodeHeader->rightBoundary = MAX_FLOAT;
    node.nodeHeader->nodeType      = (BIH_NODE_TYPE)node.splittingPlane.axis;

    // Sort primitives
    Vector3f* bufferPosition = (Vector3f*)node.leftConstructionPosition;
    uint leftPosition = 0;
    uint rightPosition = nPrimitives-1;
    while(leftPosition < rightPosition)
    {
      Vector3f& v1 = bufferPosition[leftPosition*3+0];
      Vector3f& v2 = bufferPosition[leftPosition*3+1];
      Vector3f& v3 = bufferPosition[leftPosition*3+2];

      // Test whether primitive is contained in the left \ right of the middle node
      float minValue, maxValue;
      minValue = maxValue = v1(node.splittingPlane.axis);
      minValue = min(minValue, v2(node.splittingPlane.axis));
      minValue = min(minValue, v3(node.splittingPlane.axis));
      maxValue = max(maxValue, v2(node.splittingPlane.axis));
      maxValue = max(maxValue, v3(node.splittingPlane.axis));

      float positiveAxisDistance = maxValue - node.splittingPlane.position;
      float negativeAxisDistance = node.splittingPlane.position - minValue;

      if(positiveAxisDistance <= 0.0f && negativeAxisDistance >= 0.0f)
      {
        // Triangle is on the left side of the splitting plane
        leftPosition += 1;

        // Update left boundary
        node.nodeHeader->leftBoundary = max(node.nodeHeader->leftBoundary, maxValue);
      }
      else if(positiveAxisDistance > 0.0f && negativeAxisDistance <= 0.0f)
      {
         // Triangle is on the right side of the splitting plane
        swapTriangles((float*)&bufferPosition[3*leftPosition], (float*)&bufferPosition[3*rightPosition]);
        rightPosition -= 1;

        // Update right boundary
        node.nodeHeader->rightBoundary = min(node.nodeHeader->rightBoundary, minValue);
      }
      else
      {
        if(positiveAxisDistance <= negativeAxisDistance)
        {
          // Triangle is on the left side of the splitting plane
          leftPosition += 1;

          // Update left boundary
          node.nodeHeader->leftBoundary = max(node.nodeHeader->leftBoundary, maxValue);
        }
        else
        {
          // Triangle is on the right side of the splitting plane
          swapTriangles((float*)&bufferPosition[3*leftPosition], (float*)&bufferPosition[3*rightPosition]);
          rightPosition -= 1;

          // Update right boundary
          node.nodeHeader->rightBoundary = min(node.nodeHeader->rightBoundary, minValue);
        }
      }
    }

    // Abort if splitting made no difference
    // todo: This can be improved by switching to an alternative axis instead
    if(leftPosition == 0 || rightPosition == nPrimitives-1)
    {
      node.leafHeader->nodeType = LEAF;
      node.leafHeader->nPrimitives = nPrimitives;
      node.leafHeader->primitiveOffset = ((OSuint)(node.leftConstructionPosition-(float*)primitiveData)) / 9;
      return;
    }

    //// Build child nodes
    // Initialize nodes
    BIH_FineNode childNode;

    // Left node
    childNode.leftConstructionPosition = node.leftConstructionPosition;
    childNode.rightConstructionPosition = node.leftConstructionPosition + 9*(leftPosition+1);
    childNode.boundingBox = node.boundingBox;
    childNode.boundingBox(3+node.splittingPlane.axis) = min(node.nodeHeader->leftBoundary, node.splittingPlane.position);
    BIH_BuildFineNode(childNode);

    // Right node
    childNode.leftConstructionPosition = node.leftConstructionPosition + 9*(leftPosition+1);
    childNode.rightConstructionPosition = node.rightConstructionPosition;
    childNode.boundingBox = node.boundingBox;
    childNode.boundingBox(node.splittingPlane.axis) = max(node.nodeHeader->rightBoundary, node.splittingPlane.position);
    BIH_BuildFineNode(childNode);

    //// Calculate right node header offset
    node.nodeHeader->rightOffset = (OSuint)((BIH_NodeHeader*)structConstructionPosition - node.nodeHeader);
  }

  inline void NativeGeometryPool::BIH_RefineCoarseNode(BIH_CoarseNode& node, std::list<OSobject>& geometryInstances)
  {
    // Get the splitting plane
    BIH_getSplittingPlane(node.splittingPlane, node.boundingBox);

    // Move the geometry instances down into the child nodes
    for(std::list<OSobject>::iterator i = geometryInstances.begin(); i != geometryInstances.end(); ++i)
    {
      GeometryInstance* geometryInstance = cast_id<GeometryInstance>(*i);
      BIH_InsertInstance(node, geometryInstance);
    }
  }

  inline void NativeGeometryPool::BIH_BuildCoarseNode(BIH_CoarseNode& node)
  {
    static const OSuint N_MIN_SPLIT_PRIMITIVES = 16;
    float* const nodePrimitivePosition = (float*)primitiveConstructionPosition;

    //// Initialize
    // Calculate rough primitive construction boundaries (using instance nodes)
    node.leftConstructionPosition        = (float*)primitiveConstructionPosition;
    node.middleLeftConstructionPosition  = (float*)primitiveConstructionPosition + 9*node.nLeftPrimitives;
    node.middleRightConstructionPosition = (float*)primitiveConstructionPosition + 9*(node.nLeftPrimitives + node.nMiddlePrimitives);
    node.rightConstructionPosition       = (float*)primitiveConstructionPosition + 9*(node.nLeftPrimitives + node.nMiddlePrimitives);

    // Initialize node structure header
    node.nodeHeader = (BIH_NodeHeader*)structConstructionPosition;
    node.nodeHeader->nodeType = (BIH_NODE_TYPE)node.splittingPlane.axis;
    node.nodeHeader->leftBoundary = -MAX_FLOAT;
    node.nodeHeader->rightBoundary = MAX_FLOAT;
    structConstructionPosition = ((BIH_NodeHeader*)structConstructionPosition) + 1;

    //// Unpack middle node components
    BIH_UnpackComponents<MIDDLE_COMPONENTS>(node);
    node.middleGeometryInstances.clear();

    //// Build and refine left child node
    if(node.nLeftPrimitives <= N_MIN_SPLIT_PRIMITIVES)
    {
      //// Construct fine node
      // Unpack components
      BIH_UnpackComponents<LEFT_COMPONENTS>(node);
      node.leftGeometryInstances.clear();

      // Create node
      BIH_FineNode childNode;
      childNode.leftConstructionPosition = nodePrimitivePosition;
      childNode.rightConstructionPosition = nodePrimitivePosition + 9*node.nLeftPrimitives;
      //older:childNode.rightConstructionPosition = nodePrimitivePosition + node.nodeHeader->rightOffset;
      //older: childNode.rightConstructionPosition = node.middleLeftConstructionPosition;

      // Calculate bounding box
      // note: the min condition helps avoid situations where a polygon is so big that it prevents a node shrinking in size with further subdivisions
      childNode.boundingBox = node.boundingBox;
      childNode.boundingBox(3+node.splittingPlane.axis) = min(node.nodeHeader->leftBoundary, node.splittingPlane.position);

      // Subdivide components
      BIH_BuildFineNode(childNode);
    }
    else
    {
      //// Construct coarse node
      // Calculate bounding box
      BIH_CoarseNode childNode;
      childNode.boundingBox = node.boundingBox;
      childNode.boundingBox(3+node.splittingPlane.axis) = min(node.nodeHeader->leftBoundary, node.splittingPlane.position);

      // Split the node
      BIH_RefineCoarseNode(childNode, node.leftGeometryInstances);
      node.leftGeometryInstances.clear();

      // Subdivide node
      BIH_BuildCoarseNode(childNode);
    }

    //// Calculate right node header offset
    node.nodeHeader->rightOffset = (OSuint)((BIH_NodeHeader*)structConstructionPosition - node.nodeHeader);

    // Update primitive construction pointer
    primitiveConstructionPosition = nodePrimitivePosition + 9*node.nLeftPrimitives;

    //// Build and refine right child node
    if(node.nRightPrimitives <= N_MIN_SPLIT_PRIMITIVES)
    {
      //// Construct fine node
      // Unpack components
      BIH_UnpackComponents<RIGHT_COMPONENTS>(node);
      node.rightGeometryInstances.clear();

      // Create node
      BIH_FineNode childNode;
      //old: childNode.leftConstructionPosition = nodePrimitivePosition + 3*node.nodeHeader->rightOffset;
      childNode.leftConstructionPosition = nodePrimitivePosition + 9*node.nLeftPrimitives;
      //old: childNode.rightConstructionPosition = nodePrimitivePosition +  + 9*(node.nLeftPrimitives + node.nMiddlePrimitives + node.nRightPrimitives);
      childNode.rightConstructionPosition = childNode.leftConstructionPosition + 9*node.nRightPrimitives;
      //older: childNode.leftConstructionPosition = (float*)primitiveConstructionPosition;
      //older: childNode.rightConstructionPosition = node.middleLeftConstructionPosition;

      // Calculate bounding box
      childNode.boundingBox = node.boundingBox;
      childNode.boundingBox(node.splittingPlane.axis) = max(node.nodeHeader->rightBoundary, node.splittingPlane.position);

      // Subdivide components
      BIH_BuildFineNode(childNode);
    }
    else
    {
      //// Construct coarse node
      // Calculate bounding box
      BIH_CoarseNode childNode;
      childNode.boundingBox = node.boundingBox;
      childNode.boundingBox(node.splittingPlane.axis) = max(node.nodeHeader->rightBoundary, node.splittingPlane.position);

      // Split the node
      BIH_RefineCoarseNode(childNode, node.rightGeometryInstances);
      node.rightGeometryInstances.clear();

      // Subdivide node
      BIH_BuildCoarseNode(childNode);
    }

    // Update primitive construction pointer
    //old: primitiveConstructionPosition = nodePrimitivePosition + 9*(node.nLeftPrimitives + node.nMiddlePrimitives + node.nRightPrimitives);
    primitiveConstructionPosition = nodePrimitivePosition + 9*(node.nLeftPrimitives + node.nRightPrimitives);
  }

  inline void NativeGeometryPool::BIH_BuildRootNode(BIH_CoarseNode& rootNode)
  {
    // Intitialize root node
    rootNode.boundingBox = boundingBox;

    // Get the splitting plane
    BIH_getSplittingPlane(rootNode.splittingPlane, boundingBox);

    // Refine root node (Insert geometry instances into the node)
    GeometryInstanceMap* geometryInstanceMaps[3];
    geometryInstanceMaps[0] = &staticInstances;
    geometryInstanceMaps[1] = &dynamicStableInstances;
    geometryInstanceMaps[2] = &dynamicUnstableInstances;

    for(int c = 0; c < 3; ++c)
      for(GeometryInstanceMap::iterator i = geometryInstanceMaps[c]->begin(); i != geometryInstanceMaps[c]->end(); ++i)
      {
        GeometryInstance* geometryInstance = (*i).second;
        BIH_InsertInstance(rootNode, geometryInstance);
      }

    // Subdivide node
    BIH_BuildCoarseNode(rootNode);
  }

  inline OSid NativeGeometryPool::BIH_rebuild()
  {
    return 0;
  }

  inline OSid NativeGeometryPool::BIH_build()
  {
    const int MAX_PRIMITIVES_PER_LEAF = 4;

    // Create memory for the structure
    OSuint nPrimitives = countPrimitives();
    primitiveData = new float[nPrimitives*3*3];
    structData = new BIH_NodeHeader[sizeof(float*) + ((int)ceilf(nPrimitives/(float)MAX_PRIMITIVES_PER_LEAF))*2]; //todo: recalc
    *(float**)structData = (float*)primitiveData;
    //structData = ((float**)structData) + 1;

    // Initialize temporary construction variables
    //structConstructionPosition = structData;
    structConstructionPosition = ((float**)structData) + 1;
    primitiveConstructionPosition = primitiveData;

    // Build the root node
    BIH_CoarseNode rootNode;
    BIH_BuildRootNode(rootNode);

    return (OSid)structData; //todo: also return primitive data
  }

  inline AxisAlignedBoundingBox NativeGeometryPool::calculateBoundingBox()
  {
    // Initialize bounding box
    if(!staticInstances.empty())
      boundingBox = (*staticInstances.begin()).second->calculateBoundingBox();
    else if(!dynamicStableInstances.empty())
      boundingBox = (*dynamicStableInstances.begin()).second->calculateBoundingBox();
    else if(!dynamicUnstableInstances.empty())
      boundingBox = (*dynamicUnstableInstances.begin()).second->calculateBoundingBox();
    else
      return AxisAlignedBoundingBox(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    // Retrieve bounding boxes of all geometry instances
    for(GeometryInstanceMap::iterator i = staticInstances.begin(); i != staticInstances.end(); ++i)
      boundingBox.apply_union((*i).second->calculateBoundingBox());

    for(GeometryInstanceMap::iterator i = dynamicStableInstances.begin(); i != dynamicStableInstances.end(); ++i)
      boundingBox.apply_union((*i).second->calculateBoundingBox());

    for(GeometryInstanceMap::iterator i = dynamicUnstableInstances.begin(); i != dynamicUnstableInstances.end(); ++i)
      boundingBox.apply_union((*i).second->calculateBoundingBox());
  }
}

#endif
#endif
