#ifndef __BASEGE_COMPONENTS_H__
#define __BASEGE_COMPONENTS_H__
//////////////////////////////////////////////////////////////////////////////
//
//    COMPONENTS.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Components base.

    TODO:
      Optimize common wrapper methods
*/

namespace BaseGE
{
/*                                  CLASSES                                 */
  class Components : public Base::Object
  {
  public:
    GE_COMPONENT componentType;

    struct {
    Vertices* vertices;
    Edges* edges;
    Primitives* primitives;
    Surfaces* surfaces;

    VertexData* vertexData;
    EdgeData* edgeData;
    PrimitiveData* primitiveData;
    SurfaceData* surfaceData; };

    inline Components(GE_COMPONENT componentType) : componentType(componentType)
    { memset(&vertices, 0, sizeof(void*) * 8); }

    // Wrappers
    inline Elements* getVertexIndices()
    {
      if(vertices == 0)
        return 0;

      return cast_id<BaseGE::Elements>(vertices->getAttribute(GE_INDEX));
    }

    inline Elements* getVertexPositions()
    {
      // todo: speed this up by storing the index elements in a seperate pointer

      if(vertices == 0)
        return 0;

      // Get index elements
      Elements* indexElements = getVertexIndices();

      // If no indices, return unindexed position elements
      if(indexElements == 0)
        return BaseGE::cast_id<BaseGE::Elements>(vertices->getAttribute(GE_POSITION));

      // Test whether indexed vertex data exists
      if(vertexData == 0)
        return 0;

      // Return indexed position elements if contained by the indexed vertex data
      Elements* positionElements = BaseGE::cast_id<BaseGE::Elements>(vertexData->getAttribute(GE_POSITION));
      if(positionElements != 0)
        return positionElements;

      // Return unindexed position elements
      return BaseGE::cast_id<BaseGE::Elements>(vertices->getAttribute(GE_POSITION));
    }
  };
}

#endif
