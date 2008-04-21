#ifndef __GLRASRE_GEOMETRY_H__
#define __GLRASRE_GEOMETRY_H__
//////////////////////////////////////////////////////////////////////////////
//
//    GEOMETRY.H
//
//    Author:      Rehno Lindeque
//
//    Description: GlRasRE classes used to implement api
//
//    Version: 0.1 (informal prototype)
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      Geometry implementation for opengl using PBO's fpr VBO (etc) and texture
      storage.

    IMPLEMENTATION:
      + Currently the implementation assumes (requires) 16bit unsigned
        integer format for indexes.

    DEPENDENCIES:
      stream.h
*/
/*                              COMPILER MACROS                             */
#ifndef __BASERE_STREAM_H__
  #error "stream.h must be included before geometry.h"
#endif

/*                                   CLASSES                                */
using namespace BaseRE;
namespace GlRasRE
{
  class GlRasMesh : public Mesh
  {
  protected:
    /*float* vertices;
    uint16* indices;*/
  public:
    /*GlStream vertexStream;
    GlStream indexStream;
    //GlStream discStream; // for ambient occlusion
    GlStream normDiscStream; // vertex normals and disc area for ambient occlusion

    void create(uint sizeX, uint sizeY /* todo:, flag = STATIC_GEOMETRY *)
    {
      vertexStream.create(sizeX, sizeY, VEC4_FLOAT16_STREAM, null);
      indexStream.create(sizeX, sizeY, FIXED16_STREAM, null);
      //discStream.create(vertexStream.getLength(), VEC4_FLOAT16_STREAM, null);
      //discStream.create(sizeX, sizeY, FLOAT16_STREAM, null);
      normDiscStream.create(sizeX, sizeY, VEC4_FLOAT16_STREAM, null);

      /*DEPRICATED: how big will the indexStream be? For now: size/2
      indexStream.create((uint)ceil(sizeX/2.0), (uint)ceil(sizeY/2.0), VEC4_FIXED8_STREAM, null);*

      vertices = (float*)vertexStream.map(WRITE_ONLY);
      indices = (uint16*)indexStream.map(WRITE_ONLY);
    }*/

    void build()
    {
      Component* component;
      if(points == null)
      {
        if(vertices == null)
          return;
        else
          component = vertices;
      }
      else
        component = points;

      // calculate bent normals and disc areas (for use in ambient occlusion calculation)
      if(component->getAttributes(RE_EXT_BENT_NORMAL_AREA) == 0)
      {
        // determine number of points in the mesh
        REid locations = component->getAttributes(RE_LOCATION);
        REuint nElements = reGetElementsLength(locations);

        // create an elements set for bent normals and point disc areas
        REid normAreaElements = reBeginElements(RE_VEC4_FLOAT, nElements);
          float normArea[4] = {0.0f, 0.0f, 0.0f, 2.0f};
          for(uint c = 0; c < nElements; ++c)
          {
            reElement(normArea);
          }
        reEndElements();

        component->addAttributes(RE_EXT_BENT_NORMAL_AREA, normAreaElements);
      }
    }

