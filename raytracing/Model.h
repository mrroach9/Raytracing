#ifndef _MODEL_H_
#define _MODEL_H_

#include "Mesh.h"
#include "Material.h"
#include "Geometry.h"
#include "KdTree.h"
#include "jsoncpp\json.h"

class Model{
public:
	CMesh* mesh;
	//scale first, then rotation, finally translation
	Vector3D translation;
	//TODO: rotation
	Vector3D rotation;
	Vector3D scale;
	Material material;
	KdTree*	kdtree;

public:
	Model();
	Model(CMesh* mesh, Vector3D t, Vector3D r, Vector3D s, Material m);
	Model(const Model &model);
	Model(Json::Value json);
	void getTransformedMesh();
	void buildKdTree();
};

#endif //_MODEL_H_