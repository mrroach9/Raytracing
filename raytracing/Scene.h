#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include "Model.h"
#include "LightSource.h"
#include "Camera.h"
#include "Geometry.h"

using namespace std;

class Scene{
public:
	vector<Model*> modelList;
	vector<LightSource*> lightList;
	Camera* camera;

public:
	Scene();
	void addModel(Model* m);
	void addLight(LightSource* l);
	void setCamera(Camera* c);

	//Convert view point to (0,0,0), looking towards z axis, and heads up along y axis
	void transformModelAndLight();
};

#endif	//_SCENE_H_