    /* old void build()
    {
      //calculate normals & vertex-disc sizes for ambient occlusion
      if(true) // (if ambient occlusion is on? - but what about normals)
      {
        //HVector3* vertices = (HVector3*)GlRasGeometry::vertices;
        HCoord3* vertices = (HCoord3*)GlRasGeometry::vertices;

        // create & open stream for disc areas
        //discStream.create(vertexStream.getLength(), FLOAT16_STREAM, null);
        //discStream.create(vertexStream.getLength(), VEC4_FLOAT16_STREAM, null);
        float* discs = (float*)normDiscStream.map(WRITE_ONLY);
        memset(discs, 0, normDiscStream.getMappedBufferSize());

        for(uint c = 0; c < indexStream.getLength()/3; c++) // for each triangle
        {
          // initialize variables
          //Vector3 v1(&vertices[indices[c*3+0]*4]); //note: 3 indices\vertices per triangle, 4 floats per vertex
          uint16 i1 = indices[c*3+0];
          uint16 i2 = indices[c*3+1];
          uint16 i3 = indices[c*3+2];

          //HVector3& v1 = vertices[i1];
          //HVector3& v2 = vertices[i2];
          //HVector3& v3 = vertices[i3];
          Vector3& v1 = (Vector3&)vertices[i1];
          Vector3& v2 = (Vector3&)vertices[i2];
          Vector3& v3 = (Vector3&)vertices[i3];

          // calculate 2 sides (to be used for normal calculation later)
          Vector3 s1 = v1-v2;
          Vector3 s2 = v2-v3;

          // calculate triangle side lengths
          float l1 = s1.getLength(); //(v1-v2).getLength();
          float l2 = s2.getLength(); //(v2-v3).getLength();
          float l3 = (v3-v1).getLength();

          // calculate triangle area using heron's formula
          float s = (l1 + l2 + l3)/2.0f;
          float triangleArea = sqrt(s*(s-l1)*(s-l2)*(s-l3));

          // add to disc areas
          discs[i1*4+3] += triangleArea / 3.0f;
          discs[i2*4+3] += triangleArea / 3.0f;
          discs[i3*4+3] += triangleArea / 3.0f;

          // calculate triangle normal
          Vector3 n = Vector3::cross(s1, s2).normalize();

          // add triangle normal to each vertex
          Vector3& n1 = *(Vector3*)&discs[i1*4];
          Vector3& n2 = *(Vector3*)&discs[i2*4];
          Vector3& n3 = *(Vector3*)&discs[i3*4];
          n1 += n;
          n2 += n;
          n3 += n;
        }

        // normalize vertex normals
        for(uint c = 0; c < vertexStream.getLength()/3; c++) // for each vertex \ disc
        {
          Vector3& n = *(Vector3*)&discs[c*4];
          n.normalize();
        }

        // close stream for normals & disc areas
        normDiscStream.unmap();
        discs = null;
      }

      // close vertex and index streams
      vertexStream.unmap();
      indexStream.unmap();
      vertices = null;
      indices = null;
    }*/

