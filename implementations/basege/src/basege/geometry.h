#ifndef __BASEGE_GEOMETRY_H__
#define __BASEGE_GEOMETRY_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GEOMETRY.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Geometry base.

    NOTES:
      + Currently, Geometry does not take into account the possibility that
        some vertices may not be indexed. Calculations such as bounding box
        calculation uses all vertex data available.
*/

namespace BaseGE
{
/*                                  CLASSES                                 */
  class Geometry : public Base::Object
  {
  protected:
    /*Points* points;
    Lines* lines;
    Triangles* triangles;
    Quads* quads;
    Polygons* polygons;*/

    /*union
    {
      std::vector<Components> constructionComponents;
      Components* components;
    };*/
    std::vector<Components*> componentsList;

    /*Vertices* vertices;
    Edges* edges;
    Primitives* primitives;
    Surfaces* surfaces;

    VertexData* vertexData;
    EdgeData* edgeData;
    PrimitiveData* primitiveData;
    SurfaceData* surfaceData;*/

    AxisAlignedBoundingBox boundingBox;
    OSuint nPrimitives;

  public:
    inline Geometry() /*: points(0), lines(0), triangles(0), quads(0), polygons(0),
                        vertices(0), edges(0), primitives(0), surfaces(0),
                        vertexData(0), edgeData(0), primitiveData(0), surfaceData(0) */
    {}

    /*inline void setPoints(Points* points) { Geometry::points = points; }
    inline void setLines(Lines* lines) { Geometry::lines = lines; }
    inline void setTriangles(Triangles* triangles) { Geometry::triangles = triangles; }
    inline void setQuads(Quads* quads) { Geometry::quads = quads; }
    inline void setPolygons(Polygons* polygons) { Geometry::polygons = polygons; }*/

    inline void addPoints(Components* points) { componentsList.push_back(points); }
    inline void addLines(Components* lines) { componentsList.push_back(lines); }
    inline void addTriangles(Components* triangles) { componentsList.push_back(triangles); }
    inline void addQuads(Components* quads) { componentsList.push_back(quads); }
    inline void addPolygons(Components* polygons) { componentsList.push_back(polygons); }

    /*inline void setVertices(Vertices* vertices) { Geometry::vertices = vertices; }
    inline void setEdges(Edges* edges) { Geometry::edges = edges; }
    inline void setPrimitives(Primitives* primitives) { Geometry::primitives = primitives; }
    inline void setSurfaces(Surfaces* surfaces) { Geometry::surfaces = surfaces; }

    inline void setVertexData(VertexData* vertexData) { Geometry::vertexData = vertexData; }
    inline void setEdgeData(EdgeData* edgeData) { Geometry::edgeData = edgeData; }
    inline void setPrimitiveData(PrimitiveData* primitiveData) { Geometry::primitiveData = primitiveData; }
    inline void setSurfaceData(SurfaceData* surfaceData) { Geometry::surfaceData = surfaceData; }*/

    inline OSuint countPrimitives()
    {
      return nPrimitives;
    }

