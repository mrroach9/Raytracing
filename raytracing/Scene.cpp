#include "Scene.h"


Scene::Scene(){
	modelList.clear();
	lightList.clear();
}

void Scene::addModel(Model* m){
	modelList.push_back(m);
}

void Scene::addLight(LightSource* l){
	lightList.push_back(l);
}

void Scene::setCamera(Camera* c){
	camera = c;
}

void Scene::transformModelAndLight(){
	Vector3D u = camera->up;
	Vector3D d = camera->dir;
	Vector3D o = camera->pos;
	Vector3D l = u^d;

	for (UINT i = 0; i < modelList.size(); ++i){
		Model* m = modelList[i];
		for (UINT j = 0; j < m->mesh->m_nVertex; ++j){
			Vector3D v = m->mesh->m_pVertex[j].m_vPosition;
			v -= o;
			Vector3D nv(v*l, v*u, v*d);
			m->mesh->m_pVertex[j].m_vPosition = nv;
		}
		for (UINT j = 0; j < m->mesh->m_nFace; ++j){
			m->mesh->calcFaceNormal(j);
		}
		for (UINT j = 0; j < m->mesh->m_nVertex; ++j) {
			m->mesh->calcVertexNormal(j);
		}
	}

	for (UINT i = 0; i < lightList.size(); ++i) {
		LightSource* ls = lightList[i];
		Vector3D v = ls->pos;
		v -= o;
		Vector3D nv;
		nv.x = v*l;
		nv.y = v*u;
		nv.z = v*d;
		ls->pos = nv;
	}
}