    /*old: void addQuads(const Quads& quads)
    {
      // convert quads to triangles
      uint nTriangles = quads.nQuads*2;
      /*short* indices = null;
      float* vertices = null;*

      // create vertices
      if(quads.vertexStride == /*sizeof(float)*4* vertexStream.getMappedElementSize())
        //vertices = quads.vertices;
        vertexStream.appendCopy(quads.vertices, quads.nVertices);
      else
      {
        float* vertices = (float*)vertexStream.appendCreate(quads.nVertices); //DEPRICATED: vertices = new float[quads.nVertices*4];
        for(uint c = 0; c < quads.nQuads; ++c)
        {
          ((HPoint3*)vertices)[c*4+0] = quads.getVertex(c,0);
          ((HPoint3*)vertices)[c*4+1] = quads.getVertex(c,1);
          ((HPoint3*)vertices)[c*4+2] = quads.getVertex(c,2);
          ((HPoint3*)vertices)[c*4+3] = quads.getVertex(c,3);
        }
      }

      // create indices
      if(quads.indices != null)
      {
        // initialize temporary variables
        uint nIndices = nTriangles * 3;

        uint16* indices = (uint16*)indexStream.appendCreate(nIndices); //DEPRICATED: indices = new short[nIndices];

        // convert quad indices to triangle indices
        for(uint c = 0; c < quads.nQuads; ++c)
        {
          // split quad along shortest cross-connection (note: we're working with square length for efficiency reasons)
          //todo: this could probably be optimized further (maybe look into it later)
          Vector3 v0(quads.getVertex(c,0));
          Vector3 v1(quads.getVertex(c,1));
          Vector3 v2(quads.getVertex(c,2));
          Vector3 v3(quads.getVertex(c,3));

          float len1 = (v0 - v2).getSquareLength();
          float len2 = (v1 - v3).getSquareLength();

          if(len1 < len2)
          {
            // copy quad indices 0,1,2 -> triangle indices 0,1,2
            indices[c*6+0] = quads.getIndex(c,0);
            indices[c*6+1] = quads.getIndex(c,1);
            indices[c*6+2] = quads.getIndex(c,2);

            // copy quad indices 2,3,0 -> triangle indices 3,4,5
            indices[c*6+3] = quads.getIndex(c,2);
            indices[c*6+4] = quads.getIndex(c,3);
            indices[c*6+5] = quads.getIndex(c,0);
          }
          else
          {
            // copy quad indices 0,1,3 -> triangle indices 0,1,2
            indices[c*6+0] = quads.getIndex(c,0);
            indices[c*6+1] = quads.getIndex(c,1);
            indices[c*6+2] = quads.getIndex(c,3);

            // copy quad indices 1,2,3 -> triangle indices 3,4,5
            indices[c*6+3] = quads.getIndex(c,1);
            indices[c*6+4] = quads.getIndex(c,2);
            indices[c*6+5] = quads.getIndex(c,3);
          }
        }
      }
      else
      {
        // initialize temporary variables
        uint nIndices = nTriangles * 3;
        uint16* indices = (uint16*)indexStream.appendCreate(nIndices); //DEPRICATED: indices = new short[nIndices];
        uint firstIndex = vertexStream.getLength() - quads.nVertices;

        // create triangle indices
        for(uint c = 0; c < quads.nQuads; ++c)
        {
          // split quad along shortest cross-connection (note: we're working with square length for efficiency reasons)
          //todo: this could probably be optimized further (maybe look into it later)
          Vector3 v0(quads.getVertex(c,0));
          Vector3 v1(quads.getVertex(c,1));
          Vector3 v2(quads.getVertex(c,2));
          Vector3 v3(quads.getVertex(c,3));

          float len1 = (v0 - v2).getSquareLength();
          float len2 = (v1 - v3).getSquareLength();

          if(len1 < len2)
          {
            // set quad vertices 0,1,2 -> triangle indices 0,1,2
            indices[c*6+0] = firstIndex+c*4+0;
            indices[c*6+1] = firstIndex+c*4+1;
            indices[c*6+2] = firstIndex+c*4+2;

            // copy quad vertices 2,3,0 -> triangle indices 3,4,5
            indices[c*6+3] = firstIndex+c*4+2;
            indices[c*6+4] = firstIndex+c*4+3;
            indices[c*6+5] = firstIndex+c*4+0;
          }
          else
          {
            // copy quad vertices 0,1,3 -> triangle vertices 0,1,2
            indices[c*6+0] = firstIndex+c*4+0;
            indices[c*6+1] = firstIndex+c*4+1;
            indices[c*6+2] = firstIndex+c*4+3;

            // copy quad vertices 1,2,3 -> triangle vertices 3,4,5
            indices[c*6+3] = firstIndex+c*4+1;
            indices[c*6+4] = firstIndex+c*4+2;
            indices[c*6+5] = firstIndex+c*4+3;
          }
        }
      }
      /* TEMPORARILY DEPRICATED: Note - this is needed if indices aren't used
      else
      {
        // initialize temporary variables
        uint nVertices = nTriangles * 3;
        float* vertices = new float[nTriangles*3*4];

        // convert quad vertices to triangle vertices
        for(uint c = 0; c < quads.nQuads; ++c)
        {
          // split quad along shortest cross-connection (note: we're working with square length for efficiency reasons)
          //todo: this could probably be optimized further (maybe look into it later)
          Vector3 v0(quads.getVertex(c,0));
          Vector3 v1(quads.getVertex(c,1));
          Vector3 v2(quads.getVertex(c,2));
          Vector3 v3(quads.getVertex(c,3));

          float len1 = (v0 - v2).getSquareLength();
          float len2 = (v1 - v3).getSquareLength();

          if(len1 < len2)
          {
            // copy quad vertices 0,1,2 -> triangle vertices 0,1,2
            /* old: memcpy(&vertices[(c*6+0)*3], &quads.vertices[(c*4+0)*3], 3*3*sizeof(float)); *
            *(HPoint3*)((int8*)vertices + (c*6+0) * 4*sizeof(float)) = v0;
            *(HPoint3*)((int8*)vertices + (c*6+1) * 4*sizeof(float)) = v1;
            *(HPoint3*)((int8*)vertices + (c*6+2) * 4*sizeof(float)) = v2;

            // copy quad vertices 2,3,0 -> triangle vertices 3,4,5
            /* old: memcpy(&vertices[(c*6+3)*3], &quads.vertices[(c*4+2)*3], 2*3*sizeof(float));
            memcpy(&vertices[(c*6+5)*3], &quads.vertices[(c*4+0)*3], 1*3*sizeof(float)); *
            *(HPoint3*)((int8*)vertices + (c*6+3) * 4*sizeof(float)) = v2;
            *(HPoint3*)((int8*)vertices + (c*6+4) * 4*sizeof(float)) = v3;
            *(HPoint3*)((int8*)vertices + (c*6+5) * 4*sizeof(float)) = v0;
          }
          else
          {
            // copy quad vertices 0,1,3 -> triangle vertices 0,1,2
            /*old: memcpy(&vertices[(c*6+0)*3], &quads.vertices[(c*4+0)*3], 2*3*sizeof(float));
            memcpy(&vertices[(c*6+2)*3], &quads.vertices[(c*4+3)*3], 1*3*sizeof(float)); *
            *(HPoint3*)((int8*)vertices + (c*6+0) * 4*sizeof(float)) = v0;
            *(HPoint3*)((int8*)vertices + (c*6+1) * 4*sizeof(float)) = v1;
            *(HPoint3*)((int8*)vertices + (c*6+2) * 4*sizeof(float)) = v3;

            // copy quad vertices 1,2,3 -> triangle vertices 3,4,5
            /* old: memcpy(&vertices[(c*6+3)*3], &quads.vertices[(c*4+1)*3], 3*3*sizeof(float)); *
            *(HPoint3*)((int8*)vertices + (c*6+3) * 4*sizeof(float)) = v1;
            *(HPoint3*)((int8*)vertices + (c*6+4) * 4*sizeof(float)) = v2;
            *(HPoint3*)((int8*)vertices + (c*6+5) * 4*sizeof(float)) = v3;
          }
        }
      }*/

