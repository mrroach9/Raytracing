#include "LightShadeModel.h"

LightModel LightShadeModel::lightModel = PhongLighting;
ShadeModel LightShadeModel::shadeModel = PhongShading;

void LightShadeModel::setLightModel(LightModel lm){
	lightModel = lm;
}

void LightShadeModel::setShadeModel(ShadeModel sm){
	shadeModel = sm;
}

Color3 LightShadeModel::calcColor(
		Vector3D pos, Vector3D dir, Vector3D n, Color3 color,
		LightSource *ls, Model *m, UINT face_id,
		UINT colorMask){

	if (lightModel == PhongLighting){
		Vector3D l = ls->pos - pos;
		l.normalize();
		return calcPhongLighting(n, Vector3D(0,0,0) - dir, l, color, 
			ls->color, &(m->material), colorMask);
	} else {
		return Color3(0,0,0);
	}
	return Color3(0,0,0);
}

Vector3D LightShadeModel::calcNormal(Vector3D pos, Model* m, UINT face_id){
	Vector3D n(0,0,0);
	if (shadeModel == PhongShading){
		n = calcPhongNormal(pos,m,face_id);
	} else if (shadeModel == FacetedShading){
		n = m->mesh->m_pFace[face_id].m_vNormal;
	}
	return n;
}

Vector3D LightShadeModel::calcPhongNormal(Vector3D pos, Model *m, UINT face_id){
	UINT* vList = m->mesh->m_pFace[face_id].m_piVertex;

	Vector3D alpha(0,0,0);
	if (!inTriangle(m->mesh->m_pVertex[vList[0]].m_vPosition,
				    m->mesh->m_pVertex[vList[1]].m_vPosition,
					m->mesh->m_pVertex[vList[2]].m_vPosition,
					pos, alpha)){
		return Vector3D(0,0,0);
	}
	Vector3D norm(0,0,0);
	norm += m->mesh->m_pVertex[vList[0]].m_vNormal * alpha.x;
	norm += m->mesh->m_pVertex[vList[1]].m_vNormal * alpha.y;
	norm += m->mesh->m_pVertex[vList[2]].m_vNormal * alpha.z;
	return norm;
}

Color3 LightShadeModel::calcPhongLighting(
		Vector3D n, Vector3D v, Vector3D l, 
		Color3 v_color, Color3 ls_clr,
		Material* obj_mat, UINT colorMask){
	
	Color3 totalColor(0, 0, 0);
	if (colorMask & MASK_AMBIENT) {
		Color3 ac = v_color * ls_clr * obj_mat->ambient;
		totalColor += ac;
	}

	Vector3D r =  2 * (l * n) * n - l;

	if (colorMask & MASK_DIFFUSE) {
		double diff = l*n;
		if (diff < 0) diff = 0;
		Color3 dc = diff * v_color * ls_clr * obj_mat->diffuse;
		totalColor += dc;
	}
	
	if (colorMask & MASK_SPECULAR) {
		double spec = r*v;
		if (spec < 0) spec = 0;
		spec = pow(spec, double(obj_mat->shininess));
		Color3 sc = spec * v_color * ls_clr * obj_mat->specular;
		totalColor += sc;
	}

	return totalColor;
}