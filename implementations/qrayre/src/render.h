#ifndef __QRAYRE_RENDER_H__
#define __QRAYRE_RENDER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    RENDER.H
//
//    Copyright © 2006-2007, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      QRayRE render class

    TODO: 
      + In future perhaps ignore axis-aligned rays and treat them seperately
        as special cases to avoid all the branching where division happens
*/
/*                                 INCLUDES                                 */
// Raytracing algorithms
#include "intersection/raytri/mollertrumbore.h"
#include "intersection/raytri/segurafeito.h"
#include "intersection/raytri/guiguedevillers.h"

// Distance algorithms
#include "distance/tripoint/segurafeito.h"

// Traversal algorithms
#include "bih/types.h"
//#include "bih/recursivetraversal.h"
#include "bih/stacktraversal.h"

/*                                 CLASSES                                  */
using namespace BaseRE;
namespace QRayRE
{
  class QRayRender : public BaseRE::Render
  {
  protected:
    float* primitiveData;
    BIH_NodeHeader* structureData;

    inline void BIH_Raytrace(uint32* renderBuffer)
    {
      QRayScene& qrayScene = *(QRayScene*)scene;
      primitiveData = *(float**)qrayScene.spatialStructure;
      structureData = (BIH_NodeHeader*)((float**)qrayScene.spatialStructure + 1);

      //BIH_NodeHeader* node;
      
      // Calculate camera paramters
      QRayCamera& qrayCamera = *(QRayCamera*)camera;
      Vector3f rayOrigin(qrayCamera.position);
      Vector3f dx, dy, dz;
      qrayCamera.getCameraAxis(dx, dy, dz);

      // Cull root nodes for camera frustrum
      BIH_NodeHeader* rayRootNode;
      rayRootNode = structureData;
      // todo: (frustrum cull)

      //// Calculate initial node stack for ray origin
      //if(qrayScene.boundingBox.contains(rayOrigin))
      //{
      //  while(true)
      //  {
      //    if(node->nodeType == LEAF)
      //      break;

      //    // test whether position is on the left or right side of the node
      //    if(rayOrigin(node->nodeType) <= node->leftBoundary)
      //    {
      //    }
      //    else if(rayOrigin(node->nodeType) >= node->rightBoundary)
      //    {
      //    }
      //    else
      //    {
      //    }

      //    //node->leftBoundary
      //  }
      //}
      //else
      //{
      //  //todo: necessary?
      //}
      
      // Cast rays
      for(uint cy = 0; cy < 600; ++cy)
      {
        for(uint cx = 0; cx < 800; ++cx)
        {
          BASEREDBG_RASTERPOSITION(cx,600-cy)

          // Generate a ray through the screen pixel
          const float aspectRatio = (float)(800.0 / 600.0);
          const float zDistance = 1.5f;
          float scanX = cx/400.0f - 1.0f + (float)(1.0/800.0);
          float scanY = cy/300.0f - 1.0f + (float)(1.0/600.0);
          
          Vector3f rayDirection = scanX * aspectRatio * dx + scanY * dy + zDistance * dz;
          rayDirection.normalize();

          // Cast ray
          float distance;
          Vector2f uv;
          uint32 primitiveId;

          if(BIH_Ray_Intersect(primitiveId, distance, uv, qrayScene.boundingBox, (Vector3f*)primitiveData, rayRootNode, rayOrigin, rayDirection))
          {
            const Vector3f* tri = ((Vector3f*)primitiveData) + primitiveId;

            //// Shade fragment
            // Lighting
            Vector3f intersection = rayOrigin + distance * rayDirection;
            Vector3f e1, e2;
            Vector3f normal;
            e1 = tri[1] - tri[0];
            e2 = tri[2] - tri[0];
            normal = Vector3f::cross(e2, e1).normalize();

            //renderBuffer[cy*800+cx] = 0xffffffff;

            float shade = 0.0f;

            for(uint c = 0; c < qrayScene.nLights; ++c)
            {
              QRayLight& light = *qrayScene.lights[c];
              Vector3f lightVector = (light.position - intersection);
              float lightDistance = lightVector.getLength();
              lightVector /= lightDistance;

              // Cast shadow ray
              //todo: reuse ray stack & use light bounding distance
              if(BIH_Segment_Test(tri, (Vector3f*)primitiveData, structureData, intersection, lightVector, lightDistance))
                continue;

              // Calculate shading
              shade += max(Vector3f::dot(normal, lightVector), 0.0f);
            }

            //// Render fragment
            uint8 val = (uint8)(min(shade*255.0f, 255.0f));
            renderBuffer[cy*800+cx] = val + (val << 8) + (val << 16);
          }
          else
            renderBuffer[cy*800+cx] = 0x00000000;
        }
      }
    }

  public:
    inline QRayRender() : primitiveData(null), structureData(null)
    {}

    inline void render()
    {
      if(scene == 0)
        return; //error    

      // render test quad
      //glClear(GL_COLOR_BUFFER_BIT);
      //glColor3f(1.0f, 1.0f, 1.0f);
      //glBegin(GL_QUADS);
      //  glVertex3f(-1.0f, 1.0f, 0.0f);
      //  glVertex3f( 1.0f, 1.0f, 0.0f);
      //  glVertex3f( 1.0f,-1.0f, 0.0f);
      //  glVertex3f(-1.0f,-1.0f, 0.0f);
      //glEnd();

      // create framebuffer
      uint32* renderBuffer = new uint32[800*600];

      // render framebuffer
      BIH_Raytrace(renderBuffer);

      // draw framebuffer
      //glRasterPos2i(-400,-300);
      glDrawPixels(800, 600, GL_RGBA, GL_UNSIGNED_BYTE, renderBuffer);
      glFlush();

      // delete framebuffer
      delete[] renderBuffer;
    }

    virtual ~QRayRender()
    {}
  };
}

/*                                  MACROS                                  */
#undef BIT_INVERSE

#endif
