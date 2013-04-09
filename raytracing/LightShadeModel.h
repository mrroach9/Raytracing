#ifndef _LIGHT_SHADE_MODEL_H_
#define _LIGHT_SHADE_MODEL_H_

#include <iostream>
#include "Geometry.h"
#include "Model.h"
#include "LightSource.h"
#include "Color3.h"

#define MASK_AMBIENT	0x00000001
#define MASK_DIFFUSE	0x00000002
#define MASK_SPECULAR	0x00000004

typedef enum _LIGHTMODEL{
	PhongLighting
} LightModel;

typedef enum _SHADEMODEL{
	FacetedShading, PhongShading
} ShadeModel;

class LightShadeModel{
public:
	static LightModel lightModel;
	static ShadeModel shadeModel;

public:
	static void setLightModel(LightModel lm);
	static void setShadeModel(ShadeModel sm);
	// ColorMask shows the components included: Ambient, diffuse and/or specular
	static Color3 calcColor(
		Vector3D pos, Vector3D eyeDir, Vector3D n, Color3 color, 
		LightSource *ls, Model *m, UINT face_id, UINT colorMask);

public:
	static Vector3D calcNormal(Vector3D pos, Model* m, UINT face_id);
	//TODO: Phong Normal Interpolation
	static Vector3D calcPhongNormal(Vector3D pos, Model *m, UINT face_id);
	//No Ambient
	static Color3 calcPhongLighting(
		Vector3D n, Vector3D v, Vector3D l,
		Color3 v_color, Color3 ls_clr,
		Material* obj_mat, UINT colorMask);
};

#endif // _LIGHT_SHADE_MODEL_H_