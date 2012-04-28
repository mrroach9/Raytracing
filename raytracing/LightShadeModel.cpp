#include "LightShadeModel.h"

LightModel LightShadeModel::lightModel = LightModel::PhongLighting;
ShadeModel LightShadeModel::shadeModel = ShadeModel::PhongShading;

void LightShadeModel::setLightModel(LightModel lm){
	lightModel = lm;
}

void LightShadeModel::setShadeModel(ShadeModel sm){
	shadeModel = sm;
}

Color3 LightShadeModel::calcColor(
		Vector3D pos, Vector3D dir, Vector3D n, Color3 color,
		LightSource *ls, Model *m, UINT face_id,
		bool amb_only){

	if (lightModel == PhongLighting){
		Vector3D l = ls->pos - pos;
		l.normalize();
		return calcPhongLighting(n, Vector3D(0,0,0) - dir, l, color, 
			&(ls->material), &(m->material), amb_only);
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
		Color3 v_color, Material* ls_mat,
		Material* obj_mat, bool amb_only){
	
	Color3 ac = v_color * ls_mat->ambient * obj_mat->ambient;
	if (amb_only) {
		return ac;
	}

	Vector3D r =  2*fabs(l*n)*n - l;

	double diff = l*n;
	if (diff < 0) diff = 0;
	double spec = r*v;
	if (spec < 0) spec = 0;
	Color3 dc = diff * v_color * ls_mat->diffuse * obj_mat->diffuse;
	spec = pow(spec, double(obj_mat->shininess));
	Color3 sc = spec * v_color * ls_mat->specular * obj_mat->specular;

	return ac + sc + dc;
}