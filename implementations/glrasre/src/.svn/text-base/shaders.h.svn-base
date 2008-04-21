#ifndef __GLRASRE_SHADERS_H__
#define __GLRASRE_SHADERS_H__
//////////////////////////////////////////////////////////////////////////////
//
//    SHADERS.H
//
//    Author:      Rehno Lindeque
//
//    Description: Gpu kernels & shaders for GlRasRE
//
//    Version: 0.1 (informal prototype)
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*    
    DESCRIPTION:
      Shaders used by the GlRasRE backend.
    
    NAMING CONVENTIONS:
      Fragmentshader main programs are denoted with the prefix fs
      Vertexshader main programs are denoted with the prefix vs

      To distinguish between different return types for the same function
      the following suffixes are used:
      f  = float
      f4 = float4\vec4

    IMPLEMENTATION:
      + lookup2DCoord is used to lookup the 2D coordinate into a texture using a 1D index
      + texture2D
*/


using namespace BaseRE;
namespace GlRasRE
{
/*                              HELPER FUNCTIONS                            */
  /*  Get the 2D coordinates of a 1D index into a texture (using [THRANE] technique)
      X-axis coordinate = index % texture width, but if no texture-clamping is used, then the index will automatically
                        wrap around so that the modulo operation isn't needed
      Y-axis coordinate = floor(index / texture width)
      But since both coordinates must also be in the range [0,1], they also need to be divided by texture width and
      texture height respectively. If the texture is square, then texture width can be used instead.
  */
  #define lookup2DCoord \
    "vec2 lookup2DCoord(float index, float width)" \
    "{" \
    "  float oneDivWidth = 1.0 / width;" \
    "  vec2 lookup_const = vec2(oneDivWidth, oneDivWidth * oneDivWidth);" \
    "  vec2 coord = lookup_const * (index + 0.5);"  /* lookup at center of pixel */ \
    "  return coord;" \
    "}"
  
  /* This is the inverse of lookup2DCoord. It calculates a 1D index from a 2D texture coordinate */
  #define lookup1DIndex \
    "vec2 lookup2DCoord(float index, float width)" \
    "{" \
    "  float oneDivWidth = 1.0 / width;" \
    "  vec2 lookup_const = vec2(oneDivWidth, oneDivWidth * oneDivWidth);" \
    "  vec2 coord = lookup_const * (index + 0.5);" /* lookup at center of pixel */ \
    "  return coord;" \
    "}"

    /*  Read the value in a 2D texture using a 1D index. This depends on lookup2DCoord
    */
  #define texture2DIndexf4 \
    "vec4 texture2DIndexf4(sampler2D data, float index, float width)" \
    "{" \
    "  return texture2D(data, lookup2DCoord(index, width));" \
    "}"
  #define texture2DIndexf \
    "float texture2DIndexf(sampler2D data, float index, float width)" \
    "{" \
    "  return texture2D(data, lookup2DCoord(index, width));" \
    "}"

/*                            SHADER PROGRAMS                               */
  /*  Precompute Ambient Occlusion vertex disc areas. This can't be done
      because it requires scatter operations.
  */
  /*const char* fsCalculateAOVertexDiscArea[] =
  { lookup2DCoord,
    
    texture2DIndexf4,
    
    "varying float2 currentPos;"
    "uniform sampler2D discArea;"
    "uniform sampler2D indices;"
    "uniform sampler2D vertices;"
    "void main()"
    "{"
    // initialize variables
    "  float index1 = indices[currentPos*3+0];"
    "  float index2 = indices[currentPos*3+1];"
    "  float index3 = indices[currentPos*3+2];"
    "  vec3 vertex1 = vertices[index1];"
    "  vec3 vertex2 = vertices[index2];"
    "  vec3 vertex3 = vertices[index3];"

    // calculate triangle area using heron's formula
    "  float a = distance( vertex1, vertex2 );"
    "  float b = distance( vertex1, vertex3 );"
    "  float c = distance( vertex2, vertex3 );"
    "  float s = (a + b + c)/2;"
    "  float triangleArea = sqrt(s*(s-a)*(s-b)*(s-c));"
    
    // add to disc areas
    "  discArea[index1] += triangleArea / 3.0;"
    "  discArea[index2] += triangleArea / 3.0;"
    "  discArea[index3] += triangleArea / 3.0;"
    "}" };*/

