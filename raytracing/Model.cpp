#include "Model.h"

Model::Model(){
	mesh = NULL;
	kdtree = NULL;
	translation = Vector3D(0,0,0);
	rotation = Vector3D(0,0,0);
	scale = Vector3D(1,1,1);
	material = Material();
}

Model::Model(CMesh* mesh, Vector3D t, Vector3D r, Vector3D s, Material m){
	this->mesh = new CMesh(*mesh);
	kdtree = NULL;
	translation = t;
	rotation = r;
	scale = s;
	material = m;
}

Model::Model(const Model &m){
	mesh = new CMesh(*(m.mesh));
	kdtree = NULL;
	translation = m.translation;
	rotation = m.rotation;
	scale = m.scale;
	material = m.material;
}

void Model::getTransformedMesh(){
	Vector3D r = rotation;
	Vector3D rx = Vector3D(cos(r.y)*cos(r.z),
				 -cos(r.x)*sin(r.z) + sin(r.x)*sin(r.y)*cos(r.z),
				  sin(r.x)*sin(r.z) + cos(r.x)*sin(r.y)*cos(r.z));
	Vector3D ry = Vector3D(cos(r.y)*sin(r.z),
				  cos(r.x)*cos(r.z) + sin(r.x)*sin(r.y)*sin(r.z),
				 -sin(r.x)*cos(r.z) + cos(r.x)*sin(r.y)*sin(r.z));
	Vector3D rz = Vector3D(-sin(r.y), sin(r.x)*cos(r.y), cos(r.x)*cos(r.y));

	for (UINT i = 0; i < mesh->m_nVertex; ++i){
		Vector3D v = mesh->m_pVertex[i].m_vPosition;
		v.x *= scale.x;
		v.y *= scale.y;
		v.z *= scale.z;
		Vector3D tv;
		tv.x = rx * v;
		tv.y = ry * v;
		tv.z = rz * v;
		tv += translation;
		mesh->m_pVertex[i].m_vPosition = tv;
	}
	for (UINT i = 0; i < mesh->m_nFace; ++i){
		mesh->calcFaceNormal(i);
	}
	for (UINT i = 0; i < mesh->m_nVertex; ++i){
		mesh->calcVertexNormal(i);
	}
}

void Model::buildKdTree(){
	kdtree = new KdTree(this->mesh);
}