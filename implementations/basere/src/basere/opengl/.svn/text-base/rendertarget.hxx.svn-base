#ifdef __BASERE_RENDERTARGET_H__
#ifndef __BASERE_RENDERTARGET_HXX__
#define __BASERE_RENDERTARGET_HXX__
//////////////////////////////////////////////////////////////////////////////
//
//    RENDERTARGET.HXX
//
//    Author:      Rehno Lindeque
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */

/*                               IMPLEMENTATION                             */
inline GlRenderTarget::GlRenderTarget() : frameBufferObject(0)//, active(false)
{ 
  //memset(inputStreams, 0, 8*sizeof(bool));
}

inline void GlRenderTarget::create()
{
  glGenFramebuffersEXT(1, &frameBufferObject);
}

inline void GlRenderTarget::bind()
{ 
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject);
  //active = true;
}

void GlRenderTarget::unbind()
{ 
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

inline void GlRenderTarget::destroy()
{ 
  //restore();
  glDeleteFramebuffersEXT(1, &frameBufferObject);
}

#endif
#endif
