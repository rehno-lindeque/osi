#ifndef __GLRASRE_API_H__
#define __GLRASRE_API_H__
//////////////////////////////////////////////////////////////////////////////
//
//    API.H
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

// BaseRE
#include <basere/basere.h>
#include <basere/opengl/stream.h>
#include <basere/opengl/shader.h>
#include <basere/opengl/kernel.h>
#include <basere/opengl/rendertarget.h>

#include "geometry.h"
#include "elements.h"
#include "scene.h"
#include "frame.h"
#include "shaders.h"

using namespace BaseRE;

namespace GlRasRE
{
/*                               CONSTANTS                                  */
/* Render Parameters */
  enum RE_GI_PARAM
  {
    RE_AMBIENT_OCCLUSION
    //RE_PHOTON_MAPPING
    //RE_RADIOSITY
  };

  const REParameterInfo globalIlluminationInfo[] =
  { { RE_AMBIENT_OCCLUSION, 1, 0, "Ambient Occlusion" } };

  const RESettingInfo ambientOcclusionQualityInfo[] =
  { { (REuint)-1, "Default Quality" }, { 0, "Per-vertex Disc Technique" }, { 1, "Per-face Disc Technique" }/*, { 2, "Monty-carlo Ray Casting" }*/ };
  /*struct RESettingInfo
  {
    REuint setting;
    REstring description;
  };*/
};

#endif
