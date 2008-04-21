#ifndef __COLLADATEMPLATE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    COLLADATEMPLATE.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*    
    DESCRIPTION:
      TemplateXML template for Collada DAE files

    TODO:
      + Load indices as uint16 rather than int.
*/
/*                                DEFINITIONS                               */
#ifndef TEMPLATEXML_TEMPLATE_NAMESPACE
  #define TEMPLATEXML_TEMPLATE_NAMESPACE ColladaTemplate
#endif

/*                                 INCLUDES                                */
#include "../xmltemplate.h"

/*                                XML TEMPLATE                             */

TEMPLATEXML_BEGIN_TEMPLATE
  
  TEMPLATEXML_BEGIN_ELEMENT(Optics)
		//		<technique_common>
		//			<perspective>
		//				<yfov>49.13434</yfov>
		//				<znear>0.1</znear>
		//				<zfar>100.0</zfar>
		//			</perspective>
		//		</technique_common>
		//	</optics>
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(Camera)
    //<camera id="Camera" name="Camera">  
    TEMPLATEXML_ELEMENT(Optics, optics, "optics")
    //</camera>
  TEMPLATEXML_END_ELEMENT
  
  TEMPLATEXML_BEGIN_ELEMENT(LibraryCameras)
		TEMPLATEXML_ELEMENT(Camera, camera, "camera")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(FloatArray)
    //TEMPLATEXML_ATTRIBUTE(int, "count")    
    TEMPLATEXML_ID_ATTRIBUTE("id")
    TEMPLATEXML_ARRAY_CONTENTS(float)
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(Param)
    //<param name="X" type="float"/>
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(Accessor)
    //TEMPLATEXML_ATTRIBUTE(int, "count")
    TEMPLATEXML_REFERENCE_ATTRIBUTE(FloatArray, source, "source")
    //TEMPLATEXML_ATTRIBUTE(int, "stride")
    TEMPLATEXML_ELEMENT(Param, param, "param") //todo: These should probably be ordered
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(Source_TechniqueCommon)
    TEMPLATEXML_ELEMENT(Accessor, accessor, "accessor")
  TEMPLATEXML_END_ELEMENT
  
  TEMPLATEXML_BEGIN_ELEMENT(Source)
    TEMPLATEXML_ID_ATTRIBUTE("id")
    TEMPLATEXML_ELEMENT(FloatArray, floatArray, "float_array")
		TEMPLATEXML_ELEMENT(Source_TechniqueCommon, technique, "technique_common")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ENUM(InputSemantic)
    TEMPLATEXML_ENUM(POSITION, "POSITION")
    TEMPLATEXML_ENUM(VERTEX, "VERTEX")
    TEMPLATEXML_ENUM(NORMAL, "NORMAL")
  TEMPLATEXML_END_ENUM

  TEMPLATEXML_BEGIN_ELEMENT(Input)
    //TEMPLATEXML_ATTRIBUTE(int, "offset")
    TEMPLATEXML_ENUM_ATTRIBUTE(InputSemantic, semantic, "semantic") // POSITION \ NORMAL \ VERTEX
    TEMPLATEXML_VOID_REFERENCE_ATTRIBUTE(source, "source")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(Vertices)
    TEMPLATEXML_ID_ATTRIBUTE("id")
    TEMPLATEXML_ELEMENT(Input, input, "input")
  TEMPLATEXML_END_ELEMENT 

  TEMPLATEXML_BEGIN_ELEMENT(Triangles)
    //TEMPLATEXML_ATTRIBUTE(int, "count")
    //TEMPLATEXML_ATTRIBUTE(string, "material")	
    TEMPLATEXML_ELEMENT(Input, input, "input")
    TEMPLATEXML_ARRAY_ELEMENT(int, p, "p")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(Polygons)
    //TEMPLATEXML_ATTRIBUTE(int, "count")
    //TEMPLATEXML_ATTRIBUTE(string, "material")
    TEMPLATEXML_ELEMENT(Input, input, "input")
    TEMPLATEXML_ARRAY_ELEMENT(int, p, "p")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(Mesh)
    TEMPLATEXML_ELEMENT(Source, source, "source")
    TEMPLATEXML_ELEMENT(Vertices, vertices, "vertices")
    TEMPLATEXML_ELEMENT(Triangles, triangles, "triangles")
    TEMPLATEXML_ELEMENT(Polygons, polygons, "polygons")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(Geometry)
    TEMPLATEXML_ID_ATTRIBUTE("id")
    //TEMPLATEXML_STRING_ATTRIBUTE("name")

    TEMPLATEXML_ELEMENT(Mesh, mesh, "mesh")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(LibraryGeometries) 
    TEMPLATEXML_ELEMENT(Geometry, geometry, "geometry")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(LibraryLights) 
	//	<light id="Spot" name="Spot">
	//		<technique_common>
	//			<point>
	//				<color>1.0 1.0 1.0</color>
	//				<constant_attenuation>0.0</constant_attenuation>
	//				<linear_attenuation>0.0</linear_attenuation>
	//				<quadratic_attenuation>0.0</quadratic_attenuation>
	//			</point>
	//		</technique_common>
	//	</light>
	TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(InstanceGeometry)
    TEMPLATEXML_REFERENCE_ATTRIBUTE(Geometry, ref, "url")
    //		<instance_geometry url="#Cylinder_002">
		//			<bind_material>
		//				<technique_common>
		//					<instance_material symbol="Material" target="#Material"/>
		//				</technique_common>
		//			</bind_material>
		//		</instance_geometry>
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(Node)
    TEMPLATEXML_ID_ATTRIBUTE("id")
    //TEMPLATEXML_STRING_ATTRIBUTE("name")
    TEMPLATEXML_ARRAY_ELEMENT(float, scale, "scale")
    TEMPLATEXML_ARRAY_ELEMENT(float, rotate, "rotate")
    TEMPLATEXML_ARRAY_ELEMENT(float, translate, "translate")
    TEMPLATEXML_ELEMENT(InstanceGeometry, instanceGeometry, "instance_geometry")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(VisualScene)
    TEMPLATEXML_ID_ATTRIBUTE("id")
    //TEMPLATEXML_STRING_ATTRIBUTE("name")
    TEMPLATEXML_ELEMENT(Node, node, "node")		
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(LibraryVisualScenes)
		TEMPLATEXML_ELEMENT(VisualScene, visualScene, "visual_scene")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(InstanceVisualScene)
    TEMPLATEXML_REFERENCE_ATTRIBUTE(VisualScene, ref, "url")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(Scene)
	  TEMPLATEXML_ELEMENT(InstanceVisualScene, instanceVisualScene, "instance_visual_scene")
  TEMPLATEXML_END_ELEMENT

  TEMPLATEXML_BEGIN_ELEMENT(Collada)
    TEMPLATEXML_ELEMENT(LibraryCameras, libraryCameras, "library_cameras")
    TEMPLATEXML_ELEMENT(LibraryGeometries, libraryGeometries, "library_geometries")
    TEMPLATEXML_ELEMENT(LibraryLights, libraryLights, "library_lights")
    TEMPLATEXML_ELEMENT(LibraryVisualScenes, libraryVisualScenes, "library_visual_scenes")
    TEMPLATEXML_ELEMENT(Scene, scene, "scene")
  TEMPLATEXML_END_ELEMENT
TEMPLATEXML_END_TEMPLATE

/*                                   MACROS                                */
#ifndef TEMPLATEXML_LOOP_DONE
  #include "colladatemplate.h"
#endif
#define __COLLADATEMPLATE_H__


#endif