      /* DEPRICATED:
      // append triangle data to the buffer
      vertexStream.append(vertices, nTriangles*3);
      indexStream.append(indices, nIndices);

      // destroy temporary buffer
      delete[] vertices;*
    }


    /*
    void oldAddQuads(const Quads& quads)
    {
      // convert quads to triangles
      uint nTriangles = quads.nQuads*2;

      // create vertices

      // initialize temporary variables
      uint nVertices = nTriangles * 3;
      float* vertices = new float[nTriangles*3*4];

      // convert quad vertices to triangle vertices
      for(uint c = 0; c < quads.nQuads; ++c)
      {
        // split quad along shortest cross-connection (note: we're working with square length for efficiency reasons)
        //todo: this could probably be optimized further (maybe look into it later)
        Vector3 v0(quads.getVertex(c,0));
        Vector3 v1(quads.getVertex(c,1));
        Vector3 v2(quads.getVertex(c,2));
        Vector3 v3(quads.getVertex(c,3));

        float len1 = (v0 - v2).getSquareLength();
        float len2 = (v1 - v3).getSquareLength();

        if(len1 < len2)
        {
          // copy quad vertices 0,1,2 -> triangle vertices 0,1,2
          /* old: memcpy(&vertices[(c*6+0)*3], &quads.vertices[(c*4+0)*3], 3*3*sizeof(float)); *
          *(HPoint3*)((int8*)vertices + (c*6+0) * 4*sizeof(float)) = v0;
          *(HPoint3*)((int8*)vertices + (c*6+1) * 4*sizeof(float)) = v1;
          *(HPoint3*)((int8*)vertices + (c*6+2) * 4*sizeof(float)) = v2;

          // copy quad vertices 2,3,0 -> triangle vertices 3,4,5
          /* old: memcpy(&vertices[(c*6+3)*3], &quads.vertices[(c*4+2)*3], 2*3*sizeof(float));
          memcpy(&vertices[(c*6+5)*3], &quads.vertices[(c*4+0)*3], 1*3*sizeof(float)); *
          *(HPoint3*)((int8*)vertices + (c*6+3) * 4*sizeof(float)) = v2;
          *(HPoint3*)((int8*)vertices + (c*6+4) * 4*sizeof(float)) = v3;
          *(HPoint3*)((int8*)vertices + (c*6+5) * 4*sizeof(float)) = v0;
        }
        else
        {
          // copy quad vertices 0,1,3 -> triangle vertices 0,1,2
          /*old: memcpy(&vertices[(c*6+0)*3], &quads.vertices[(c*4+0)*3], 2*3*sizeof(float));
          memcpy(&vertices[(c*6+2)*3], &quads.vertices[(c*4+3)*3], 1*3*sizeof(float)); *
          *(HPoint3*)((int8*)vertices + (c*6+0) * 4*sizeof(float)) = v0;
          *(HPoint3*)((int8*)vertices + (c*6+1) * 4*sizeof(float)) = v1;
          *(HPoint3*)((int8*)vertices + (c*6+2) * 4*sizeof(float)) = v3;

          // copy quad vertices 1,2,3 -> triangle vertices 3,4,5
          /* old: memcpy(&vertices[(c*6+3)*3], &quads.vertices[(c*4+1)*3], 3*3*sizeof(float)); *
          *(HPoint3*)((int8*)vertices + (c*6+3) * 4*sizeof(float)) = v1;
          *(HPoint3*)((int8*)vertices + (c*6+4) * 4*sizeof(float)) = v2;
          *(HPoint3*)((int8*)vertices + (c*6+5) * 4*sizeof(float)) = v3;
        }
      }
      // append triangle data to the buffer
      vertexStream.appendCopy(vertices, nTriangles*3);

      // destroy temporary buffer
      delete[] vertices;
    }*/

    void destroy()
    {
      /*indexStream.destroy();
      vertexStream.destroy();
      normDiscStream.destroy();*/
    }
  };
};

#endif