    inline void build()
    {
      // Initialize variables
      nPrimitives = 0;

      boundingBox.x1 = MAX_FLOAT;
      boundingBox.x2 = -MAX_FLOAT;
      boundingBox.y1 = MAX_FLOAT;
      boundingBox.y2 = -MAX_FLOAT;
      boundingBox.z1 = MAX_FLOAT;
      boundingBox.z2 = -MAX_FLOAT;

      for(std::vector<Components*>::iterator i = componentsList.begin(); i != componentsList.end(); ++i)
      {
        // Calculate number of primitives in geometry
        OSuint nComponentVertices; // number of component vertices
        Components* components = *i;

        switch(components->componentType)
        {
        case GE_POINT:
          nComponentVertices = 1;
          break;
        case GE_LINE:
          nComponentVertices = 2;
          break;
        case GE_TRIANGLE:
          nComponentVertices = 3;
          break;
        case GE_QUAD:
          nComponentVertices = 4;
          break;
        /* todo:
        case GE_POLYGON:
          nComponentVertices = 0; //NA
          break;*/
        }

        if(components->vertices == 0)
          continue;

        Elements* vertexIndices = cast_id<Elements>(components->vertices->getAttribute(GE_INDEX));
        Elements* vertexPositions;

        if(vertexIndices == 0)
        {
          vertexPositions = cast_id<Elements>(components->vertices->getAttribute(GE_POSITION));
          if(vertexPositions == 0)
            continue;

          nPrimitives += vertexPositions->getLength() / nComponentVertices;
        }
        else
        {
          if(components->vertexData == 0)
            continue;

          vertexPositions = (BaseGE::Elements*)components->vertexData->getAttribute(GE_POSITION);
          if(vertexPositions == 0)
            continue;

          nPrimitives += vertexIndices->getLength() / nComponentVertices;
        }

        // Calculate bounding box
        switch(vertexPositions->getType())
        {
        case OS_VEC3_FLOAT:
          {
            float* elements = (float*)vertexPositions->getData();

            for(uint c = 0; c < vertexPositions->getLength(); ++c)
            {
              boundingBox.x1 = min(boundingBox.x1, elements[3*c + 0]);
              boundingBox.y1 = min(boundingBox.y1, elements[3*c + 1]);
              boundingBox.z1 = min(boundingBox.z1, elements[3*c + 2]);
              boundingBox.x2 = max(boundingBox.x2, elements[3*c + 0]);
              boundingBox.y2 = max(boundingBox.y2, elements[3*c + 1]);
              boundingBox.z2 = max(boundingBox.z2, elements[3*c + 2]);
            }
            break;
          }

        case OS_VEC4_FLOAT:
          {
            float* elements = (float*)vertexPositions->getData();

            for(uint c = 0; c < vertexPositions->getLength(); ++c)
            {
              boundingBox.x1 = min(boundingBox.x1, elements[4*c + 0]);
              boundingBox.y1 = min(boundingBox.y1, elements[4*c + 1]);
              boundingBox.z1 = min(boundingBox.z1, elements[4*c + 2]);
              boundingBox.x2 = max(boundingBox.x2, elements[4*c + 0]);
              boundingBox.y2 = max(boundingBox.y2, elements[4*c + 1]);
              boundingBox.z2 = max(boundingBox.z2, elements[4*c + 2]);
            }
            break;
          }

        case OS_VEC3_DOUBLE:
          {
            double* elements = (double*)vertexPositions->getData();

            for(uint c = 0; c < vertexPositions->getLength(); ++c)
            {
              boundingBox.x1 = min(boundingBox.x1, (float)elements[3*c + 0]);
              boundingBox.y1 = min(boundingBox.y1, (float)elements[3*c + 1]);
              boundingBox.z1 = min(boundingBox.z1, (float)elements[3*c + 2]);
              boundingBox.x2 = max(boundingBox.x2, (float)elements[3*c + 0]);
              boundingBox.y2 = max(boundingBox.y2, (float)elements[3*c + 1]);
              boundingBox.z2 = max(boundingBox.z2, (float)elements[3*c + 2]);
            }
            break;
          }

        case OS_VEC4_DOUBLE:
          {
            double* elements = (double*)vertexPositions->getData();

            for(uint c = 0; c < vertexPositions->getLength(); ++c)
            {
              boundingBox.x1 = min(boundingBox.x1, (float)elements[4*c + 0]);
              boundingBox.y1 = min(boundingBox.y1, (float)elements[4*c + 1]);
              boundingBox.z1 = min(boundingBox.z1, (float)elements[4*c + 2]);
              boundingBox.x2 = max(boundingBox.x2, (float)elements[4*c + 0]);
              boundingBox.y2 = max(boundingBox.y2, (float)elements[4*c + 1]);
              boundingBox.z2 = max(boundingBox.z2, (float)elements[4*c + 2]);
            }
            break;
          }

        default:
          continue; //error
        }
      }

      //calculateBoundingBox();
    }