  /* General-purpose vertex shader for interpolating texture coordinates
  */
  const char* vsTexCoord1 =
    "varying vec2 texcoord;"
    "void main()"
    "{"
      "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
      //"texcoord = vec2(gl_MultiTexCoord0);"
      //"texcoord = vec2(gl_TexCoord[0]);"
      //"texcoord = vec2(1.0-gl_MultiTexCoord0.x, 1.0-gl_MultiTexCoord0.y);"
      //"texcoord = vec2(gl_MultiTexCoord0.x, 1.0-gl_MultiTexCoord0.y);"
      "texcoord = vec2(gl_MultiTexCoord0.x, gl_MultiTexCoord0.y);"
    "}";

  /*  Compute disc occlusion & bent normals

      Algorithm:
        for all receivers
          for all emitters
            subtract emittor-receiver occlusion from receiver accesibility value
          end
        end            
  */
  const char* fsCalculateDiscOcclusion1Pass =
    /* Functions */
    lookup2DCoord
    texture2DIndexf4
    
    "float calcOcclusion(vec3 receiverToEmitter, vec4 receiver, vec4 emitter, float emitterAccesibility)"
    "{"
      "vec3 receiverNormal = receiver.xyz;"
      "vec3 emitterNormal = emitter.xyz;"
      "float rSquared = dot(receiverToEmitter,receiverToEmitter);" //distance between emitter & receiver squared
      "vec3 v = normalize(receiverToEmitter);"
      //"vec3 v = recieverToEmitter;"
      "float receiverArea  = receiver.w;"
      "float emitterArea  = emitter.w;"
      "float occlusion;"
      "const float pi = 3.17;" //temp: in future precalculate area / pi
      
      //Straight 
      //"occlusion = 1 - dot(r, recNormal)"
      
      // GPU Gems 2 Chap 14 implementation:
      //todo 
      "occlusion = (1.0 - inversesqrt(emitterArea/pi + rSquared))" //inversesqrt(emitterArea/rSquared + 1)
      "            * clamp(dot(emitterNormal, v), 0.0, 1.0)"
      "            * clamp(4*dot(receiverNormal, v), 0.0, 1.0);"
      "return occlusion;"
    "}"

    /* Importing Parameters */
    "uniform sampler2D vertices;"    
    "uniform sampler2D normDiscs;"
    "uniform float nEmitters;" // Number of emitters
    "varying vec2 texcoord;"

    /* Main Program */
    "void main()"
    "{"
      // Initialize variables
      "float accesibility = 1.0;"      
      "vec4 receiverNormArea = vec4(texture2D(normDiscs, texcoord));"
      "vec3 receiverVertex   = vec3(texture2D(vertices, texcoord));"
      //"gl_FragData[0] = vec4(texcoord, 0.0, 1.0);"
      //"gl_FragData[0] = vec4(0.5, 0.5, 1.0, 0.5);"
      
      //temp:
      //"gl_FragData[0] = vec4(vec3(dot(normDisc.xyz, vec3(0.0,-1.0, 0.0))), 1.0);"

      // todo: loop through emitters
      "float emitterIndex = 0.0;"
      "while(true)"
      "{"        
        "if(emitterIndex >= nEmitters)"
          "break;"  
        
        // read emitter disc normal & area
        "vec4 emitterNormArea = texture2DIndexf4(normDiscs, emitterIndex, 512.0);"
        "vec3 emitterVertex   = vec3(texture2DIndexf4(vertices,  emitterIndex, 512.0));" //todo: also retrieve accessibility from here??
        //"accesibility -= 0.02;"

        // calculate emitter disc occlusion on receiver
        "float occlusion = calcOcclusion(emitterVertex-receiverVertex, receiverNormArea, emitterNormArea, 0.0);" //todo: use emitter accessibility
        "accesibility -= occlusion;"

        "emitterIndex += 1.0;"
      "}"

      // save result
      "gl_FragData[0] = vec4(receiverNormArea.xyz, accesibility);"
    "}";
  
  /* Render scene with Ambient Occlusion
  */
  const char* vsRenderAO =
    "attribute vec4 occlusion;"
    "varying vec3 color;"
    "void main()"
    "{"
      "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
      "color = vec3(occlusion.a, occlusion.a, occlusion.a);"
      //"color = vec3(occlusion.r, occlusion.g, occlusion.b);"
    "}";
;
  const char* fsRenderAO =
    "varying vec3 color;"
    "void main()"
    "{"
      //"gl_FragColor = vec4(color, 1.0);"
      "gl_FragColor = vec4(color, 1.0);"
    "}";
};

#endif