    /*inline void calculateBoundingBox()
    {
      OSid locationElements = 0;

      // Retrieve location elements
      if(vertexData)
        locationElements = vertexData->getAttribute(GE_POSITION);

      if(locationElements == 0 && vertices)
        locationElements = vertices->getAttribute(GE_POSITION);

      if(locationElements == 0)
        return; // error?

      Elements* locationElementsObject = cast_id<Elements>(locationElements);

      // Calculate bounding box
      switch(locationElementsObject->getType())
      {
      case OS_VEC3_FLOAT:
        {
          float* elements = (float*)locationElementsObject->getData();
          boundingBox.x1 = boundingBox.x2 = elements[0];
          boundingBox.y1 = boundingBox.y2 = elements[1];
          boundingBox.z1 = boundingBox.z2 = elements[2];

          for(uint c = 1; c < locationElementsObject->getLength(); ++c)
          {
            boundingBox.x1 = min(boundingBox.x1, elements[3*c + 0]);
            boundingBox.y1 = min(boundingBox.y1, elements[3*c + 1]);
            boundingBox.z1 = min(boundingBox.z1, elements[3*c + 2]);
            boundingBox.x2 = max(boundingBox.x2, elements[3*c + 0]);
            boundingBox.y2 = max(boundingBox.y2, elements[3*c + 1]);
            boundingBox.z2 = max(boundingBox.z2, elements[3*c + 2]);
          }
          break;
        }

      case OS_VEC4_FLOAT:
        {
          float* elements = (float*)locationElementsObject->getData();
          boundingBox.x1 = boundingBox.x2 = elements[0];
          boundingBox.y1 = boundingBox.y2 = elements[1];
          boundingBox.z1 = boundingBox.z2 = elements[2];

          for(uint c = 1; c < locationElementsObject->getLength(); ++c)
          {
            boundingBox.x1 = min(boundingBox.x1, elements[4*c + 0]);
            boundingBox.y1 = min(boundingBox.y1, elements[4*c + 1]);
            boundingBox.z1 = min(boundingBox.z1, elements[4*c + 2]);
            boundingBox.x2 = max(boundingBox.x2, elements[4*c + 0]);
            boundingBox.y2 = max(boundingBox.y2, elements[4*c + 1]);
            boundingBox.z2 = max(boundingBox.z2, elements[4*c + 2]);
          }
          break;
        }

      case OS_VEC3_DOUBLE:
        {
          double* elements = (double*)locationElementsObject->getData();
          boundingBox.x1 = boundingBox.x2 = (float)elements[0];
          boundingBox.y1 = boundingBox.y2 = (float)elements[1];
          boundingBox.z1 = boundingBox.z2 = (float)elements[2];

          for(uint c = 1; c < locationElementsObject->getLength(); ++c)
          {
            boundingBox.x1 = min(boundingBox.x1, (float)elements[3*c + 0]);
            boundingBox.y1 = min(boundingBox.y1, (float)elements[3*c + 1]);
            boundingBox.z1 = min(boundingBox.z1, (float)elements[3*c + 2]);
            boundingBox.x2 = max(boundingBox.x2, (float)elements[3*c + 0]);
            boundingBox.y2 = max(boundingBox.y2, (float)elements[3*c + 1]);
            boundingBox.z2 = max(boundingBox.z2, (float)elements[3*c + 2]);
          }
          break;
        }

      case OS_VEC4_DOUBLE:
        {
          double* elements = (double*)locationElementsObject->getData();
          boundingBox.x1 = boundingBox.x2 = (float)elements[0];
          boundingBox.y1 = boundingBox.y2 = (float)elements[1];
          boundingBox.z1 = boundingBox.z2 = (float)elements[2];

          for(uint c = 1; c < locationElementsObject->getLength(); ++c)
          {
            boundingBox.x1 = min(boundingBox.x1, (float)elements[4*c + 0]);
            boundingBox.y1 = min(boundingBox.y1, (float)elements[4*c + 1]);
            boundingBox.z1 = min(boundingBox.z1, (float)elements[4*c + 2]);
            boundingBox.x2 = max(boundingBox.x2, (float)elements[4*c + 0]);
            boundingBox.y2 = max(boundingBox.y2, (float)elements[4*c + 1]);
            boundingBox.z2 = max(boundingBox.z2, (float)elements[4*c + 2]);
          }
          break;
        }

      default:
        return; //error
      }
    }*/

    inline const AxisAlignedBoundingBox& getBoundingBox() const
    {
      return boundingBox;
    }

    //inline const std::vector<Components*>* getComponentsList() const
    inline std::vector<Components*>* getComponentsList()
    {
      return &componentsList;
    }
  };
}

#endif
